/*
 * Dallas 1-wire Test program
 *
 * Steve Buer, N7MKO
 * Olympic College
 *
 */

#include <mcs51/8052.h>
#include <stdint.h>
#include <stdio.h>
#include "onewire.h"

/* millis timer constants */

#define F_OSC 11059200UL
#define TICKS_PER_MS (F_OSC / 12 / 1000)
#define RELOAD_VAL (65536 - TICKS_PER_MS)

/* 9600 baud uart */

#define T1_RELOAD_VAL 0xFD

/* dev board buttons */

#define BUTTON1 P3_2
#define BUTTON2 P3_3

__sbit s2;
__sbit s3;

/* uart */

void uart_init(void)
{
        TMOD &= 0x0F;     
        TMOD |= 0x20;      

        TH1 = T1_RELOAD_VAL;
        TL1 = T1_RELOAD_VAL;

	/* tx only */

        SCON = 0x40;

        TR1 = 1; 

        TI = 1;
}

int putchar(int c)
{
	/* add cr */

	if (c == '\n') {
		
		while (!TI);

		TI = 0;

		SBUF = '\r';
	}

	/* send c */

        while (!TI);

        TI = 0;

        SBUF = c;

        return c;
}

/* millis */

void ms_delay(int ms)
{
	unsigned int i;

	/* timer 0: mode 1 */

	TMOD &= 0xF0; 
	TMOD |= 0x01;

	/* 1 ms timer loop */

	for (i = 0; i < ms; i++) {

		TH0 = (RELOAD_VAL >> 8) & 0xFF;
		TL0 = RELOAD_VAL & 0xFF;

		TF0 = 0;
		TR0 = 1;

		while (!TF0);

		TR0 = 0;
	}
}

/* main */

int main(void)
{
	uart_init();

	puts("ok");

	while (1) {

		s2 = !BUTTON1;
		s3 = !BUTTON2;

		if (s2) {

			P2_0 ^= 1;

			puts("s2");
		}

		if (s3)
			ow_scanbus();

		ms_delay(500);
	}
}
