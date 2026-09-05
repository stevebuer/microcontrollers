/* 
 * Convert DS18B20 raw value to Celsius string
 *
 * Steve Buer
 * Olympic College
 * September 2026
 */

#include <stdio.h>
#include <stdint.h>

/* test values */

// int16_t raw_temp = 0x017D; // reading from my sensor
// int16_t raw_temp = 0x0191; // +25.0625 C
// int16_t raw_temp = 0x07D0; // +125 C
// int16_t raw_temp = 0xFFF8; // -0.5 C
int16_t raw_temp = 0xFF5F; // -10.0625
//int16_t raw_temp = 0xFC90; // -55 C


/* conversion example */

int main(int argc, char **argv)
{
	int8_t whole = 0, frac = 0, sign = 0;
	float f_frac = 0.0, f_multiplier = 0.0, f_celsius = 0.0;

	printf("  raw: %4X\n", raw_temp & 0xFFFF);

	/* sign */

	sign = (raw_temp & (1 << 15)) ? -1 : 1;

	printf(" sign: %d\n", sign);

	/* integer */

	whole = (raw_temp >> 4) & 0b11111111;

	printf("whole: 0x%2X -> %2d\n", whole & 0xFFFF, whole);

	/* fractional part */

	frac = raw_temp & 0b1111;

	f_frac = (float) frac;

	f_multiplier = (1.0 / 16.0);
	
	printf(" frac: 0x%02X -> %2d [%.4f * (%.4f)] = (%.4f)\n", frac, frac, f_frac, f_multiplier, f_frac * f_multiplier);

	f_celsius = (float) whole;

	f_celsius += f_frac * f_multiplier;
	
	printf(" temp: %.4f C (%.4f F)\n", f_celsius, (f_celsius * 1.8) + 32);

	return 0;
}
