#include <msp430.h>
#include "uart.h"
#include "command.h"

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	uart_init();
	cmd_init();

	for (;;) {
		cmd_poll();
	}

	return 0;
}
