/*
 * BLink Test LED on STM8S003F3P6 eBay development board
 *
 * compile: sdcc -mstm8 led-test.c
 *
 * Steve Buer
 * July 2025
 */

/* LED is on Port B pin 5 */

#define PB_ODR *(volatile unsigned char *) 0x5005
#define PB_IDR *(volatile unsigned char *) 0x5006
// #define PB_DDR *(volatile unsigned char *) 0x5007
#define PB_CR1 *(volatile unsigned char *) 0x5008
#define PB_CR2 *(volatile unsigned char *) 0x5009

#define PIN5 (1 << 5)

unsigned char testvar;

/* create volatile pointers and assign register locations as their values */

volatile unsigned char * PB_ODR = 0x5005;
volatile unsigned char * PB_DDR = 0x5007;

/* timer interrupt handler */

void timer_interrupt(void) __interrupt(20)
{
	testvar++;
}

/* main */

int main() 
{
	*PB_DDR = 0b00101010; // put this value in location pointed to by PB_DDR

	while(1)
		__asm__("nop");
}
