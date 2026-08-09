#include "onewire.h"
#include "uart.h"

void onewire_init(void)
{
	/* Placeholder for next session: GPIO/timing setup for Dallas 1-Wire. */
}

void onewire_handle_command(char* args)
{
	if (!args || *args == '\0') {
		uart_puts("1W placeholder commands:\r\n");
		uart_puts("  ow help\r\n");
		uart_puts("  ow scan\r\n");
		uart_puts("  ow readrom\r\n");
		uart_puts("  ow reset\r\n");
		uart_puts("(not implemented yet)\r\n");
		return;
	}

	uart_puts("ERR: 1W command not implemented yet: ");
	uart_puts(args);
	uart_puts("\r\n");
}