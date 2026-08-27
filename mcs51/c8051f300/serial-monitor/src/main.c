/*
 * C8051F300 Serial Monitor
 */

#include <C8051F300.h>
#include "command.h"
#include "uart.h"

#define USER_LED P0_0

/* disable watchdog timer */

unsigned char _sdcc_external_startup(void)
{
	PCA0MD = 0;

	return 0;
}

/* simple delay */

void ms_delay(unsigned int ms)
{
	unsigned int i, j;

	for (i = 0; i < ms; i++)
		for (j = 0; j < 123; j++); // this needs to be longer
}

int main()
{
	/* enable crossbar */
	
	XBR2 = 0x40;

	/* enable serial and interrupts */

	uart_init(9600);

	EA = 1;

	/* loop */

	while (1) {

		if (cmd_flag)
			process_command();

		USER_LED ^= 1;

		ms_delay(1000);
	}
}
