#include "stm8s_conf.h"
#include "uart.h"

#define UART_RX_BUFFER_SIZE 64U
#define UART_RX_BUFFER_MASK (UART_RX_BUFFER_SIZE - 1U)

static volatile char uart_rx_buffer[UART_RX_BUFFER_SIZE];
static volatile uint8_t uart_rx_head;
static volatile uint8_t uart_rx_tail;

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

	uart_rx_head = 0;
	uart_rx_tail = 0;
	UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
}

void uart_rx_isr(void)
{
	uint8_t next = (uart_rx_head + 1U) & UART_RX_BUFFER_MASK;
	char received = (char)UART2_ReceiveData8();

	if (next != uart_rx_tail) {
		uart_rx_buffer[uart_rx_head] = received;
		uart_rx_head = next;
	}
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
	return uart_rx_head != uart_rx_tail;
}

char uart_getc(void)
{
	char received = uart_rx_buffer[uart_rx_tail];

	uart_rx_tail = (uart_rx_tail + 1U) & UART_RX_BUFFER_MASK;
	return received;
}
