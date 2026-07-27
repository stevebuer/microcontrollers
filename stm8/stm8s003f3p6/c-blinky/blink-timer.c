/*
 * Blink Test LED via Timer4 on STM8S003F3P6 eBay development board
 *
 * compile: sdcc -mstm8 timer.c
 *
 * Steve Buer
 * July 2025
 */

/* default clock rate is 2 Mhz on restart */

#define CLOCK_RATE 2000000

/* LED is on Port B pin 5 */

#define PIN5 (1 << 5)
#define TIM4_ISR 23

/* pointers to port B registers */

volatile unsigned char * PORTB_ODR = 0x5005;
volatile unsigned char * PORTB_DDR = 0x5007;
volatile unsigned char * PORTB_CR1 = 0x5008;

/* pointers to Timer 4 registers */

volatile unsigned char * TIM4_CR1 = 0x5340;
volatile unsigned char * TIM4_IER = 0x5343;
volatile unsigned char * TIM4_SR = 0x5344;
volatile unsigned char * TIM4_PSCR = 0x5347;
volatile unsigned char * TIM4_ARR = 0x5348;

/* timer interrupt handler */

void timer_interrupt(void) __interrupt(TIM4_ISR)
{
	*PORTB_ODR ^= PIN5;
	TIM4_SR &= ~(1 << UIF);
}

int main() 
{
	/* Port B data direction register (DDR) pin 5 output */

	*PORTB_DDR = PIN5;

	/* Port B configuration register (CR1) pin 5 push-pull */ 
	
	*PORTB_CR1 = PIN5;

	/* timer 4 prescaler set to 128 (max) */

	*TIM4_PSCR = 0x7;

	/* set timer 4 auto-reload register (reset status: FF) */

	/* compiler echo? or store string? : f_io = f_clk / [2 * prescaler * (1 + ARR)] todo: locate in data sheet! */

	// todo

	/* configure timer 4 interrupt enable register */

	TIM4_IER |= (1 << TODO);

	/* enable timer4 via control register */

	TIM4_CR1 |= (1 << TODO);

	/* enable interrupts */

	__asm__("rim");

	while (1);
}
