#include "stm8s_conf.h"
#include "uart.h"

void uart_init(unsigned long baud)
{
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART2, ENABLE);
	UART2_DeInit();

	UART2_Init(baud,
		UART2_WORDLENGTH_8D,
		UART2_STOPBITS_1,
		UART2_PARITY_NO,
		UART2_SYNCMODE_CLOCK_DISABLE,
		UART2_MODE_TXRX_ENABLE);
}

void uart_putc(char c)
{
	UART2_SendData8((uint8_t)c);
	while (UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET) {
	}
}

void uart_puts(const char* s)
{
	while (*s) {
		uart_putc(*s++);
	}
}

void uart_puthex8(unsigned char value)
{
	const char hex[] = "0123456789ABCDEF";
	uart_putc(hex[(value >> 4) & 0x0F]);
	uart_putc(hex[value & 0x0F]);
}

unsigned char uart_rx_ready(void)
{
	return (UART2_GetFlagStatus(UART2_FLAG_RXNE) != RESET);
}

char uart_getc(void)
{
	return (char)UART2_ReceiveData8();
}
