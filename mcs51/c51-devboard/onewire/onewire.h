/*
 * MCS-51 Dallas 1-Wire Routines
 *
 * Steve Buer
 * Olympic College
 * September 2026
 *
 */

/* 1-wire bus pin */

#define DQ P1_0

/* 1-wire command bytes */

#define ONEWIRE_READ_ROM 0x33
#define ONEWIRE_SKIP_ROM 0xCC

/* 1-wire api */

void ow_init(void);
__sbit ow_reset(void);
void ow_scanbus(void);
void ow_write_byte(unsigned char);
unsigned char ow_read_byte(void);
unsigned char ow_read_rom(uint8_t *);
