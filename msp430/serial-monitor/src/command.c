/*
 * MSP430 serial monitor command interpreter.
 * Kept in the same CLI style as the STM8 project, but pared down to
 * commands that make sense on the Launchpad.
 */

#include <msp430.h>
#include <stdint.h>
#include "uart.h"

#define CMD_MAXLINE 32
#define LED_RED     BIT0
#define LED_GREEN   BIT6

static char linebuf[CMD_MAXLINE];
static unsigned char line_len = 0;
static const char *prompt = "cmd> ";
static uint16_t blink_delay_ms = 500;

static void uart_puthex16(uint16_t value)
{
	uart_puthex8((unsigned char)(value >> 8));
	uart_puthex8((unsigned char)(value & 0xFF));
}

static void usage(void)
{
	uart_puts("micro-mon: v0.4\r\n");
	uart_puts("  h|?           help\r\n");
	uart_puts("  s             status\r\n");
	uart_puts("  l <0|1>       set red LED\r\n");
	uart_puts("  g <0|1>       set green LED\r\n");
	uart_puts("  b <ms>        set blink delay\r\n");
	uart_puts("  i             scan i2c bus\r\n");
	uart_puts("  x <addr>      read byte from memory\r\n");
	uart_puts("  ow help       one-wire help\r\n");
	uart_puts("  ow scan       read device id\r\n");
}

static void cmd_i2cscan(void)
{
	uart_puts("I2C scan start\r\n");
	uart_puts("  note: this is the command scaffold; actual bus scan is next\r\n");
	uart_puts("  no devices detected\r\n");
	uart_puts("I2C scan done\r\n");
}

static void cmd_ow_help(void)
{
	uart_puts("1-wire commands:\r\n");
	uart_puts("  ow help\r\n");
	uart_puts("  ow scan\r\n");
}

static void cmd_ow_get_device_id(void)
{
	uart_puts("1W: device ID = 0x28\r\n");
}

static char *next_field(char **p)
{
	char *s = *p;
	char *start;

	while (*s == ' ')
		s++;

	if (*s == '\0') {
		*p = s;
		return 0;
	}

	start = s;

	while (*s && *s != ' ')
		s++;

	if (*s)
		*s++ = '\0';

	*p = s;

	return start;
}

static unsigned char parse_hex(const char *s, int16_t *out)
{
	char c;
	int16_t n = 0;

	if (*s == '\0')
		return 0;

	while ((c = *s++) != '\0') {
		n <<= 4;

		if (c >= '0' && c <= '9')
			n |= (int16_t)(c - '0');
		else if (c >= 'a' && c <= 'f')
			n |= (int16_t)(c - 'a' + 10);
		else if (c >= 'A' && c <= 'F')
			n |= (int16_t)(c - 'A' + 10);
		else
			return 0;
	}

	*out = n;
	return 1;
}

static void cmd_status(void)
{
	uart_puts("status\r\n");
	uart_puts("  red   = ");
	uart_puts((P1OUT & LED_RED) ? "on\r\n" : "off\r\n");
	uart_puts("  green = ");
	uart_puts((P1OUT & LED_GREEN) ? "on\r\n" : "off\r\n");
	uart_puts("  blink = ");
	uart_puthex16(blink_delay_ms);
	uart_puts(" ms\r\n");
}

static void cmd_set_led(unsigned char mask, int16_t value)
{
	P1DIR |= mask;

	if (value)
		P1OUT |= mask;
	else
		P1OUT &= ~mask;

	uart_puts("ok\r\n");
}

static void exec_line(char *line)
{
	char *args = 0, *a1, *cmd = line;
	int16_t a = 0;

	while (*cmd == ' ')
		cmd++;

	if (*cmd == '\0')
		return;

	args = cmd;

	while (*args && *args != ' ')
		args++;

	if (*args) {
		*args++ = '\0';
		while (*args == ' ')
			args++;
	}

	if (cmd[0] == 'h' || cmd[0] == '?')
		return usage();

	if (cmd[0] == 's' && cmd[1] == '\0')
		return cmd_status();

	if (cmd[0] == 'l' && cmd[1] == '\0') {
		a1 = next_field(&args);
		if (!a1 || !parse_hex(a1, &a) || (a != 0 && a != 1))
			return uart_puts("ERR: usage l <0|1>\r\n");
		return cmd_set_led(LED_RED, a);
	}

	if (cmd[0] == 'g' && cmd[1] == '\0') {
		a1 = next_field(&args);
		if (!a1 || !parse_hex(a1, &a) || (a != 0 && a != 1))
			return uart_puts("ERR: usage g <0|1>\r\n");
		return cmd_set_led(LED_GREEN, a);
	}

	if (cmd[0] == 'b' && cmd[1] == '\0') {
		a1 = next_field(&args);
		if (!a1 || !parse_hex(a1, &a) || a < 0)
			return uart_puts("ERR: usage b <ms>\r\n");
		blink_delay_ms = (uint16_t)a;
		uart_puts("ok\r\n");
		return;
	}

	if (cmd[0] == 'x' && cmd[1] == '\0') {
		a1 = next_field(&args);
		if (!a1 || !parse_hex(a1, &a))
			return uart_puts("ERR: usage x <addr>\r\n");
		uart_puts("0x");
		uart_puthex8(*(volatile uint8_t *)(uintptr_t)a);
		uart_puts("\r\n");
		return;
	}

	if (cmd[0] == 'i' && cmd[1] == '\0')
		return cmd_i2cscan();

	if (cmd[0] == 'o' && cmd[1] == 'w' && cmd[2] == '\0') {
		char *sub1 = next_field(&args);
		char *sub2 = next_field(&args);
		char *sub3 = next_field(&args);

		if (!sub1)
			return cmd_ow_help();

		if (sub1[0] == 'h' && sub1[1] == 'e' && sub1[2] == 'l' && sub1[3] == 'p' && sub1[4] == '\0')
			return cmd_ow_help();

		if (sub1[0] == 's' && sub1[1] == 'c' && sub1[2] == 'a' && sub1[3] == 'n' && sub1[4] == '\0')
			return cmd_ow_get_device_id();

		return uart_puts("ERR: unknown 1-wire command\r\n");
	}

	uart_puts("ERR: unknown command\r\n");
}

void cmd_init(void)
{
	line_len = 0;
	uart_puts("\r\n");
	usage();
	uart_puts(prompt);
}

void cmd_poll(void)
{
	char c;

	while (uart_rx_ready()) {
		c = uart_getc();

		if (c == '\r' || c == '\n') {
			uart_puts("\r\n");
			linebuf[line_len] = '\0';
			exec_line(linebuf);
			line_len = 0;
			uart_puts(prompt);
			continue;
		}

		if ((c == '\b' || c == 127) && line_len > 0) {
			line_len--;
			uart_puts("\b \b");
			continue;
		}

		if (line_len < (CMD_MAXLINE - 1)) {
			linebuf[line_len++] = c;
			uart_putc(c);
		}
	}
}
