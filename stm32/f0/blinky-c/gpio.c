/* GPIO routines */

#include "stm32f0xx.h"

#define LED_PIN   4

void gpio_init(void)
{
	/* Enable GPIOA clock : RCC_AHBENR bit 17 */

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	/* LED pin: PA4 as general-purpose output (MODER = 01) */

	GPIOA->MODER &= ~(3U << (LED_PIN * 2));
	GPIOA->MODER |=  (1U << (LED_PIN * 2));

	/* USART1 TX pin: PA9 as alternate function (MODER = 10) */

	GPIOA->MODER &= ~(3U << (9 * 2));
	GPIOA->MODER |=  (2U << (9 * 2));

	/* rebase AFRH: pin 9 -> AF1 = USART1 (bits [7:4], since pin 9 is index 1 in AFRH) */

	GPIOA->AFR[1] &= ~(0xFU << ((9 - 8) * 4));
	GPIOA->AFR[1] |=  (1U   << ((9 - 8) * 4));
}
