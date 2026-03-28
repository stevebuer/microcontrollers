/*
 * stm8 main.c template
 */

#include "stm8s.h"
#include "stm8s_gpio.h"
#include "stm8s_clk.h"

static void clock_init(void) 
{
    /* Use HSI at full speed (16 MHz) */

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

    while (1) {
	
	/* Toggle LED */

	GPIO_WriteReverse(GPIOD, GPIO_PIN_0);  

        for (volatile uint32_t i = 0; i < 50000; i++) {
            /* delay loop */
        }
    }
}

