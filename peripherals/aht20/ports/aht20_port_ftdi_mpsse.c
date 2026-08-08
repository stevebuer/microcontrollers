/**
 * @file    aht20_port_ftdi_mpsse.c
 * @brief   AHT20 platform port for an FTDI FT232H (e.g. Adafruit's FT232H
 *          breakout) using libftdi1, driving the chip's MPSSE hardware
 *          engine to generate real I2C on SCL/SDA.
 *
 * WIRING (standard FTDI application-note wiring for I2C over MPSSE):
 *   ADBUS0 (SK)      -> SCL
 *   ADBUS1 (DO)      -> SDA  \_ jumper these two pins together on the
 *   ADBUS2 (DI)      -> SDA  /  breakout board; this combined line is SDA.
 *   External pull-up resistors (~4.7k) from SCL and SDA to VDD -- MPSSE
 *   pins are push-pull, not open-drain, so DO is only ever driven LOW or
 *   released to float (tri-stated); it can never drive the line HIGH.
 *   Getting the actual HIGH level is entirely the external pull-up's job,
 *   same as on any other I2C bus.
 *
 * WHY DO+DI ARE JUMPERED TOGETHER: this is the standard trick for
 * emulating open-drain I2C on hardware (MPSSE) that only has push-pull
 * GPIO. To drive SDA=0, set DO as an output driving low. To "drive"
 * SDA=1, set DO as an *input* (tri-state) so it stops fighting the
 * external pull-up. To read the actual bus level (e.g. to sample an ACK
 * bit, or read data the AHT20 is driving), read back through DI, which is
 * wired to the same physical net.
 *
 * libftdi1 has no built-in I2C protocol layer (unlike FTDI's own
 * proprietary libMPSSE-I2C) -- only raw MPSSE command access. So this
 * file bit-bangs the I2C protocol (START/STOP/ACK/data bits) on top of
 * MPSSE's "set/read GPIO pins" commands, one bit at a time. This trades
 * some speed for being straightforward to read, verify against the MPSSE
 * command reference, and debug on a scope/logic analyzer.
 *
 * NOTE: this file has been compiled against real libftdi1 1.5 headers,
 * but has NOT been run against real hardware -- I don't have a physical
 * FT232H available to test against. Treat the protocol-level logic as
 * carefully-written-but-unverified until you've confirmed it against a
 * real AHT20 (or at minimum, a scope/logic analyzer on SCL/SDA).
 */

/* usleep() isn't visible under strict -std=c99 without this -- it's a
 * POSIX function, not part of the C standard itself. */
#define _DEFAULT_SOURCE

#include "aht20.h"
#include <ftdi.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* Adafruit's FT232H breakout (and most FT232H boards) ship with FTDI's
 * generic default VID/PID -- correct unless you've reprogrammed the
 * EEPROM with a custom ID. */
#define FT232H_VID 0x0403
#define FT232H_PID 0x6014

/* MPSSE pin bits on the low byte (ADBUS0-7) */
#define PIN_SCL (1u << 0)  /* ADBUS0 */
#define PIN_SDA_OUT (1u << 1)  /* ADBUS1 (DO) -- drives SDA low, or released */
#define PIN_SDA_IN  (1u << 2)  /* ADBUS2 (DI) -- reads actual SDA level */

/* MPSSE commands used (see FTDI AN_108, "Command Processor for MPSSE") */
#define MPSSE_SET_BITS_LOW   0x80
#define MPSSE_GET_BITS_LOW   0x81
#define MPSSE_LOOPBACK_OFF   0x85
#define MPSSE_CLOCK_DIVISOR  0x86
#define MPSSE_CLK_DIV5_ON    0x8B  /* base clock = 12MHz (standard/compat) */
#define MPSSE_ADAPTIVE_OFF   0x97
#define MPSSE_3PHASE_OFF     0x8D

/* Target ~100kHz (standard-mode I2C). Base clock 12MHz (div/5 enabled):
 *   f = 12MHz / ((1 + divisor) * 2)  =>  divisor = 12MHz/(2*100kHz) - 1 = 59
 */
#define I2C_CLOCK_DIVISOR 59u

