/*
 * Bit bang 1200 baud UART TX for STC89 (8052 core)
 * Crystal: 11.0592 MHz crystal, default 12T mode
 * 8N1 framing: 1 start bit (low), 8 data bits LSB-first, 1 stop bit (high), idle high.
 *
 * If crystal or clock mode differs, recompute:
 *   cycles_per_bit = (crystal_hz / 12) / baud_rate      // 12T mode
 *   cycles_per_bit = (crystal_hz / 6)  / baud_rate      // 6T / double-speed mode (STC AUXR.T0x12=1)
 *   reload = 65536 - cycles_per_bit
 *   THx = reload >> 8;  TLx = reload & 0xFF;
 */

#include <mcs51/8052.h>
#include "softserial.h"

#define TX_PIN P1_2

#define BIT_RELOAD_H  0xFD
#define BIT_RELOAD_L  0x00

#define T0_RELOAD_VAL 64614

/* Blocks for exactly one bit period (833.33 us @ 11.0592 MHz, 12T mode) */

// todo: calibrate -- move to T2

static void delay_bit(void)
{
	/* stop timer, clear overflow */

	TR2 = 0;
	TF2 = 0;

	/* reload */

	TH2 = BIT_RELOAD_H;
	TL2 = BIT_RELOAD_L;

	/* run, wait for overflow, stop = one bit period elapsed */

	TR2 = 1;
	while (!TF2);            
	TR2 = 0;
}

void softserial_init(void)
{
	T2CON = 0;
	TX_PIN = 1;
}

/* Transmit one byte, 8N1, LSB first */

void softserial_tx(unsigned char b)
{
	unsigned char i;

	/* start bit */

	TX_PIN = 0;               
	delay_bit();

	/* data bits */

	for (i = 0; i < 8; i++) {

		TX_PIN = (__sbit) (b & 0x01);
		b >>= 1;
		delay_bit();
	}

	/* stop bit */

	TX_PIN = 1;               
	delay_bit();
}

void softserial_send_string(char *s)
{
	while (*s)
		softserial_tx((unsigned char) *s++);
}


void delay_ms(int ms)
{
        unsigned int i;

        /* Timer 0: Mode 1 */

        TMOD &= 0xF0; 
        TMOD |= 0x01;

        /* 1ms timer */

        for (i = 0; i < ms; i++) {

                TH0 = (T0_RELOAD_VAL >> 8) & 0xFF;
                TL0 = T0_RELOAD_VAL & 0xFF;

                TF0 = 0;
                TR0 = 1;

                while (!TF0);

                TR0 = 0;
        }
}

#if 1
void main(void)
{
	softserial_init();

	while (1) {

		softserial_send_string("TELEMETRY,123,45.6\r\n");
	
		delay_ms(1000);
	}

}
#endif
