/*
 * 8051 UART
 *
 * Steve Buer
 * Olympic College
 * August 2026
 *
 */

#include <mcs51/at89x52.h>
#include "system.h"
#include "uart.h"

/* command buffer and flag */

volatile char cmd_buf = 0;

/* RX and TX share interrupt 4 */

void uart_ISR(void) __interrupt(4) 
{
	if (RI) {

		/* store received byte in command buffer */

		cmd_buf = SBUF;        
		
		/* clear RX flag */

		RI = 0;
	}
}

/* initialize */

void uart_init()
{
	/* configure T1 for mode 2 */

	TMOD &= 0x0F;     
	TMOD |= 0x20;      

	/* TH1 holds reload value for 9600 baud @ 11.0592 Mhz, load TL1 initial value */
	
	TH1 = 0xFD;
	TL1 = 0xFD;

	/* set uart for serial mode 1 and REN = 1 (0b0101) */

	SCON = 0x50;

	/* start T1 */
    
	TR1 = 1; 

	/* TI must be set for initial state or putchar() hangs */

	TI = 1;

	/* enable uart interrupt */

	ES = 1;	
}

/* putchar() must be implemented for sdcc libc stdio */

int putchar(int c)
{
	/* wait until TI = 1, previous byte send completed */

	while (!TI);

	/* clear TI */

	TI = 0;

	/* move char to tx buf */

	SBUF = c;

	/* return transmitted char */

	return c;
}
