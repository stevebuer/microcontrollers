#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *s);
void uart_puthex8(uint8_t value);
unsigned char uart_rx_ready(void);
char uart_getc(void);

#endif