/* Small delay between pin changes, matching real I2C bus timing rather
 * than slamming edges back-to-back faster than the AHT20 (or the bus
 * capacitance) can follow. 5us gives comfortable margin under 100kHz's
 * ~5us half-period. */
static void mpsse_bit_delay(void)
{
    usleep(5);
}

/* --- Low-level MPSSE pin access ---------------------------------------- */

/**
 * Set the low GPIO byte: `value` is the output level for pins configured
 * as outputs, `direction` is 1=output/0=input per bit. SCL is always
 * driven as an output here (this driver doesn't implement clock-stretch
 * detection -- the AHT20 driver core handles its own timing via delay_ms
 * and status polling, not by relying on the sensor stretching SCL).
 */
static int mpsse_set_pins(struct ftdi_context *ftdi, uint8_t value, uint8_t direction)
{
    uint8_t cmd[3] = { MPSSE_SET_BITS_LOW, value, direction };
    if (ftdi_write_data(ftdi, cmd, sizeof(cmd)) != (int)sizeof(cmd)) {
        return -1;
    }
    return 0;
}

/** Read the low GPIO byte back; returns 1 if SDA (DI) currently reads high. */
static int mpsse_read_sda(struct ftdi_context *ftdi)
{
    uint8_t cmd = MPSSE_GET_BITS_LOW;
    uint8_t rx = 0;

    if (ftdi_write_data(ftdi, &cmd, 1) != 1) {
        return -1;
    }
    /* MPSSE responses can be split across USB packets; a single status
     * byte at 100kHz bit-bang speed is in practice always available
     * quickly, but loop defensively rather than assume one read suffices. */
    for (int attempt = 0; attempt < 100; attempt++) {
        int n = ftdi_read_data(ftdi, &rx, 1);
        if (n == 1) {
            return (rx & PIN_SDA_IN) ? 1 : 0;
        }
        if (n < 0) {
            return -1;
        }
        usleep(100);
    }
    return -1; /* timed out waiting for the read-back byte */
}

/* --- I2C bit/byte primitives, bit-banged on top of the pins above ------ */

/* Idle bus state: SCL high (driven), SDA released (input, pulled high). */
static int i2c_idle(struct ftdi_context *ftdi)
{
    return mpsse_set_pins(ftdi, PIN_SCL, PIN_SCL); /* SCL=1 out, SDA=in */
}

static int i2c_start(struct ftdi_context *ftdi)
{
    /* Both lines released/high first, to guarantee a clean starting point
     * even if the previous transaction didn't end cleanly. */
    if (mpsse_set_pins(ftdi, PIN_SCL, PIN_SCL) != 0) return -1; /* SDA released (high) */
    mpsse_bit_delay();
    /* START: SDA falls while SCL is high. */
    if (mpsse_set_pins(ftdi, PIN_SCL | 0, PIN_SCL | PIN_SDA_OUT) != 0) return -1; /* drive SDA low */
    mpsse_bit_delay();
    /* Drop SCL low so we can start clocking data. */
    if (mpsse_set_pins(ftdi, 0, PIN_SCL | PIN_SDA_OUT) != 0) return -1;
    mpsse_bit_delay();
    return 0;
}

static int i2c_stop(struct ftdi_context *ftdi)
{
    /* Ensure SDA is low and SCL is low first, so the rising edges below
     * are unambiguous START/STOP framing rather than mid-byte glitches. */
    if (mpsse_set_pins(ftdi, 0, PIN_SCL | PIN_SDA_OUT) != 0) return -1; /* SCL=0, SDA=0 */
    mpsse_bit_delay();
    if (mpsse_set_pins(ftdi, PIN_SCL, PIN_SCL | PIN_SDA_OUT) != 0) return -1; /* SCL=1, SDA still 0 */
    mpsse_bit_delay();
    /* STOP: SDA rises while SCL is high. Release SDA (tri-state) so the
     * external pull-up actually pulls it high. */
    if (mpsse_set_pins(ftdi, PIN_SCL, PIN_SCL) != 0) return -1; /* SDA -> input */
    mpsse_bit_delay();
    return 0;
}

