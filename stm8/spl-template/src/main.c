/*
 * stm8 main.c template
 */

#include "stm8s_conf.h"

static void clock_init(void) 
{
	/* Use HSI at full speed (16 MHz) */

	CLK_DeInit();
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

	/* Enable  UART clock */

	CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);
}

static void gpio_init(void) 
{
	/* Configure PD0 as push-pull output, low speed */

	GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST);
}

static void uart_init(uint32_t baud) 
{
	UART1_DeInit();

	UART1_Init(baud, 
		UART1_WORDLENGTH_8D, 
		UART1_STOPBITS_1, 
		UART1_PARITY_NO,
               	UART1_SYNCMODE_CLOCK_DISABLE,
               	UART1_MODE_TXRX_ENABLE);
}

int main(void) 
{
	clock_init();
	gpio_init();

	while (1) {
	
		/* Toggle LED */

		GPIO_WriteReverse(GPIOD, GPIO_PIN_0);  

		for (volatile uint32_t i = 0; i < 50000; i++) {
			
			/* delay loop */
        	}
    	}
}

