#include "stm8s_conf.h"
#include "uart.h"
#include "cmd.h"
#include "onewire.h"

#define CMD_MAXLINE 32

static char linebuf[CMD_MAXLINE];
static unsigned char line_len = 0;
static const char *prompt = "cmd> ";
static void (*i2c_scan_hook)(void) = 0;
static unsigned char (*i2c_read_reg_hook)(unsigned char, unsigned char, unsigned char*) = 0;
static unsigned char (*i2c_write_reg_hook)(unsigned char, unsigned char, unsigned char) = 0;

static void uart_puthex16(uint16_t value)
{
	uart_puthex8((unsigned char)(value >> 8));
	uart_puthex8((unsigned char)(value & 0xFF));
}

static void usage(void)
{
	uart_puts("micro-mon: v0.3\r\n");
	uart_puts("  h|?          help\r\n");
	uart_puts("  i            scan i2c bus\r\n");
	uart_puts("  r <a> <r>    i2c read reg (hex bytes)\r\n");
	uart_puts("  w <a> <r> <v> i2c write reg (hex bytes)\r\n");
	uart_puts("  ow ...       dallas 1-wire placeholders\r\n");
}

static char* next_field(char** p)
{
	char* s = *p;
	char* start;

	while (*s == ' ') {
		s++;
	}

	if (*s == '\0') {
		*p = s;
		return 0;
	}

	start = s;
	while (*s && *s != ' ') {
		s++;
	}

	if (*s) {
		*s++ = '\0';
	}

	*p = s;
	return start;
}

static unsigned char parse_hex(const char* s, int16_t* out)
{
	int16_t n = 0;
	char c;
	if (*s == '\0') {
		return 0;
	}
	while ((c = *s++) != '\0') {
		n <<= 4;
		if (c >= '0' && c <= '9') {
			n |= (int16_t)(c - '0');
		} else if (c >= 'a' && c <= 'f') {
			n |= (int16_t)(c - 'a' + 10);
		} else if (c >= 'A' && c <= 'F') {
			n |= (int16_t)(c - 'A' + 10);
		} else {
			return 0;
		}
	}
	*out = n;
	return 1;
}

static void exec_line(char* line)
{
	char* cmd = line;
	char* args = 0;
	char* a1;
	char* a2;
	char* a3;
	int16_t n = 0;
	unsigned char ok = 0;
	int16_t a = 0;
	int16_t r = 0;
	int16_t v = 0;
	unsigned char rv = 0;

	while (*cmd == ' ') {
		cmd++;
	}
	if (*cmd == '\0') {
		return;
	}

	args = cmd;
	while (*args && *args != ' ') {
		args++;
	}
	if (*args) {
		*args++ = '\0';
		while (*args == ' ') {
			args++;
		}
	}

	if (cmd[0] == 'h' || cmd[0] == '?') {
		usage();
		return;
	}

	if (cmd[0] == 'i' && cmd[1] == '\0') {
		if (i2c_scan_hook) {
			i2c_scan_hook();
		} else {
			uart_puts("ERR: no i2c scan hook\r\n");
		}
		return;
	}

	if (cmd[0] == 'r' && cmd[1] == '\0') {
		a1 = next_field(&args);
		a2 = next_field(&args);
		if (!a1 || !a2 || !parse_hex(a1, &a) || !parse_hex(a2, &r)) {
			uart_puts("ERR: usage r <addr> <reg>\r\n");
			return;
		}

		if (!i2c_read_reg_hook) {
			uart_puts("ERR: no i2c read hook\r\n");
			return;
		}

		rv = i2c_read_reg_hook((unsigned char)a, (unsigned char)r, (unsigned char*)&v);
		if (rv == 1) {
			uart_puts("  0x");
			uart_puthex8((unsigned char)v);
			uart_puts("\r\n");
		} else if (rv == 2) {
			uart_puts("ERR: i2c timeout\r\n");
		} else {
			uart_puts("ERR: i2c no-ack\r\n");
		}
		return;
	}

	if (cmd[0] == 'w' && cmd[1] == '\0') {
		a1 = next_field(&args);
		a2 = next_field(&args);
		a3 = next_field(&args);
		if (!a1 || !a2 || !a3 || !parse_hex(a1, &a) || !parse_hex(a2, &r) || !parse_hex(a3, &v)) {
			uart_puts("ERR: usage w <addr> <reg> <val>\r\n");
			return;
		}

		if (!i2c_write_reg_hook) {
			uart_puts("ERR: no i2c write hook\r\n");
			return;
		}

		rv = i2c_write_reg_hook((unsigned char)a, (unsigned char)r, (unsigned char)v);
		if (rv == 1) {
			uart_puts("ok\r\n");
		} else if (rv == 2) {
			uart_puts("ERR: i2c timeout\r\n");
		} else {
			uart_puts("ERR: i2c no-ack\r\n");
		}
		return;
	}

	if (cmd[0] == 'o' && cmd[1] == 'w' && cmd[2] == '\0') {
		onewire_handle_command(args);
		return;
	}

	uart_puts("ERR: unknown command\r\n");
}

void cmd_set_i2c_scan_hook(void (*scan_fn)(void))
{
	i2c_scan_hook = scan_fn;
}

void cmd_set_i2c_rw_hooks(unsigned char (*read_reg_fn)(unsigned char addr, unsigned char reg, unsigned char* value),
	unsigned char (*write_reg_fn)(unsigned char addr, unsigned char reg, unsigned char value))
{
	i2c_read_reg_hook = read_reg_fn;
	i2c_write_reg_hook = write_reg_fn;
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
