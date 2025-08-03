/*
 * micromorse
 *
 * keyer for 8051 microcontroller
 *
 * Steve Buer, N7MKO
 */

void dah()
{
}

void dit()
{
}

void space()
{
}

/* sdcc requires a user-provided putchar() implementation */

void putchar(char c)
{
	c = c + 1;
}