/** Clock out one bit (MSB convention handled by the caller). */
static int i2c_write_bit(struct ftdi_context *ftdi, int bit)
{
    uint8_t dir = PIN_SCL | (bit ? 0 : PIN_SDA_OUT); /* bit=1 -> release (input); bit=0 -> drive low */
    uint8_t val = PIN_SCL /* SCL low for this call, set below */;
    (void)val;

    /* SCL low, set SDA for this bit */
    if (mpsse_set_pins(ftdi, 0, dir) != 0) return -1;
    mpsse_bit_delay();
    /* SCL high -- slave samples SDA here */
    if (mpsse_set_pins(ftdi, PIN_SCL, dir) != 0) return -1;
    mpsse_bit_delay();
    /* SCL low again, ready for next bit */
    if (mpsse_set_pins(ftdi, 0, dir) != 0) return -1;
    mpsse_bit_delay();
    return 0;
}

/** Clock in one bit; releases SDA first so the transmitter (slave) can drive it. */
static int i2c_read_bit(struct ftdi_context *ftdi)
{
    int level;

    /* SCL low, release SDA so the other side can drive it */
    if (mpsse_set_pins(ftdi, 0, PIN_SCL) != 0) return -1;
    mpsse_bit_delay();
    /* SCL high -- sample now */
    if (mpsse_set_pins(ftdi, PIN_SCL, PIN_SCL) != 0) return -1;
    mpsse_bit_delay();
    level = mpsse_read_sda(ftdi);
    if (level < 0) return -1;
    /* SCL low again */
    if (mpsse_set_pins(ftdi, 0, PIN_SCL) != 0) return -1;
    mpsse_bit_delay();
    return level;
}

/** Write one byte MSB-first, then clock in and return the ACK bit (0=ACK). */
static int i2c_write_byte(struct ftdi_context *ftdi, uint8_t byte)
{
    for (int i = 7; i >= 0; i--) {
        if (i2c_write_bit(ftdi, (byte >> i) & 1) != 0) {
            return -1;
        }
    }
    return i2c_read_bit(ftdi); /* 0 = ACK, 1 = NACK, <0 = I/O error */
}

/** Read one byte MSB-first, then drive the ACK/NACK bit ourselves (master). */
static int i2c_read_byte(struct ftdi_context *ftdi, uint8_t *out, int send_ack)
{
    uint8_t byte = 0;

    for (int i = 7; i >= 0; i--) {
        int bit = i2c_read_bit(ftdi);
        if (bit < 0) {
            return -1;
        }
        byte = (uint8_t)((byte << 1) | (bit & 1));
    }
    *out = byte;
    /* Master sends ACK (0) to request more bytes, NACK (1) on the last one. */
    return i2c_write_bit(ftdi, send_ack ? 0 : 1);
}

/* --- aht20_ops_t callbacks ---------------------------------------------- */

static int ftdi_i2c_write(void *ctx, uint8_t addr, const uint8_t *data, uint16_t len)
{
    struct ftdi_context *ftdi = (struct ftdi_context *)ctx;
    uint8_t addr_byte = (uint8_t)((addr << 1) | 0u); /* R/W=0 (write) */

    if (i2c_start(ftdi) != 0) return -1;

    if (i2c_write_byte(ftdi, addr_byte) != 0) { /* nonzero = NACK or I/O error */
        i2c_stop(ftdi);
        return -1;
    }

    for (uint16_t i = 0; i < len; i++) {
        if (i2c_write_byte(ftdi, data[i]) != 0) {
            i2c_stop(ftdi);
            return -1;
        }
    }

    return i2c_stop(ftdi);
}

static int ftdi_i2c_read(void *ctx, uint8_t addr, uint8_t *data, uint16_t len)
{
    struct ftdi_context *ftdi = (struct ftdi_context *)ctx;
    uint8_t addr_byte = (uint8_t)((addr << 1) | 1u); /* R/W=1 (read) */

    if (i2c_start(ftdi) != 0) return -1;

    if (i2c_write_byte(ftdi, addr_byte) != 0) {
        i2c_stop(ftdi);
        return -1;
    }

    for (uint16_t i = 0; i < len; i++) {
        int send_ack = (i != (uint16_t)(len - 1)); /* NACK the final byte */
        if (i2c_read_byte(ftdi, &data[i], send_ack) != 0) {
            i2c_stop(ftdi);
            return -1;
        }
    }

    return i2c_stop(ftdi);
}

