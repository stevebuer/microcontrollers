/*
 * Morse code routines for various microcontrollers
 *
 * Steve Buer, N7MKO
 *
 */

#include <stdint.h>
#include <stdio.h>
#include "micromorse.h"

#define TESTPROG 1

#define DIGITS 10 	/* 10 digits in our table */
#define DIGIT_LEN 5 	/* numerals are fixed length */
#define ALPHABET 26 	/* 26 letters in our table */

/* dot: 0, dash: 1 */

uint8_t morse_digits[DIGITS] = { 0b11111, 0b11110, 0b11100, 0b11000, 0b10000, 0b00000, 0b00001, 0b00011, 0b00111, 0b01111 }; 

/* for letters, store length in upper 3 bits -- dit/dah encoding in lower 4 */

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

void send_digit(uint8_t code)
{
	for (int i = 0; i < DIGIT_LEN; i++) {

		if (code & 0x1)
			dah();
		else
			dit();
		
		code = code >> 1;
	}
}

/* send letters */

void send_letter(uint8_t code)
{
	int len = (code & 0b11100000) >> 5;

#if DEBUG
	printf("length: %d: ", len);
#endif

	for (int i = 0; i < len; i++) {

		if (code & 0x1)
			dah();
		else
			dit();
		
		code = code >> 1;
	}

	
}

/* send a string of characters */

void morse_sendmsg(char *msg)
{
	do {
		if (*msg >= '0' && *msg <= '9') {

			printf("sending digit: %c\n", *msg);
			send_digit(morse_digits[*msg - '0']);

		} else if (*msg == ' ') {

			printf("sending space\n");
			word_space();
		
		} else if (*msg >= 'a' && *msg <= 'z') {

			printf("sending letter: %c\n", *msg);
			send_letter(morse_letters[*msg - 'a']);

		} else if (*msg == '.') {
			
			printf("sending period\n");
			dit(); dah(); dit(); dah(); dit(); dah();

		} else
			printf("unknown character\n");

		putchar('\n');

	} while (*++msg != '\0');
}

#if TESTPROG

int main()
{
	letters:

	printf("testing letter patterns...\n");
	printf("a: "); send_letter(morse_letters['a' - 'a']); putchar('\n');
	printf("b: "); send_letter(morse_letters['b' - 'a']); putchar('\n');
	printf("c: "); send_letter(morse_letters['c' - 'a']); putchar('\n');
	printf("d: "); send_letter(morse_letters['d' - 'a']); putchar('\n');
	printf("e: "); send_letter(morse_letters['e' - 'a']); putchar('\n');
	printf("f: "); send_letter(morse_letters['f' - 'a']); putchar('\n');
	printf("g: "); send_letter(morse_letters['g' - 'a']); putchar('\n');
	printf("h: "); send_letter(morse_letters['h' - 'a']); putchar('\n');
	printf("i: "); send_letter(morse_letters['i' - 'a']); putchar('\n');
	printf("j: "); send_letter(morse_letters['j' - 'a']); putchar('\n');
	printf("k: "); send_letter(morse_letters['k' - 'a']); putchar('\n');
	printf("l: "); send_letter(morse_letters['l' - 'a']); putchar('\n');
	printf("m: "); send_letter(morse_letters['m' - 'a']); putchar('\n');
	printf("n: "); send_letter(morse_letters['n' - 'a']); putchar('\n');
	printf("o: "); send_letter(morse_letters['o' - 'a']); putchar('\n');
	printf("p: "); send_letter(morse_letters['p' - 'a']); putchar('\n');
	printf("q: "); send_letter(morse_letters['q' - 'a']); putchar('\n');
	printf("r: "); send_letter(morse_letters['r' - 'a']); putchar('\n');
	printf("s: "); send_letter(morse_letters['s' - 'a']); putchar('\n');
	printf("t: "); send_letter(morse_letters['t' - 'a']); putchar('\n');
	printf("u: "); send_letter(morse_letters['u' - 'a']); putchar('\n');
	printf("v: "); send_letter(morse_letters['v' - 'a']); putchar('\n');
	printf("w: "); send_letter(morse_letters['w' - 'a']); putchar('\n');
	printf("x: "); send_letter(morse_letters['x' - 'a']); putchar('\n');
	printf("y: "); send_letter(morse_letters['y' - 'a']); putchar('\n');
	printf("z: "); send_letter(morse_letters['z' - 'a']); putchar('\n');
	
	numbers:	

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

	testmsg:
	
	printf("sending test string: 'vvv de n7mko'\n");
	morse_sendmsg("vvv de n7mko");

	done:

	return 0;
}

#endif
