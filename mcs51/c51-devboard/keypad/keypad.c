/*
 * MCS51 Keypad Scanner
 */

#include <mcs51/8051.h>

#define R1 P1_0
#define R2 P1_1
#define R3 P1_2
#define R4 P1_3
#define C1 P1_4
#define C2 P1_5
#define C3 P1_6

#define LED P2_0

unsigned char keymap[4][3] = {

	{'1','2','3'},
	{'4','5','6'},
	{'7','8','9'},
	{'*','0','#'}
};

void delay_ms(unsigned int ms) 
{
	unsigned int i, j;

	for (i = 0; i < ms; i++)
		for (j = 0; j < 120; j++);
}

unsigned char scan_keypad(void) 
{
	unsigned char row, col;

	for (row = 0; row < 4; row++) {

		/* set all rows high, then pull the active row low */

        	R1 = R2 = R3 = R4 = 1;

        	switch (row) {

			case 0: R1 = 0; break;
			case 1: R2 = 0; break;
			case 2: R3 = 0; break;
			case 3: R4 = 0; break;
        	}

        	if (C1 == 0)
			col = 0;
        	else if (C2 == 0)
			col = 1;
        	else if (C3 == 0)
			col = 2;
        	else continue;
	
		/* debounce */

        	delay_ms(15);        

        	if ((col == 0 && C1 == 0) || (col == 1 && C2 == 0) || (col == 2 && C3 == 0)) {
			
			/* wait for release */

            		while (C1 == 0 || C2 == 0 || C3 == 0); 
            
			return keymap[row][col];
        	}
	}

	/* no key pressed */

	return 0xFF;  
}

/* UART output */

void uart_init(void)
{
        TMOD &= 0x0F;     
        TMOD |= 0x20;      

	/* 9600 baud */

        TH1 = 0xFD;
        TL1 = 0xFD;

	/* TX only */

        SCON = 0x40;

        TR1 = 1; 

        TI = 1;
}

int putchar(int c)
{
        while (!TI);

        TI = 0;

        SBUF = c;

        return c;
}

void main(void) 
{
	unsigned char key;

	uart_init();

	P1 = 0xFF;

	while (1) {

		key = scan_keypad();

		if (key != 0xFF) {

			LED = 0;

			delay_ms(250);

			LED = 1;
			
			putchar(key);
		}
    	}
}
