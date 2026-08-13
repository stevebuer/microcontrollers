/*
 * simple stm8 serial monitor
 */

#include "stm8s_conf.h"
#include "uart.h"
#include "cmd.h"
#include "i2c_bus.h"
#include "onewire.h"

static void clock_init(void) 
{
	/* Use HSI at full speed (16 MHz) */

	CLK_DeInit();
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

static void gpio_init(void)
{
	/*
	 * Board LED is on PB5, which is also I2C SDA on STM8S003.
	 * Avoid configuring PB5 as push-pull when using I2C.
	 */
}

static void startup_led_blink(void)
{
	/* PB5 LED conflicts with SDA; no blink while using I2C on PB5. */
}

int main(void) 
{
	clock_init();

	gpio_init();

	uart_init(9600);

	startup_led_blink();

	i2c_bus_init();

	onewire_init();

	cmd_init();

	while (1) {

		cmd_poll();

		for (volatile uint32_t i = 0; i < 200000; i++);
    	}
}
