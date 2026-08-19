#include <msp430.h>
#include <stdint.h>
#include "adc.h"

int32_t adc_read_temperature_milli_c(void)
{
	uint16_t adc;

	ADC10CTL1 = INCH_10 | ADC10SSEL_3;
	ADC10CTL0 = SREF_1 | ADC10SHT_3 | REFON | ADC10ON;
	ADC10CTL0 |= ENC | ADC10SC;

	while (ADC10CTL1 & ADC10BUSY)
		;

	adc = ADC10MEM;
	ADC10CTL0 &= ~ENC;
	ADC10CTL0 &= ~(REFON | ADC10ON);

	return ((int32_t)adc - 673L) * 1000L / 1423L;
}
