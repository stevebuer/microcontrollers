/*
 * C51 Development Board Blinky
 *
 * Pattern on 8 Port 2 LEDs
 *
 */

#include <mcs51/8051.h>

#define F_OSC 11059200UL
#define TICKS_PER_MS (F_OSC / 12 / 1000)
#define RELOAD_VAL (65536 - TICKS_PER_MS)

unsigned char led_patterns[] = {

	0b01111110,
	0b10111101,
	0b11011011,
	0b11100111,
	0b11011011,
	0b10111101,
};

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
	unsigned char i = 0;

	while (1) {

		P2 = led_patterns[i];

		ms_delay(1000);

		if (i == 5)
			i = 0;
		else
			i++;
	}
}
