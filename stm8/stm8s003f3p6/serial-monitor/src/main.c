/*
 * simple stm8 serial monitor
 */

#include "stm8s_conf.h"
#include "uart.h"

static void i2c_init(void)
{
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
	I2C_DeInit();

	/*
	 * 100 kHz bus, no slave addressing in this app, no ACK handling needed
	 * for address-probe scans.
	 */
	I2C_Init(100000, 0x00, I2C_DUTYCYCLE_2, I2C_ACK_NONE, I2C_ADDMODE_7BIT, 16);
	I2C_Cmd(ENABLE);
}

static uint8_t i2c_probe(uint8_t addr)
{
	while (I2C_GetFlagStatus(I2C_FLAG_BUSBUSY)) {
	}

	I2C_GenerateSTART(ENABLE);
	while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT)) {
	}

	I2C_Send7bitAddress((uint8_t)(addr << 1), I2C_DIRECTION_TX);

	if (I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
		I2C_GenerateSTOP(ENABLE);
		return 1;
	}

	/* Clear possible AF/STOP-related status and release bus. */
	(void)I2C->SR2;
	I2C_GenerateSTOP(ENABLE);
	return 0;
}

static void i2c_scan(void)
{
	uint8_t found = 0;

	uart_puts("I2C scan start\r\n");

	for (uint8_t addr = 1; addr < 127; addr++) {
		if (i2c_probe(addr)) {
			uart_puts("  found 0x");
			uart_puthex8(addr);
			uart_puts("\r\n");
			found++;
		}
	}

	if (found == 0) {
		uart_puts("  no devices\r\n");
	}

	uart_puts("I2C scan done\r\n\r\n");
}

static void clock_init(void) 
{
	/* Use HSI at full speed (16 MHz) */

	CLK_DeInit();
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

static void gpio_init(void) 
{
	/* Configure PD0 as push-pull output, low speed */

	GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST);
}

int main(void) 
{
	clock_init();

	gpio_init();

	uart_init(9600);

	i2c_init();

	while (1) {

		// i2c_scan();
	
		/* Toggle LED */

		GPIO_WriteReverse(GPIOD, GPIO_PIN_0);  

		for (volatile uint32_t i = 0; i < 200000; i++) {
			
			/* delay loop */
        	}
    	}
}
