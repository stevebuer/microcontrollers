/*
 * Morse code routines for various microcontrollers
 *
 * Steve Buer, N7MKO
 *
 */

#include <stdint.h>
#include <stdio.h>

#define DEBUG 1

#define NUM_LEN 5 /* numerals are fixed code length */

/* dot: 0, dash: 1 */

uint8_t morse_digits[10] = { 0b11111, 0b11110, 0b11100, 0b11000, 0b10000, 0b11111, 0b00001, 0b00011, 0b00111, 0b01111 }; 

/* for letters, store length in upper 3 bits -- encoding in lower 4 */

uint8_t morse_letters[26] = {

	0b01000010,
	0b10000001,
	0b10000101
};

/* send a digit */

void send_digit(int len, uint8_t code)
{
	for (int i = 0; i < len; i++) {

		if (code & 0x1)
			printf("dah ");
		else
			printf("dit ");
		
		code = code >> 1;
	}
}

/* send a string of characters */

void morse_sendmsg(char *msg)
{
	do {
		if (*msg >= 48 && *msg <= 57)
			printf("sending digit: %c\n", *msg);
		else if (*msg == 32)
			printf("sending space\n");
		else
			printf("sending letter: %c\n", *msg);

	} while (*++msg != '\0');
}

#if DEBUG

int main()
{

	printf("sending test string: 'vvv de n7mko'\n");

	morse_sendmsg("vvv de n7mko");
	
	goto done;

	printf("testing number patterns...\n");
	printf("0: "); send_digit(NUM_LEN, morse_digits[0]); putchar('\n');
	printf("1: "); send_digit(NUM_LEN, morse_digits[1]); putchar('\n');
	printf("2: "); send_digit(NUM_LEN, morse_digits[2]); putchar('\n');
	printf("3: "); send_digit(NUM_LEN, morse_digits[3]); putchar('\n');
	printf("4: "); send_digit(NUM_LEN, morse_digits[4]); putchar('\n');
	printf("5: "); send_digit(NUM_LEN, morse_digits[5]); putchar('\n');
	printf("6: "); send_digit(NUM_LEN, morse_digits[6]); putchar('\n');
	printf("7: "); send_digit(NUM_LEN, morse_digits[7]); putchar('\n');
	printf("8: "); send_digit(NUM_LEN, morse_digits[8]); putchar('\n');
	printf("9: "); send_digit(NUM_LEN, morse_digits[9]); putchar('\n');

	done:

	return 0;
}

#endif
