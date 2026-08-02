/* Bare-metal blink for STM32F407 Discovery
 * LEDs: PD12 (green), PD13 (orange), PD14 (red), PD15 (blue)
 */

#include <stdint.h>

#define RCC_AHB1ENR   (*(volatile unsigned int *) 0x40023830)
#define GPIOD_MODER   (*(volatile unsigned int *) 0x40020C00)
#define GPIOD_ODR     (*(volatile unsigned int *) 0x40020C14)

static void delay_1s(void)
{
	/* 4 million iterations * 4 cycles per loop = 16,000,000 cycles */

	volatile uint32_t count = 4000000; 

	while(count--)
		__asm("nop");
}

int main(void)
{
	/* Enable GPIOD clock (bit 3 of AHB1ENR) */

	RCC_AHB1ENR |= (1 << 3);

	/* Set PD12-PD15 as general purpose output (01 in MODER for each pin) */

	GPIOD_MODER |= (0x55u << 24);

	while (1) {

		/* toggle all four LEDs together */

		GPIOD_ODR ^= (0xF << 12);   

		/* default clock speed: 16 Mhz */
		
		delay_1s();
	}
}