static void ftdi_delay_ms(uint32_t ms)
{
    usleep(ms * 1000u);
}

/* --- Setup / teardown ---------------------------------------------------- */

/**
 * @brief Open the FT232H, configure MPSSE mode + I2C-safe clock, and fill
 *        in an aht20_ops_t bound to it.
 *
 * @return 0 on success, nonzero on failure (device not found, MPSSE
 *         config failed, etc -- prints a diagnostic to stderr either way).
 *
 * Usage:
 *   struct ftdi_context *ftdi;
 *   aht20_ops_t ops = {0};
 *
 *   if (aht20_port_ftdi_open(&ops, &ftdi) != 0) {
 *       return 1;
 *   }
 *   if (aht20_init(&ops) != AHT20_OK) {
 *       aht20_port_ftdi_close(ftdi);
 *       return 1;
 *   }
 *   ... aht20_measure(&ops, ...) ...
 *   aht20_port_ftdi_close(ftdi);
 */
int aht20_port_ftdi_open(aht20_ops_t *ops, struct ftdi_context **out_ftdi)
{
    struct ftdi_context *ftdi = ftdi_new();
    if (ftdi == NULL) {
        fprintf(stderr, "aht20_port_ftdi: ftdi_new() failed\n");
        return -1;
    }

    if (ftdi_usb_open(ftdi, FT232H_VID, FT232H_PID) < 0) {
        fprintf(stderr, "aht20_port_ftdi: ftdi_usb_open failed: %s\n", ftdi_get_error_string(ftdi));
        ftdi_free(ftdi);
        return -1;
    }

    if (ftdi_usb_reset(ftdi) < 0) {
        fprintf(stderr, "aht20_port_ftdi: ftdi_usb_reset failed: %s\n", ftdi_get_error_string(ftdi));
        goto fail;
    }

    /* Leave BITMODE_RESET briefly before entering MPSSE mode -- avoids
     * starting MPSSE configuration on top of whatever mode the chip
     * happened to power up in. */
    if (ftdi_set_bitmode(ftdi, 0x00, BITMODE_RESET) < 0) goto fail;
    if (ftdi_set_bitmode(ftdi, 0x00, BITMODE_MPSSE) < 0) goto fail;
    usleep(50 * 1000); /* let the mode switch settle before sending commands */

    {
        uint8_t setup[] = {
            MPSSE_LOOPBACK_OFF,
            MPSSE_CLK_DIV5_ON,                 /* base clock = 12MHz */
            MPSSE_CLOCK_DIVISOR, I2C_CLOCK_DIVISOR & 0xFFu, (I2C_CLOCK_DIVISOR >> 8) & 0xFFu,
            MPSSE_ADAPTIVE_OFF,
            MPSSE_3PHASE_OFF,                  /* not using shift commands, doesn't apply here */
        };
        if (ftdi_write_data(ftdi, setup, sizeof(setup)) != (int)sizeof(setup)) {
            fprintf(stderr, "aht20_port_ftdi: MPSSE setup write failed\n");
            goto fail;
        }
    }

    if (i2c_idle(ftdi) != 0) {
        fprintf(stderr, "aht20_port_ftdi: failed to set idle bus state\n");
        goto fail;
    }

    ops->i2c_write = ftdi_i2c_write;
    ops->i2c_read  = ftdi_i2c_read;
    ops->i2c_delay = ftdi_delay_ms;
    ops->ctx       = ftdi;

    *out_ftdi = ftdi;
    return 0;

fail:
    ftdi_usb_close(ftdi);
    ftdi_free(ftdi);
    return -1;
}

void aht20_port_ftdi_close(struct ftdi_context *ftdi)
{
    if (ftdi != NULL) {
        ftdi_usb_close(ftdi);
        ftdi_free(ftdi);
    }
}

static void print_usage(const char *prog)
{
    printf("Usage: %s [--help] [--probe-only|-p] [--json|-j]\n", prog);
    printf("  --help, -h    Show this help text and exit\n");
    printf("  --probe-only, -p  Probe I2C address 0x%02X and exit\n", AHT20_I2C_ADDR);
    printf("  --json, -j    Emit single-line JSON output\n");
}

