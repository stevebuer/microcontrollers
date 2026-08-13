/*
 * simple stm8 serial monitor
 */

#include "stm8s_conf.h"
#include "uart.h"
#include "cmd.h"
#include "i2c_bus.h"
#include "onewire.h"
#include "touch.h"

#define LED_MODE_OFF 0
#define LED_MODE_SLOW 1
#define LED_MODE_FAST 2

static volatile uint16_t led_elapsed_ms;
static volatile uint16_t led_period_ms;
static volatile uint8_t led_mode;

static void clock_init(void) 
{
	/* Use HSI at full speed (16 MHz) */

	CLK_DeInit();
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

static void gpio_init(void)
{
	/* Discovery board LED1 is active-low on PD0. */
	GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_HIGH_FAST);
}

static void startup_delay(void)
{
	volatile uint32_t count;

	for (count = 0; count < 100000UL; count++) {
	}
}

static void startup_led_blink(void)
{
	uint8_t blink;

	for (blink = 0; blink < 3; blink++) {
		GPIO_WriteLow(GPIOD, GPIO_PIN_0);
		startup_delay();
		GPIO_WriteHigh(GPIOD, GPIO_PIN_0);
		startup_delay();
	}
}

static void led_blink_init(void)
{
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);
	TIM4->PSCR = 7;
	TIM4->ARR = 124;
	TIM4->CNTR = 0;
	TIM4->SR1 = 0;
	TIM4->IER = TIM4_IER_UIE;
	TIM4->CR1 = TIM4_CR1_CEN;

	led_elapsed_ms = 0;
	led_period_ms = 0;
	led_mode = LED_MODE_OFF;
}

static void led_cycle_mode(void)
{
	led_mode++;
	if (led_mode > LED_MODE_FAST)
		led_mode = LED_MODE_OFF;

	led_elapsed_ms = 0;
	if (led_mode == LED_MODE_OFF) {
		led_period_ms = 0;
		GPIO_WriteHigh(GPIOD, GPIO_PIN_0);
	} else if (led_mode == LED_MODE_SLOW) {
		led_period_ms = 500;
		GPIO_WriteLow(GPIOD, GPIO_PIN_0);
	} else {
		led_period_ms = 125;
		GPIO_WriteLow(GPIOD, GPIO_PIN_0);
	}
}

INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
{
	uart_rx_isr();
}

INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
	TIM4->SR1 = 0;

	if (led_mode != LED_MODE_OFF && ++led_elapsed_ms >= led_period_ms) {
		led_elapsed_ms = 0;
		GPIOD->ODR ^= GPIO_PIN_0;
	}
}

int main(void) 
{
	clock_init();

	gpio_init();

	uart_init(9600);

	startup_led_blink();

	led_blink_init();

	i2c_bus_init();

	onewire_init();

	touch_init();

	cmd_init();

	enableInterrupts();

	while (1) {

		cmd_poll();
		touch_poll();
		if (touch_take_press())
			led_cycle_mode();
    	}
}
