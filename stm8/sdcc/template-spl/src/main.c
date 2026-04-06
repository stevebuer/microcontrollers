/*
 * stm8 main.c template
 */

#include "stm8s_conf.h"

void spi_init(void)
{
	SPI_DeInit();

	/* Enable SPI clock */

	CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, ENABLE);

	/* SPI in Master mode */

	SPI_Init(
        	SPI_FIRSTBIT_MSB,          	// MSB first
        	SPI_BAUDRATEPRESCALER_8,   	// f_CPU / 8 → 2 MHz at 16 MHz CPU
        	SPI_MODE_MASTER,           	// Master mode
		SPI_CLOCKPOLARITY_LOW,     	// CPOL = 0
        	SPI_CLOCKPHASE_1EDGE,      	// CPHA = 0 (Mode 0)
        	SPI_DATADIRECTION_1LINE_TX,	// Only MOSI used for now
        	SPI_NSS_SOFT,              	// Software NSS
        	0x07                       	// CRC polynomial (unused)
	);

	SPI_Cmd(ENABLE);
}

void spi_send(uint8_t data)
{
	SPI_SendData(data);
	while (SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);
}

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

