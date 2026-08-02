#define _POSIX_C_SOURCE 199309L

/*
 * I2C scanner using libmpsse.
 */

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#if defined(__has_include)
#if __has_include(<mpsse.h>)
#define HAVE_MPSSE_H 1
#include <mpsse.h>
#elif __has_include("mpsse.h")
#define HAVE_MPSSE_H 1
#include "mpsse.h"
#endif
#else
#define HAVE_MPSSE_H 1
#include <mpsse.h>
#endif

#ifndef HAVE_MPSSE_H
int main(void)
{
	fprintf(stderr, "libmpsse headers were not found.\n");
	fprintf(stderr, "Rebuild with CPPFLAGS pointing at the directory containing mpsse.h.\n");
	return 1;
}
#else

#define DEFAULT_CLOCK_HZ 100000U
#define DEFAULT_START_ADDR 0x03U
#define DEFAULT_END_ADDR 0x77U
#define DEFAULT_VID 0x0403
#define DEFAULT_PID 0x6014

struct app_config {
	uint32_t index;
	uint32_t clock_hz;
	unsigned int repeat_ms;
	uint8_t start_addr;
	uint8_t end_addr;
};

static void print_usage(const char *program)
{
	printf("Usage: %s [options]\n", program);
	printf("  -i, --index N          FT232H device index (default: 0)\n");
	printf("  -f, --frequency HZ     I2C clock rate in Hz (default: 100000)\n");
	printf("  -r, --repeat MS        Repeat the scan every N milliseconds\n");
	printf("  -s, --start ADDR       First 7-bit address to probe (default: 0x03)\n");
	printf("  -e, --end ADDR         Last 7-bit address to probe (default: 0x77)\n");
	printf("  -h, --help             Show this help\n");
}

static int parse_u32(const char *text, uint32_t *value)
{
	char *end = NULL;
	unsigned long parsed;

	errno = 0;
	parsed = strtoul(text, &end, 0);
	if (errno != 0 || end == text || *end != '\0' || parsed > 0xFFFFFFFFUL) {
		return -1;
	}

	*value = (uint32_t) parsed;
	return 0;
}

static int parse_u8(const char *text, uint8_t *value)
{
	uint32_t parsed;

	if (parse_u32(text, &parsed) != 0 || parsed > 0x7FU) {
		return -1;
	}

	*value = (uint8_t) parsed;
	return 0;
}

static int parse_args(int argc, char **argv, struct app_config *config)
{
	int index;

	config->index = 0;
	config->clock_hz = DEFAULT_CLOCK_HZ;
	config->repeat_ms = 0;
	config->start_addr = DEFAULT_START_ADDR;
	config->end_addr = DEFAULT_END_ADDR;

	for (index = 1; index < argc; ++index) {
		const char *arg = argv[index];

		if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
			print_usage(argv[0]);
			return 1;
		}

		if ((strcmp(arg, "-i") == 0 || strcmp(arg, "--index") == 0) && index + 1 < argc) {
			if (parse_u32(argv[++index], &config->index) != 0) {
				fprintf(stderr, "invalid device index: %s\n", argv[index]);
				return -1;
			}
			continue;
		}

		if ((strcmp(arg, "-f") == 0 || strcmp(arg, "--frequency") == 0) && index + 1 < argc) {
			if (parse_u32(argv[++index], &config->clock_hz) != 0 || config->clock_hz == 0) {
				fprintf(stderr, "invalid frequency: %s\n", argv[index]);
				return -1;
			}
			continue;
		}

		if ((strcmp(arg, "-r") == 0 || strcmp(arg, "--repeat") == 0) && index + 1 < argc) {
			uint32_t repeat_ms;

			if (parse_u32(argv[++index], &repeat_ms) != 0) {
				fprintf(stderr, "invalid repeat interval: %s\n", argv[index]);
				return -1;
			}

			config->repeat_ms = (unsigned int) repeat_ms;
			continue;
		}

		if ((strcmp(arg, "-s") == 0 || strcmp(arg, "--start") == 0) && index + 1 < argc) {
			if (parse_u8(argv[++index], &config->start_addr) != 0) {
				fprintf(stderr, "invalid start address: %s\n", argv[index]);
				return -1;
			}
			continue;
		}

		if ((strcmp(arg, "-e") == 0 || strcmp(arg, "--end") == 0) && index + 1 < argc) {
			if (parse_u8(argv[++index], &config->end_addr) != 0) {
				fprintf(stderr, "invalid end address: %s\n", argv[index]);
				return -1;
			}
			continue;
		}

		fprintf(stderr, "unknown or incomplete option: %s\n", arg);
		return -1;
	}

	if (config->start_addr > config->end_addr) {
		fprintf(stderr, "start address must be less than or equal to end address\n");
		return -1;
	}

	return 0;
}

