#include <msp430.h>

#define LED_PIN  BIT0 /* P1.0 - red LED on the MSP-EXP430G2 Launchpad */
#define LED2_PIN BIT6 /* P1.6 - green LED on the MSP-EXP430G2 Launchpad */
#define BTN_PIN  BIT3 /* P1.3 - Launchpad S2 button (active low) */

#define MODE_RATE1 0
#define MODE_RATE2 1
#define MODE_OFF   2
#define MODE_LED2  3

void main(void)
{
	unsigned char mode = MODE_RATE1;
	unsigned char last_button = 1;

	WDTCTL = WDTPW | WDTHOLD; /* stop watchdog timer */

	P1DIR |= (LED_PIN | LED2_PIN); /* LED outputs */
	P1OUT &= ~(LED_PIN | LED2_PIN); /* all LEDs off */

	P1DIR &= ~BTN_PIN;          /* button input */
	P1REN |= BTN_PIN;           /* enable pull-up/down resistor */
	P1OUT |= BTN_PIN;           /* pull-up, button reads 1 when released */

	while (1) {
		unsigned char button_now = (P1IN & BTN_PIN) ? 1 : 0;

		if (button_now == 0 && last_button == 1) {
			mode = (mode + 1) % 4;
			__delay_cycles(50000); /* debounce */
			while ((P1IN & BTN_PIN) == 0) {
				__delay_cycles(1000);
			}
		}
		last_button = button_now;

		switch (mode) {
		case MODE_RATE1:
			P1OUT |= LED_PIN;
			P1OUT &= ~LED2_PIN;
			__delay_cycles(1000000);
			P1OUT &= ~LED_PIN;
			__delay_cycles(1000000);
			break;

		case MODE_RATE2:
			P1OUT |= LED_PIN;
			P1OUT &= ~LED2_PIN;
			__delay_cycles(500000);
			P1OUT &= ~LED_PIN;
			__delay_cycles(500000);
			break;

		case MODE_OFF:
			P1OUT &= ~(LED_PIN | LED2_PIN);
			__delay_cycles(20000);
			break;

		case MODE_LED2:
		default:
			P1OUT &= ~LED_PIN;
			P1OUT |= LED2_PIN;
			__delay_cycles(20000);
			break;
		}
	}
}

