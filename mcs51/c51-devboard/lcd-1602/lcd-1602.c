/*
 * C51 Development Board 1602 LCD
 *
 * Pattern on 8 Port 2 LEDs
 *
 */

#include <mcs51/8051.h>

/* Button to read */

#define BUTTON P3_2

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
#define LCD_CMD_DISP_MODE 0x38
#define LCD_CMD_SET_POS 0x80

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
	
	__asm__("nop");	
}

/* make as one function */

void lcd_write_char(unsigned char c)
{	
	P2 = c;

	RS = LCD_CHAR;
	RW = 0;
	EN = 1;

	__asm__("nop");	

	EN = 0;
	P2 = 0xFF;
	
	__asm__("nop");	
}

void lcd_clear(void)
{
	lcd_write_command(LCD_CMD_CLEAR);
	
	ms_delay(5);

	lcd_write_command(LCD_CMD_ENTRY);
	
	ms_delay(5);
  
	lcd_write_command(LCD_CMD_SET_POS);
	
	ms_delay(5);
}

void lcd_init(void)
{
	ms_delay(25);

	/* 8-bit 2-line mode */

	lcd_write_command(LCD_CMD_DISP_MODE);
	
	ms_delay(5);

	lcd_write_command(LCD_CMD_DISP_OFF);
	
	ms_delay(5);

	lcd_clear();
}

void lcd_puts(char *s)
{
	while (*s != '\0') {

		lcd_write_char(*s);
		s++;
	}
}

int main()
{
	__sbit s2;

	lcd_init();

	lcd_puts("Hello World!");

	while (1) {

		s2 = !BUTTON;

		if (s2)
			P2_0 ^= 1;

		ms_delay(1000);
	}
}
