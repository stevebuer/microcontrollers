/*
 * 8051 UART
 *
 * Steve Buer
 * Olympic College
 * August 2026
 */

#define BAUD_RATE 9600

extern volatile char cmd_buf;

void uart_init(void);
