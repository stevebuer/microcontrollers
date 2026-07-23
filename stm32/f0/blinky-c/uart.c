/* F0 serial routines */

#include "stm32f0xx.h"

void uart_init(void)
{
	/* Enable USART1 clock -- RCC_APB2ENR bit 14 */

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	/* BRR = fCK / baud = 8,000,000 / 9600 (default 16x oversampling) */

	USART1->BRR = 8000000UL / 9600UL;

	/* Enable USART, transmitter -- UE + TE */

	USART1->CR1 = USART_CR1_UE | USART_CR1_TE;
}

/* Wait for TXE, then write */

static void uart_putchar(char c)
{
	while (!(USART1->ISR & USART_ISR_TXE)) { }

	USART1->TDR = (uint8_t) c;
}

void uart_puts(const char *s)
{
	while (*s)
		uart_putchar(*s++);
}

