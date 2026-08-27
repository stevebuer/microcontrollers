/*
 * Micro-Monitor: MCS-51 serial routines
 *
 * Steve Buer
 * Olympic College
 * July 2026
 *
 */

extern volatile __bit cmd_flag;
extern volatile char cmd_buf;

void uart_init(unsigned char baud);
