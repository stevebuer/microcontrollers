#define _POSIX_C_SOURCE 199309L

/*
 * Read BMP180 sensor via libmpsse and Bosch BMP180 driver.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "mpsse.h"
#include "bmp180.h"

#define FT232H_VID 0x0403
#define FT232H_PID 0x6014
#define I2C_CLOCK_HZ 100000
#define DEFAULT_PROBE_LEN 1U
#define MAX_PROBE_LEN 32U

static struct mpsse_context *g_mpsse = NULL;

struct app_options {
	bool json;
	bool probe_enabled;
	bool probe_only;
	u8 probe_register;
	u8 probe_len;
};

static void print_usage(const char *program)
{
	printf("Usage: %s [options]\n", program);
	printf("  --json                 Output sensor values as one-line JSON\n");
	printf("  --probe REG            Read REG before normal BMP180 read (hex or dec)\n");
	printf("  --probe-len N          Number of bytes to read for probe (default: 1)\n");
	printf("  --probe-only           Exit after probe read\n");
	printf("  -h, --help             Show this help\n");
}

static int parse_u8_value(const char *text, u8 *value)
{
	char *end = NULL;
	unsigned long parsed;

	parsed = strtoul(text, &end, 0);
	if (end == text || *end != '\0' || parsed > 0xFFUL) {
		return -1;
	}

	*value = (u8)parsed;
	return 0;
}

static int parse_options(int argc, char **argv, struct app_options *options)
{
	int i;

	options->json = false;
	options->probe_enabled = false;
	options->probe_only = false;
	options->probe_register = 0xD0;
	options->probe_len = DEFAULT_PROBE_LEN;

	for (i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "--json") == 0) {
			options->json = true;
			continue;
		}

		if (strcmp(argv[i], "--probe") == 0 && i + 1 < argc) {
			if (parse_u8_value(argv[++i], &options->probe_register) != 0) {
				fprintf(stderr, "invalid --probe register: %s\n", argv[i]);
				return -1;
			}
			options->probe_enabled = true;
			continue;
		}

		if (strcmp(argv[i], "--probe-len") == 0 && i + 1 < argc) {
			u8 parsed_len;

			if (parse_u8_value(argv[++i], &parsed_len) != 0 || parsed_len == 0 || parsed_len > MAX_PROBE_LEN) {
				fprintf(stderr, "invalid --probe-len value: %s\n", argv[i]);
				return -1;
			}

			options->probe_len = parsed_len;
			continue;
		}

		if (strcmp(argv[i], "--probe-only") == 0) {
			options->probe_only = true;
			options->probe_enabled = true;
			continue;
		}

		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			print_usage(argv[0]);
			return 1;
		}

		fprintf(stderr, "unknown or incomplete option: %s\n", argv[i]);
		return -1;
	}

	return 0;
}

static void sleep_ms(uint32_t msek)
{
	struct timespec request;

	request.tv_sec = msek / 1000U;
	request.tv_nsec = (long)(msek % 1000U) * 1000000L;
	nanosleep(&request, NULL);
}

/* Bosch callback: write cnt bytes to reg_addr. */
s8 BMP180_I2C_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
	char addr_write = (char)(dev_addr << 1);
	char payload[1 + ARRAY_SIZE_TWENTY_TWO];
	int status;

	if (g_mpsse == NULL || cnt > ARRAY_SIZE_TWENTY_TWO) {
		return (s8)-1;
	}

	payload[0] = (char)reg_addr;
	for (u8 i = 0; i < cnt; ++i) {
		payload[1 + i] = (char)reg_data[i];
	}

	status = Start(g_mpsse);
	if (status != MPSSE_OK) {
		return (s8)-1;
	}

	status = Write(g_mpsse, &addr_write, 1);
	if (status != MPSSE_OK || GetAck(g_mpsse) != ACK) {
		Stop(g_mpsse);
		return (s8)-1;
	}

	status = Write(g_mpsse, payload, 1 + cnt);
	if (status != MPSSE_OK || GetAck(g_mpsse) != ACK) {
		Stop(g_mpsse);
		return (s8)-1;
	}

	Stop(g_mpsse);
	return (s8)0;
}

/* Bosch callback: write register pointer then read cnt bytes. */
s8 BMP180_I2C_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
	char addr_write = (char)(dev_addr << 1);
	char addr_read = (char)((dev_addr << 1) | 0x01U);
	char reg = (char)reg_addr;
	char *chunk;
	int status;

	if (g_mpsse == NULL || cnt == 0) {
		return (s8)-1;
	}

	status = Start(g_mpsse);
	if (status != MPSSE_OK) {
		return (s8)-1;
	}

	status = Write(g_mpsse, &addr_write, 1);
	if (status != MPSSE_OK || GetAck(g_mpsse) != ACK) {
		Stop(g_mpsse);
		return (s8)-1;
	}

	status = Write(g_mpsse, &reg, 1);
	if (status != MPSSE_OK || GetAck(g_mpsse) != ACK) {
		Stop(g_mpsse);
		return (s8)-1;
	}

	status = Start(g_mpsse);
	if (status != MPSSE_OK) {
		Stop(g_mpsse);
		return (s8)-1;
	}

	status = Write(g_mpsse, &addr_read, 1);
	if (status != MPSSE_OK || GetAck(g_mpsse) != ACK) {
		Stop(g_mpsse);
		return (s8)-1;
	}

	for (u8 i = 0; i < cnt; ++i) {
		if (i + 1U < cnt) {
			SendAcks(g_mpsse);
		} else {
			SendNacks(g_mpsse);
		}

		chunk = Read(g_mpsse, 1);
		if (chunk == NULL) {
			Stop(g_mpsse);
			return (s8)-1;
		}

		reg_data[i] = (u8)chunk[0];
		free(chunk);
	}

	Stop(g_mpsse);
	return (s8)0;
}

