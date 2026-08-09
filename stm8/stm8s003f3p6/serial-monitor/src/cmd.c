#include "stm8s_conf.h"
#include "uart.h"
#include "cmd.h"

#define CMD_MAXLINE 32
#define USER_STACK_SIZE 8

static char linebuf[CMD_MAXLINE];
static unsigned char line_len = 0;
static const char *prompt = "cmd> ";
static int16_t ustack[USER_STACK_SIZE];
static int8_t usp = 0;
static void (*i2c_scan_hook)(void) = 0;

static void ustack_push(int16_t n)
{
	if (usp >= USER_STACK_SIZE) {
		uart_puts("ERR: stack full\r\n");
		return;
	}
	ustack[usp++] = n;
}

static int16_t ustack_pop(unsigned char* ok)
{
	if (usp <= 0) {
		*ok = 0;
		uart_puts("ERR: stack empty\r\n");
		return 0;
	}
	*ok = 1;
	usp--;
	return ustack[usp];
}

static void ustack_clr(void)
{
	usp = 0;
}

static void uart_puthex16(uint16_t value)
{
	uart_puthex8((unsigned char)(value >> 8));
	uart_puthex8((unsigned char)(value & 0xFF));
}

static void usage(void)
{
	uart_puts("micro-mon: v0.2\r\n");
	uart_puts("  h|?          help\r\n");
	uart_puts("  i            scan i2c bus\r\n");
	uart_puts("  c            clear stack\r\n");
	uart_puts("  s            show stack\r\n");
	uart_puts("  +            add top two (n n -- n)\r\n");
	uart_puts("  d <n>        push decimal number\r\n");
	uart_puts("  x <hex>      push hex number (no 0x)\r\n");
}

static unsigned char parse_dec(const char* s, int16_t* out)
{
	int16_t n = 0;
	if (*s == '\0') {
		return 0;
	}
	while (*s) {
		if (*s < '0' || *s > '9') {
			return 0;
		}
		n = (int16_t)(n * 10 + (*s - '0'));
		s++;
	}
	*out = n;
	return 1;
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

static void print_stack(void)
{
	int8_t i;
	uart_puts("stack(");
	uart_puthex8((unsigned char)usp);
	uart_puts("): ");
	for (i = 0; i < usp; i++) {
		uart_puts("0x");
		uart_puthex16((uint16_t)ustack[i]);
		uart_putc(' ');
	}
	uart_puts("\r\n");
}

static void exec_line(char* line)
{
	char* cmd = line;
	char* arg = 0;
	int16_t n = 0;
	unsigned char ok = 0;

	while (*cmd == ' ') {
		cmd++;
	}
	if (*cmd == '\0') {
		return;
	}

	arg = cmd;
	while (*arg && *arg != ' ') {
		arg++;
	}
	if (*arg) {
		*arg++ = '\0';
		while (*arg == ' ') {
			arg++;
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

	if (cmd[0] == 'c' && cmd[1] == '\0') {
		ustack_clr();
		uart_puts("ok\r\n");
		return;
	}

	if (cmd[0] == 's' && cmd[1] == '\0') {
		print_stack();
		return;
	}

	if (cmd[0] == '+' && cmd[1] == '\0') {
		int16_t n1 = ustack_pop(&ok);
		int16_t n2;
		if (!ok) {
			return;
		}
		n2 = ustack_pop(&ok);
		if (!ok) {
			ustack_push(n1);
			return;
		}
		ustack_push((int16_t)(n1 + n2));
		print_stack();
		return;
	}

	if (cmd[0] == 'd' && cmd[1] == '\0') {
		if (!arg || !parse_dec(arg, &n)) {
			uart_puts("ERR: usage d <number>\r\n");
			return;
		}
		ustack_push(n);
		return;
	}

	if (cmd[0] == 'x' && cmd[1] == '\0') {
		if (!arg || !parse_hex(arg, &n)) {
			uart_puts("ERR: usage x <hex>\r\n");
			return;
		}
		ustack_push(n);
		return;
	}

	uart_puts("ERR: unknown command\r\n");
}

void cmd_set_i2c_scan_hook(void (*scan_fn)(void))
{
	i2c_scan_hook = scan_fn;
}

void cmd_init(void)
{
	line_len = 0;
	ustack_clr();
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
