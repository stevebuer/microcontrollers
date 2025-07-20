/*
 * Blink Test LED faster/simpler on STM8S003F3P6 eBay development board
 *
 * compile: sdcc -mstm8 blink2.c
 *
 * Steve Buer
 * July 2025
 */

/* LED is on Port B pin 5 */

#define PIN5 (1 << 5)

/* pointers to port B registers */

volatile unsigned char * PORTB_ODR = 0x5005;
volatile unsigned char * PORTB_DDR = 0x5007;
volatile unsigned char * PORTB_CR1 = 0x5008;

int main() 
{
	/* Port B data direction register (DDR) pin 5 output */

	*PORTB_DDR = PIN5;

	/* Port B configuration register (CR1) pin 5 push-pull */ 
	
	*PORTB_CR1 = PIN5;

	while (1) {

		/* Port B output data register (ODR) toggle */

		*PORTB_ODR ^= PIN5; 
	
		for (unsigned long n = 100000L; n > 0; n--)
			__asm__("nop");
	}
}
