#include <msp430.h>
#include "uart.h"

static void uart_wait_tx_ready(void)
{
	while (!(IFG2 & UCA0TXIFG))
		;
}

void uart_init(void)
{
	P1SEL |= BIT1 | BIT2;     /* UCA0 TXD/TXD on P1.1/P1.2 */
	P1SEL2 |= BIT1 | BIT2;

	UCA0CTL1 |= UCSSEL_2;     /* SMCLK */
	UCA0BR0 = 104;            /* 1MHz / 9600 = 104 */
	UCA0BR1 = 0;
	UCA0MCTL = UCBRS0;        /* modulation UCBRSx = 1 */
	UCA0CTL1 &= ~UCSWRST;     /* start UART */
}

void uart_putc(char c)
{
	uart_wait_tx_ready();
	UCA0TXBUF = c;
}

void uart_puts(const char *s)
{
	while (*s) {
		uart_putc(*s++);
	}
}

void uart_puthex8(uint8_t value)
{
	static const char digits[] = "0123456789ABCDEF";
	uart_putc(digits[(value >> 4) & 0xF]);
	uart_putc(digits[value & 0xF]);
}

unsigned char uart_rx_ready(void)
{
	return (IFG2 & UCA0RXIFG) ? 1 : 0;
}

char uart_getc(void)
{
	while (!(IFG2 & UCA0RXIFG))
		;
	return UCA0RXBUF;
}
