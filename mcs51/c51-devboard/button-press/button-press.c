/*
 * C51 Development Board Button Example
 *
 * Press Button: Changes Blinking LED
 *
 */

#include <mcs51/8051.h>

#define F_OSC 11059200UL
#define TICKS_PER_MS (F_OSC / 12 / 1000)
#define RELOAD_VAL (65536 - TICKS_PER_MS)

/* user buttons are labeled S2 - S5 right to left */

#define S2 P3_2
#define S3 P3_3
#define S4 P3_4
#define S5 P3_5

void ms_delay(int ms)
{
	unsigned int i;

	/* Timer 0: Mode 1 */

	TMOD &= 0xF0; 
	TMOD |= 0x01;

	/* 1ms timer */

	for (i = 0; i < ms; i++) {

		TH0 = (RELOAD_VAL >> 8) & 0xFF;
		TL0 = RELOAD_VAL & 0xFF;

		TF0 = 0;
		TR0 = 1;

		while (!TF0);

		TR0 = 0;
	}
}

int main()
{
	while (1) {

		P2_0 ^= 1;

		ms_delay(1000);
	}
}
