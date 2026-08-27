/*
 * Steve Buer
 * Olympic College
 * August 2026
 *
 * Simple 8051 Command Interpreter (1-char)
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "uart.h"

/* tiny user stack for commands */

#define USER_STACK_SIZE 4

uint16_t ustack[USER_STACK_SIZE];

uint8_t usp = 0;

/* user stack implementation */

void ustack_push(char n)
{
	ustack[usp++] = (uint16_t) (n - 48);
}

void ustack_clear(void)
{
	memset(ustack, 0, sizeof(ustack));
	usp = 0;
}

void ustack_print(void)
{
	uint8_t idx = usp;

	while (idx != 0) {

		printf("%d: %02x\n", (idx - 1), ustack[idx - 1]);
		idx--;
	}
}

/* shell-style command dispatch */

static void usage()
{
	puts("h|? \t help");
	puts("c   \t clear stack");
	puts("i   \t i2c scan");
	puts("l   \t led control");
	puts("p   \t print stack");
}

void process_command(void)
{
	/* copy then clear flag and buffer */

	unsigned char cmd = cmd_buf;

	cmd_flag = 0;

	cmd_buf = 0;

	/* process */

	if (cmd >= '0' && cmd <= '9')
		ustack_push(cmd);
	else if (cmd == 'c')
		ustack_clear();
	else if (cmd == 'p')
		ustack_print();
	else
		usage();
}
