/*
 * Morse code routines for various microcontrollers
 *
 * Steve Buer, N7MKO
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> /* atoi -- write macro */

#define DEBUG 1

#define DIGITS 10
#define DIGIT_LEN 5 /* numerals are fixed code length */
#define ALPHABET 26
#define ABASE 97 /* alphabet zero index offset: ascii 'a' == 97 */

/* dot: 0, dash: 1 */

uint8_t morse_digits[DIGITS] = { 0b11111, 0b11110, 0b11100, 0b11000, 0b10000, 0b11111, 0b00001, 0b00011, 0b00111, 0b01111 }; 

/* for letters, store length in upper 3 bits -- encoding in lower 4 */

uint8_t morse_letters[ALPHABET] = {

	(0x2 << 5) | 0b10,	/* begin with letter 'a' */
	(0x4 << 5) | 0b0001,
	(0x4 << 5) | 0b0101,
	(0x3 << 5) | 0b001,
	(0x1 << 5) | 0b0,
	(0x4 << 5) | 0b0100,
	(0x3 << 5) | 0b0011,
	(0x4 << 5) | 0b0000,
	(0x2 << 5) | 0b00,	/* i */
	(0x4 << 5) | 0b1110,
	(0x3 << 5) | 0b101,
	(0x4 << 5) | 0b0010,
	(0x2 << 5) | 0b11,
	(0x2 << 5) | 0b01,
	(0x3 << 5) | 0b111,	/* o */
	(0x4 << 5) | 0b0110,
	(0x4 << 5) | 0b1011,
	(0x3 << 5) | 0b101,
	(0x3 << 5) | 0b000,
	(0x1 << 5) | 0b1,
	(0x3 << 5) | 0b100,
	(0x4 << 5) | 0b1000, 	/* v */
	(0x3 << 5) | 0b110,
	(0x4 << 5) | 0b1001,
	(0x4 << 5) | 0b1101,
	(0x4 << 5) | 0b0011 	/* z */
};

/* send a digit */

void send_digit(uint8_t code)
{
	for (int i = 0; i < DIGIT_LEN; i++) {

		if (code & 0x1)
			printf("dah ");
		else
			printf("dit ");
		
		code = code >> 1;
	}
}

/* send letters */

void send_letter(uint8_t code)
{
	int len = (code & 0b11100000) >> 5;

#if DEBUG
	printf("length: %d : ", len);
#endif

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
		if (*msg >= 48 && *msg <= 57) {

			printf("sending digit: %c\n", *msg);
			// send_digit(NUM_LEN, morse_digits[]); fixme

		} else if (*msg == 32)
			printf("sending space\n");
		else
			printf("sending letter: %c\n", *msg);

		// check for lower/upper case letters and reject invalid chars

		// send period.

	} while (*++msg != '\0');
}

#if DEBUG

int main()
{

	goto letters;

	printf("sending test string: 'vvv de n7mko'\n");
	morse_sendmsg("vvv de n7mko");

	letters:

	printf("testing letter patterns...\n");
	printf("a: "); send_letter(morse_letters['a' - ABASE]); putchar('\n');
	printf("b: "); send_letter(morse_letters['b' - ABASE]); putchar('\n');
	printf("c: "); send_letter(morse_letters['c' - ABASE]); putchar('\n');
	printf("d: "); send_letter(morse_letters['d' - ABASE]); putchar('\n');
	printf("e: "); send_letter(morse_letters['e' - ABASE]); putchar('\n');
	printf("f: "); send_letter(morse_letters['f' - ABASE]); putchar('\n');
	printf("g: "); send_letter(morse_letters['g' - ABASE]); putchar('\n');
	printf("h: "); send_letter(morse_letters['h' - ABASE]); putchar('\n');
	printf("i: "); send_letter(morse_letters['i' - ABASE]); putchar('\n');
	printf("j: "); send_letter(morse_letters['j' - ABASE]); putchar('\n');
	printf("k: "); send_letter(morse_letters['k' - ABASE]); putchar('\n');
	printf("l: "); send_letter(morse_letters['l' - ABASE]); putchar('\n');
	printf("m: "); send_letter(morse_letters['m' - ABASE]); putchar('\n');
	printf("n: "); send_letter(morse_letters['n' - ABASE]); putchar('\n');
	printf("o: "); send_letter(morse_letters['o' - ABASE]); putchar('\n');
	printf("p: "); send_letter(morse_letters['p' - ABASE]); putchar('\n');
	printf("q: "); send_letter(morse_letters['q' - ABASE]); putchar('\n');
	printf("r: "); send_letter(morse_letters['r' - ABASE]); putchar('\n');
	printf("s: "); send_letter(morse_letters['s' - ABASE]); putchar('\n');
	printf("t: "); send_letter(morse_letters['t' - ABASE]); putchar('\n');
	printf("u: "); send_letter(morse_letters['u' - ABASE]); putchar('\n');
	printf("v: "); send_letter(morse_letters['v' - ABASE]); putchar('\n');
	printf("w: "); send_letter(morse_letters['w' - ABASE]); putchar('\n');
	printf("x: "); send_letter(morse_letters['x' - ABASE]); putchar('\n');
	printf("y: "); send_letter(morse_letters['y' - ABASE]); putchar('\n');
	printf("z: "); send_letter(morse_letters['z' - ABASE]); putchar('\n');
	
	goto done;

	printf("testing number patterns...\n");
	printf("0: "); send_digit(morse_digits[0]); putchar('\n');
	printf("1: "); send_digit(morse_digits[1]); putchar('\n');
	printf("2: "); send_digit(morse_digits[2]); putchar('\n');
	printf("3: "); send_digit(morse_digits[3]); putchar('\n');
	printf("4: "); send_digit(morse_digits[4]); putchar('\n');
	printf("5: "); send_digit(morse_digits[5]); putchar('\n');
	printf("6: "); send_digit(morse_digits[6]); putchar('\n');
	printf("7: "); send_digit(morse_digits[7]); putchar('\n');
	printf("8: "); send_digit(morse_digits[8]); putchar('\n');
	printf("9: "); send_digit(morse_digits[9]); putchar('\n');

	done:

	return 0;
}

#endif