void BMP180_delay_msek(u32 msek)
{
	sleep_ms(msek);
}

static int open_ft232h(void)
{
	g_mpsse = OpenIndex(
		FT232H_VID,
		FT232H_PID,
		I2C,
		I2C_CLOCK_HZ,
		MSB,
		IFACE_A,
		NULL,
		NULL,
		0);

	if (g_mpsse == NULL || !g_mpsse->open) {
		fprintf(stderr, "OpenIndex failed: %s\n", g_mpsse ? ErrorString(g_mpsse) : "NULL context");
		if (g_mpsse != NULL) {
			Close(g_mpsse);
			g_mpsse = NULL;
		}
		return -1;
	}

	return 0;
}

static void close_ft232h(void)
{
	if (g_mpsse != NULL) {
		Close(g_mpsse);
		g_mpsse = NULL;
	}
}

static int probe_registers(u8 reg_addr, u8 *buffer, u8 len)
{
	if (BMP180_I2C_bus_read(BMP180_I2C_ADDR, reg_addr, buffer, len) != 0) {
		return -1;
	}

	return 0;
}

int main(int argc, char **argv)
{
	struct app_options options;
	struct bmp180_t sensor;
	BMP180_RETURN_FUNCTION_TYPE rc;
	u16 ut;
	u32 up;
	s16 temp_tenths_c;
	s32 pressure_pa;
	u8 probe_buf[MAX_PROBE_LEN];
	u8 chip_id = 0;
	int parse_status;

	parse_status = parse_options(argc, argv, &options);
	if (parse_status > 0) {
		return 0;
	}

	if (parse_status < 0) {
		print_usage(argv[0]);
		return 2;
	}

	if (!options.json) {
		printf("Reading BMP180...\n");
	}

	if (open_ft232h() != 0) {
		return 1;
	}

	if (options.probe_enabled) {
		if (probe_registers(options.probe_register, probe_buf, options.probe_len) != 0) {
			fprintf(stderr, "register probe failed at 0x%02X\n", options.probe_register);
			close_ft232h();
			return 3;
		}

		if (options.json) {
			printf("{\"probe\":{\"register\":\"0x%02X\",\"len\":%u,\"data\":[",
				options.probe_register,
				options.probe_len);
			for (u8 i = 0; i < options.probe_len; ++i) {
				printf("\"0x%02X\"%s", probe_buf[i], (i + 1U < options.probe_len) ? "," : "");
			}
			printf("]}}");
		} else {
			printf("probe reg 0x%02X:", options.probe_register);
			for (u8 i = 0; i < options.probe_len; ++i) {
				printf(" 0x%02X", probe_buf[i]);
			}
			printf("\n");
		}

		if (options.probe_only) {
			if (options.json) {
				printf("\n");
			}
			close_ft232h();
			return 0;
		}

		if (options.json) {
			printf("\n");
		}
	}

	memset(&sensor, 0, sizeof(sensor));
	sensor.bus_write = BMP180_I2C_bus_write;
	sensor.bus_read = BMP180_I2C_bus_read;
	sensor.delay_msec = BMP180_delay_msek;
	sensor.dev_addr = BMP180_I2C_ADDR;

	rc = bmp180_init(&sensor);
	if (rc != 0) {
		fprintf(stderr, "bmp180_init failed: %d\n", rc);
		close_ft232h();
		return 2;
	}

	ut = bmp180_get_uncomp_temperature();
	up = bmp180_get_uncomp_pressure();
	temp_tenths_c = bmp180_get_temperature(ut);
	pressure_pa = bmp180_get_pressure(up);
	chip_id = sensor.chip_id;

	if (options.json) {
		double temp_c = (double)temp_tenths_c / 10.0;
		double temp_f = temp_c * 9.0 / 5.0 + 32.0;
		printf(
			"{\"chip_id\":\"0x%02X\",\"temperature_c\":%.1f,\"temperature_f\":%.1f,\"pressure_pa\":%ld,\"pressure_hpa\":%.2f}\n",
			chip_id,
			temp_c,
			temp_f,
			(long)pressure_pa,
			(double)pressure_pa / 100.0);
	} else {
		printf("chip_id=0x%02X\n", chip_id);
		printf("temperature=%d.%d C\n", temp_tenths_c / 10, abs(temp_tenths_c % 10));
		printf("pressure=%ld Pa\n", (long)pressure_pa);
	}

	close_ft232h();
	return 0;
}
