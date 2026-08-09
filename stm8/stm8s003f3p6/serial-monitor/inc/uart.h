#ifndef UART_H
#define UART_H

void uart_init(unsigned long baud);
void uart_putc(char c);
void uart_puts(const char* s);
void uart_puthex8(unsigned char value);

#endif
