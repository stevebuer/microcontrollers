/*
 * 8051 program to read Dallas 1-Wire DS18B20
 *
 * Steve Buer
 * Olympic College
 * August 2026
 */

#include <mcs51/at89x52.h>
#include "system.h"
#include "command.h"
#include "uart.h"

void main()
{
	uart_init();

	EA = 1;

	while (1) {

		if (cmd_buf)
			process_command();	

	}
}
