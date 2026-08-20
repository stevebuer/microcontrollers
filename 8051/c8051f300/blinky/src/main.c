/*
 * C8051F300 Blinky
 */

#include <C8051F300.h>

volatile unsigned long int systick;

void t0_handler(void) __interrupt(1)
{
	TH0 = (65536 - 125) / 256;
	TL0 = (65536 - 125) % 256;
	systick++;
}

void ms_delay(unsigned int ms)
{
	unsigned int i, j;

	for (i = 0; i < ms; i++)
		for (j = 0; j < 123; j++);
}

int led_toggle()
{
	return 0;
}

int main()
{
	/* Port 2 Mode Output */

	/* enable interrupts */

	EA = 1;

	/* blink */

	while (1) {

		led_toggle();

		ms_delay(1000);
	}
}
