/**
 * @file    test_aht20.c
 * @brief   Host-side tests for the AHT20 driver core, using a mocked I2C bus.
 *
 * No hardware or cross toolchain needed -- build and run with your host gcc:
 *   gcc -I.. -o test_aht20 test_aht20.c ../aht20.c && ./test_aht20
 *
 * The mock simulates the sensor's response sequence so we can check the
 * driver's command bytes, CRC validation, and unit conversion in isolation.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "aht20.h"

/* ---- Mock I2C bus ------------------------------------------------------ */

#define MOCK_MAX_CALLS 16

typedef struct {
    uint8_t data[8];
    uint16_t len;
} mock_call_t;

static mock_call_t g_writes[MOCK_MAX_CALLS];
static uint8_t g_write_count = 0;

/* Queue of responses returned by successive reads, in order. */
static uint8_t g_read_queue[MOCK_MAX_CALLS][8];
static uint16_t g_read_len[MOCK_MAX_CALLS];
static uint8_t g_read_count = 0;
static uint8_t g_read_index = 0;

static void mock_reset(void)
{
    g_write_count = 0;
    g_read_count = 0;
    g_read_index = 0;
    memset(g_writes, 0, sizeof(g_writes));
}

static void mock_queue_read(const uint8_t *data, uint16_t len)
{
    memcpy(g_read_queue[g_read_count], data, len);
    g_read_len[g_read_count] = len;
    g_read_count++;
}

static int mock_i2c_write(void *ctx, uint8_t addr, const uint8_t *data, uint16_t len)
{
    (void)ctx;
    assert(addr == AHT20_I2C_ADDR);
    assert(g_write_count < MOCK_MAX_CALLS);
    memcpy(g_writes[g_write_count].data, data, len);
    g_writes[g_write_count].len = len;
    g_write_count++;
    return 0;
}

static int mock_i2c_read(void *ctx, uint8_t addr, uint8_t *data, uint16_t len)
{
    (void)ctx;
    assert(addr == AHT20_I2C_ADDR);
    assert(g_read_index < g_read_count);
    assert(len == g_read_len[g_read_index]);
    memcpy(data, g_read_queue[g_read_index], len);
    g_read_index++;
    return 0;
}

static void mock_delay_ms(void *ctx, uint32_t ms)
{
    (void)ctx;
    (void)ms; /* instant in the mock */
}

/* Same CRC8 (poly 0x31, init 0xFF) as the driver, used to build fixtures. */
static uint8_t crc8(const uint8_t *data, uint16_t len)
{
    uint8_t crc = 0xFFu;
    for (uint16_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t b = 0; b < 8; b++) {
            crc = (crc & 0x80u) ? (uint8_t)((crc << 1) ^ 0x31u) : (uint8_t)(crc << 1);
        }
    }
    return crc;
}

/* ---- Tests -------------------------------------------------------------- */

static void test_init_sends_correct_sequence_and_checks_calibration(void)
{
    mock_reset();
    aht20_platform_t plat = {
        .i2c_write = mock_i2c_write, .i2c_read = mock_i2c_read, .delay_ms = mock_delay_ms, .ctx = NULL
    };

    uint8_t status_calibrated = 0x08u; /* CAL bit set, not busy */
    mock_queue_read(&status_calibrated, 1);

    aht20_t dev;
    aht20_status_t rc = aht20_init(&dev, &plat);

    assert(rc == AHT20_OK);
    assert(dev.initialized);
    assert(g_write_count == 1);
    assert(g_writes[0].len == 3);
    assert(g_writes[0].data[0] == 0xBEu); /* init command */
    assert(g_writes[0].data[1] == 0x08u);
    assert(g_writes[0].data[2] == 0x00u);
    printf("PASS: init sequence + calibration check\n");
}

static void test_init_fails_when_not_calibrated(void)
{
    mock_reset();
    aht20_platform_t plat = {
        .i2c_write = mock_i2c_write, .i2c_read = mock_i2c_read, .delay_ms = mock_delay_ms, .ctx = NULL
    };

    uint8_t status_not_calibrated = 0x00u;
    mock_queue_read(&status_not_calibrated, 1);

    aht20_t dev;
    aht20_status_t rc = aht20_init(&dev, &plat);

    assert(rc == AHT20_ERR_NOT_CALIBRATED);
    assert(!dev.initialized);
    printf("PASS: init reports uncalibrated sensor\n");
}

static void test_measure_converts_known_raw_values(void)
{
    mock_reset();
    aht20_platform_t plat = {
        .i2c_write = mock_i2c_write, .i2c_read = mock_i2c_read, .delay_ms = mock_delay_ms, .ctx = NULL
    };

    uint8_t status_calibrated = 0x08u;
    mock_queue_read(&status_calibrated, 1);
    aht20_t dev;
    assert(aht20_init(&dev, &plat) == AHT20_OK);

    /* Craft a raw frame for a known humidity/temperature reading.
     * humidity_raw = 524288 (=> 50.0 %RH), temperature_raw = 419430 (=> ~30.0 C) */
    uint32_t hum_raw = 524288u;
    uint32_t temp_raw = 419430u;

    uint8_t frame[7];
    frame[0] = 0x00u; /* status: not busy */
    frame[1] = (uint8_t)(hum_raw >> 12);
    frame[2] = (uint8_t)(hum_raw >> 4);
    frame[3] = (uint8_t)(((hum_raw & 0x0Fu) << 4) | ((temp_raw >> 16) & 0x0Fu));
    frame[4] = (uint8_t)(temp_raw >> 8);
    frame[5] = (uint8_t)(temp_raw);
    frame[6] = crc8(frame, 6);

    mock_queue_read(&frame[0], 1); /* status poll inside aht20_wait_ready */
    mock_queue_read(frame, 7);     /* full frame read */

    float temp_c = 0.0f, hum_pct = 0.0f;
    aht20_status_t rc = aht20_measure(&dev, &temp_c, &hum_pct);

    assert(rc == AHT20_OK);
    assert(hum_pct > 49.9f && hum_pct < 50.1f);
    assert(temp_c > 29.9f && temp_c < 30.1f);
    printf("PASS: measure() converts raw counts to %.2f C / %.2f %%RH\n", temp_c, hum_pct);
}

static void test_measure_rejects_bad_crc(void)
{
    mock_reset();
    aht20_platform_t plat = {
        .i2c_write = mock_i2c_write, .i2c_read = mock_i2c_read, .delay_ms = mock_delay_ms, .ctx = NULL
    };

    uint8_t status_calibrated = 0x08u;
    mock_queue_read(&status_calibrated, 1);
    aht20_t dev;
    assert(aht20_init(&dev, &plat) == AHT20_OK);

    uint8_t frame[7] = { 0x00u, 0x11u, 0x22u, 0x33u, 0x44u, 0x55u, 0x00u /* wrong CRC */ };
    mock_queue_read(&frame[0], 1);
    mock_queue_read(frame, 7);

    aht20_status_t rc = aht20_measure(&dev, NULL, NULL);
    assert(rc == AHT20_ERR_CRC);
    printf("PASS: measure() rejects a corrupted frame via CRC8\n");
}

int main(void)
{
    test_init_sends_correct_sequence_and_checks_calibration();
    test_init_fails_when_not_calibrated();
    test_measure_converts_known_raw_values();
    test_measure_rejects_bad_crc();
    printf("\nAll tests passed.\n");
    return 0;
}
