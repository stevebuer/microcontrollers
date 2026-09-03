/*
 * onewire.h
 */

/* 1-wire bus pin */

#define DQ P1_0

/* 1-wire api */

void ow_init(void);

void ow_scanbus(void);

void ow_write_byte(unsigned char);

unsigned char ow_read_byte(void);

unsigned char ow_read_rom(uint8_t *);