/* Probe by issuing only the write address byte and checking for ACK. */
static int aht20_probe_address(aht20_ops_t *ops)
{
    return ops->i2c_write(ops->ctx, AHT20_I2C_ADDR, NULL, 0);
}

int main(int argc, char **argv)
{
    bool probe_only = false;
    bool json_output = false;

    for (int i = 1; i < argc; i++) {
        if ((strcmp(argv[i], "--help") == 0) || (strcmp(argv[i], "-h") == 0)) {
            print_usage(argv[0]);
            return 0;
        }
        if ((strcmp(argv[i], "--probe-only") == 0) || (strcmp(argv[i], "-p") == 0)) {
            probe_only = true;
            continue;
        }
        if ((strcmp(argv[i], "--json") == 0) || (strcmp(argv[i], "-j") == 0)) {
            json_output = true;
            continue;
        }

        if (json_output) {
            printf("{\"ok\":false,\"stage\":\"args\",\"error\":\"unknown_option\",\"arg\":\"%s\"}\n", argv[i]);
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
        }
        return 2;
    }

    aht20_ops_t ops = { 0 };
    struct ftdi_context *ftdi = NULL;

    if (aht20_port_ftdi_open(&ops, &ftdi) != 0) {
        if (json_output) {
            printf("{\"ok\":false,\"stage\":\"open\",\"error\":\"ftdi_open_failed\"}\n");
        } else {
            fprintf(stderr, "Failed to open/configure FT232H in MPSSE mode\n");
        }
        return 1;
    }

    if (aht20_probe_address(&ops) != 0) {
        if (json_output) {
            printf("{\"ok\":false,\"stage\":\"probe\",\"probe_ack\":false,\"address\":\"0x%02X\",\"error\":\"no_ack\"}\n", AHT20_I2C_ADDR);
        } else {
            fprintf(stderr, "AHT20 probe at 0x%02X: no ACK\n", AHT20_I2C_ADDR);
        }
        aht20_port_ftdi_close(ftdi);
        return 1;
    }

    if (!json_output) {
        printf("AHT20 probe at 0x%02X: ACK\n", AHT20_I2C_ADDR);
    }

    if (probe_only) {
        if (json_output) {
            printf("{\"ok\":true,\"stage\":\"probe\",\"probe_ack\":true,\"address\":\"0x%02X\"}\n", AHT20_I2C_ADDR);
        }
        aht20_port_ftdi_close(ftdi);
        return 0;
    }

    aht20_status_t rc = aht20_init(&ops);
    if (rc != AHT20_OK) {
        if (json_output) {
            printf("{\"ok\":false,\"stage\":\"init\",\"probe_ack\":true,\"address\":\"0x%02X\",\"status\":%d}\n", AHT20_I2C_ADDR, (int)rc);
        } else {
            fprintf(stderr, "aht20_init failed: %d\n", (int)rc);
        }
        aht20_port_ftdi_close(ftdi);
        return 1;
    }

    float temp_c = 0.0f;
    float hum_pct = 0.0f;
    rc = aht20_measure(&ops, &temp_c, &hum_pct);
    if (rc != AHT20_OK) {
        if (json_output) {
            printf("{\"ok\":false,\"stage\":\"measure\",\"probe_ack\":true,\"address\":\"0x%02X\",\"status\":%d}\n", AHT20_I2C_ADDR, (int)rc);
        } else {
            fprintf(stderr, "aht20_measure failed: %d\n", (int)rc);
        }
        aht20_port_ftdi_close(ftdi);
        return 1;
    }

    if (json_output) {
        printf("{\"ok\":true,\"stage\":\"measure\",\"probe_ack\":true,\"address\":\"0x%02X\",\"temperature_c\":%.2f,\"humidity_pct\":%.2f}\n", AHT20_I2C_ADDR, temp_c, hum_pct);
    } else {
        printf("AHT20: %.2f C, %.2f %%RH\n", temp_c, hum_pct);
    }

    aht20_port_ftdi_close(ftdi);
    return 0;
}
