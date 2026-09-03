/*
 * Dallas 1-wire routines
 *
 * Steve Buer, N7MKO
 * Olympic College
 *
 */

#include <mcs51/8052.h>
#include <stdint.h>
#include <stdio.h>
#include "onewire.h"

/* 1-wire command bytes */

#define CMD_READ_ROM 0x33

/* todo: choose pin on init */

void ow_init(void)
{
    	DQ = 1;
}

/* 1 nop = ~1.085 us @ 11.0592 Mhz */

#define OW_NOP   __asm nop __endasm;
#define OW_NOP2  OW_NOP  OW_NOP
#define OW_NOP4  OW_NOP2 OW_NOP2
#define OW_NOP8  OW_NOP4 OW_NOP4
#define OW_NOP16 OW_NOP8 OW_NOP8
#define OW_NOP32 OW_NOP16 OW_NOP16

#define OW_DELAY_2US  do { OW_NOP2                          } while (0)
#define OW_DELAY_4US  do { OW_NOP4                          } while (0)
#define OW_DELAY_6US  do { OW_NOP4 OW_NOP2                  } while (0)
#define OW_DELAY_9US  do { OW_NOP8 OW_NOP                   } while (0)
#define OW_DELAY_50US do { OW_NOP32 OW_NOP16 OW_NOP2        } while (0)
#define OW_DELAY_60US do { OW_NOP32 OW_NOP16 OW_NOP8 OW_NOP4} while (0)
#define OW_DELAY_64US do { OW_NOP32 OW_NOP32                } while (0)

/* presence delays */

#define OW_RESET_LOOPS   70   /* bus low ~480-960us, target ~600us */
#define OW_WAIT_LOOPS     5   /* wait before sampling presence, ~15-60us */
#define OW_FINISH_LOOPS  47   /* finish out the reset/presence slot, ~400us */

static unsigned char ow_reset(void)
{
	unsigned char presence;
	unsigned int i;

	/* hold bus low for reset pulse (480-960us) */

	DQ = 0;

	for (i = 0; i < OW_RESET_LOOPS; i++)
		__asm__("nop");

	DQ = 1;

	/* wait for device to pull low (presence pulse starts within 15-60us) */

	for (i = 0; i < OW_WAIT_LOOPS; i++)
		__asm__("nop");

	/* sample presence: low = device present, high = no device */

	presence = DQ;

	/* finish the 480us+ reset/presence slot */

	for (i = 0; i < OW_FINISH_LOOPS; i++)
		__asm__("nop");

	return (presence == 0) ? 1 : 0;
}

/* write a single bit */

void ow_write_bit(unsigned char bit)
{
	/* start slot: low */

	DQ = 0;              

	if (bit) {

		/* send 1: short low, long high */

		OW_DELAY_6US;
		DQ = 1;
		OW_DELAY_64US;

	} else {

		/* send 0: hold full slot low wth 4us recovery */

		OW_DELAY_60US;
		DQ = 1;
		OW_DELAY_4US;
	}
}

/* read a single bit */

unsigned char ow_read_bit(void)
{
	unsigned char bit;

	/* start slot */

	DQ = 0;
	OW_DELAY_2US;
	DQ = 1;
	
	/* wait before sample point (~13-15us from slot start) */

	OW_DELAY_9US;

	/* read sample */

	bit = DQ;
	
	/* wait for slot end (~60us total) */

	OW_DELAY_50US;

	return bit;
}

/* write a byte, LSB first */

void ow_write_byte(unsigned char data)
{
	unsigned char i;

	for (i = 0; i < 8; i++) {

		ow_write_bit(data & 0x01);
		data >>= 1;
	}
}

/* read a byte, LSB first */

unsigned char ow_read_byte(void)
{
	unsigned char i, data = 0;

	for (i = 0; i < 8; i++) {

		data >>= 1;

		if (ow_read_bit())
			data |= 0x80;
	}

	return data;
}

/* read 64-bit ROM id: returns 1 on success, 0 if no device present */

unsigned char ow_read_rom(uint8_t *rom)
{
	uint8_t i;

	/* no device present */

	if (!ow_reset())
		return 0;

	/* Read ROM */

	ow_write_byte(CMD_READ_ROM);  

	for (i = 0; i < 8; i++)
		rom[i] = ow_read_byte();

	return 1;
}

/* our bus only has a single device for now */

void ow_scanbus(void)
{
	uint8_t i, rom[8];

	if (!ow_read_rom(rom)) {

		puts("no presence pulse detected");
		return;
	}

	for (i = 0; i < 8; i++) {

		/* print two hex digits per byte */

		putchar("0123456789ABCDEF"[rom[i] >> 4]);
		putchar("0123456789ABCDEF"[rom[i] & 0x0F]);
	}

	putchar('\n');
}
