/*
 * C51 Development Board UART Program
 *
 * Steve Buer
 * Olympic College
 * August 2026
 *
 */

#include <mcs51/8051.h>
#include <stdio.h>

/* Timer values */

#define F_OSC 11059200UL
#define TICKS_PER_MS (F_OSC / 12 / 1000)
#define T0_RELOAD_VAL (65536 - TICKS_PER_MS)
#define T1_RELOAD_VAL 0xFD

/* UART */

volatile unsigned char rx_buf;

void uart_ISR(void) __interrupt(4) 
{
	if (RI) {

		/* copy received byte */

		rx_buf = SBUF;        

		/* clear flag */
		
		RI = 0;
	}
}

void ms_delay(int ms)
{
	unsigned int i;

	/* Timer 0: Mode 1 */

	TMOD &= 0xF0; 
	TMOD |= 0x01;

	/* 1ms timer */

	for (i = 0; i < ms; i++) {

		TH0 = (T0_RELOAD_VAL >> 8) & 0xFF;
		TL0 = T0_RELOAD_VAL & 0xFF;

		TF0 = 0;
		TR0 = 1;

		while (!TF0);

		TR0 = 0;
	}
}

/* use sdcc libc stdio */

int putchar(int c)
{
	/* wait for tx complete */

	while (!TI);

	/* clear tx complete flag */

	TI = 0;

	/* mov to send buf */

	SBUF = c;

	return c;
}

void uart_init(void)
{
	/* T1 mode 2 */

	TMOD &= 0x0F;     
	TMOD |= 0x20;      

	/* TH1 holds reload value, TL1 needs initial value */
	
	TH1 = T1_RELOAD_VAL;
	TL1 = T1_RELOAD_VAL;

	/* UART mode 1 & receive enable */

	SCON = 0x50;

	/* T1 run */
    
	TR1 = 1; 

	/* TI must be set for initial state or putchar() hangs */

	TI = 1;

	/* UART interrupt enable */

	ES = 1;	
}

int main()
{
	__sbit pressed = 0;

	uart_init();

	puts("MCS51 Monitor\n");

	EA = 1; 

	while (1) {

		// scan button

		if (pressed)
			puts("Button");

		ms_delay(100);

	}
}
