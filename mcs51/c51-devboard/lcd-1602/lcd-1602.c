/*
 * C51 Development Board 1602 LCD
 *
 * Print a 2-line message and toggle LED via button S2
 *
 */

#include <mcs51/8051.h>

/* Button to read */

#define BUTTON P3_2

__sbit s2;

/* LCD control line pins */

#define RS P1_2
#define RW P1_1
#define EN P1_0

/* LCD RS line */

#define LCD_CMD 0x0
#define LCD_CHAR 0x1

/* LCD command bytes */

#define LCD_CMD_CLEAR 0x01
#define LCD_CMD_ENTRY 0x06
#define LCD_CMD_DISP_OFF 0x08
#define LCD_CMD_DISP_ON 0x0C
#define LCD_CMD_8BIT_1LINE 0x30
#define LCD_CMD_8BIT_2LINES 0x38
#define LCD_CMD_SET_POS 0x80

/* DDRAM start address for rows (2-row mode) */

#define LCD_ROW1_ADDR 0x00
#define LCD_ROW2_ADDR 0x40

/* Timer constants */

#define F_OSC 11059200UL
#define TICKS_PER_MS (F_OSC / 12 / 1000)
#define RELOAD_VAL (65536 - TICKS_PER_MS)

void ms_delay(int ms)
{
	unsigned int i;

	/* Timer 0: Mode 1 */

	TMOD &= 0xF0; 
	TMOD |= 0x01;

	/* 1ms timer */

	for (i = 0; i < ms; i++) {

		TH0 = (RELOAD_VAL >> 8) & 0xFF;
		TL0 = RELOAD_VAL & 0xFF;

		TF0 = 0;
		TR0 = 1;

		while (!TF0);

		TR0 = 0;
	}
}

void lcd_write_command(unsigned char byte)
{
	P2 = byte;

	RS = LCD_CMD;
	RW = 0;
	EN = 1;

	__asm__("nop");

	EN = 0;
	P2 = 0xFF;
	
	ms_delay(1);
}

void lcd_write_char(unsigned char c)
{	
	P2 = c;

	RS = LCD_CHAR;
	RW = 0;
	EN = 1;

	__asm__("nop");

	EN = 0;
	P2 = 0xFF;

	ms_delay(2);
}

void lcd_clear(void)
{
	lcd_write_command(LCD_CMD_CLEAR);
	
	lcd_write_command(LCD_CMD_ENTRY);
	
	lcd_write_command(LCD_CMD_SET_POS);
}

void lcd_init(void)
{
	/* power-on wait */

	ms_delay(50);

	/* HD44780 "initialization by instruction" (no busy-flag check yet) */

	lcd_write_command(LCD_CMD_8BIT_1LINE);

	ms_delay(5);

	lcd_write_command(LCD_CMD_8BIT_1LINE);

	lcd_write_command(LCD_CMD_8BIT_1LINE);

	/* 8-bit 2-line mode - must follow the sequence above to latch reliably */

	lcd_write_command(LCD_CMD_8BIT_2LINES);
	
	lcd_write_command(LCD_CMD_DISP_OFF);
	
	lcd_clear();

	lcd_write_command(LCD_CMD_DISP_ON);
}

void lcd_puts(char *s)
{
	while (*s != '\0') {

		lcd_write_char(*s);
		s++;
	}
}

void lcd_position(unsigned char row, unsigned char column)
{
	unsigned char addr;

	addr = (row == 1) ? LCD_ROW1_ADDR : LCD_ROW2_ADDR;

	addr += column - 1;

	lcd_write_command(LCD_CMD_SET_POS | addr);
}

int main()
{
	lcd_init();

	lcd_puts("Hello World!");

	lcd_position(2, 1);
	
	lcd_write_char('X'); lcd_write_char('Y'); lcd_write_char('Z');	

	while (1) {

		s2 = !BUTTON;

		if (s2)
			P2_0 ^= 1;

		ms_delay(1000);
	}
}