static void sleep_ms(unsigned int milliseconds)
{
#ifdef _WIN32
	Sleep(milliseconds);
#else
	struct timespec request;

	request.tv_sec = milliseconds / 1000U;
	request.tv_nsec = (long) (milliseconds % 1000U) * 1000000L;
	nanosleep(&request, NULL);
#endif
}

static struct mpsse_context *open_context(const struct app_config *config)
{
	struct mpsse_context *context;

	context = OpenIndex(
		DEFAULT_VID,
		DEFAULT_PID,
		I2C,
		(int) config->clock_hz,
		MSB,
		IFACE_A,
		NULL,
		NULL,
		(int) config->index);
	if (context == NULL) {
		fprintf(stderr, "OpenIndex returned NULL. Is the FT232H connected?\n");
		return NULL;
	}

	if (!context->open) {
		fprintf(stderr, "Unable to open FT232H device index %u: %s\n",
			(unsigned int) config->index,
			ErrorString(context));
		Close(context);
		return NULL;
	}

	return context;
}

static int probe_address(struct mpsse_context *context, uint8_t address)
{
	char write_address;
	int status;

	write_address = (char) (address << 1);
	status = Start(context);
	if (status != MPSSE_OK) {
		return MPSSE_FAIL;
	}

	status = Write(context, &write_address, 1);
	Stop(context);
	if (status != MPSSE_OK) {
		return MPSSE_FAIL;
	}

	return GetAck(context) == ACK ? MPSSE_OK : MPSSE_FAIL;
}

static unsigned int scan_bus(struct mpsse_context *context, const struct app_config *config)
{
	uint32_t address;
	unsigned int found = 0;

	printf("Scanning addresses 0x%02X to 0x%02X at %u Hz on channel %u\n",
		config->start_addr,
		config->end_addr,
		(unsigned int) config->clock_hz,
		(unsigned int) config->index);

	for (address = config->start_addr; address <= config->end_addr; ++address) {
		if (probe_address(context, (uint8_t) address) == MPSSE_OK) {
			printf("Found device at 0x%02X (%u)\n", (unsigned int) address, (unsigned int) address);
			++found;
		}
	}

	if (found == 0) {
		printf("No I2C devices found.\n");
	} else {
		printf("Found %u device(s).\n", found);
	}

	return found;
}

int main(int argc, char **argv)
{
	struct app_config config;
	struct mpsse_context *context;
	int parse_status;

	parse_status = parse_args(argc, argv, &config);
	if (parse_status > 0) {
		return 0;
	}

	if (parse_status < 0) {
		print_usage(argv[0]);
		return 2;
	}

	context = open_context(&config);
	if (context == NULL) {
		return 1;
	}

	do {
		scan_bus(context, &config);
		if (config.repeat_ms > 0) {
			putchar('\n');
			sleep_ms(config.repeat_ms);
		}
	} while (config.repeat_ms > 0);

	Close(context);
	return 0;
}
#endif
