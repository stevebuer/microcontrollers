#include "stm8s_conf.h"
#include "touch.h"
#include "uart.h"

#define TOUCH_KEY_PIN GPIO_PIN_1
#define TOUCH_LOAD_PIN GPIO_PIN_2
#define TOUCH_SHIELD_PIN GPIO_PIN_3
#define TOUCH_DRIVE_PINS (TOUCH_KEY_PIN | TOUCH_SHIELD_PIN)
#define TOUCH_TIMEOUT 0x0E00U
#define TOUCH_DETECT_DELTA 15U

static uint16_t touch_baseline;
static uint8_t touch_was_detected;
static uint8_t touch_press_pending;

static uint16_t touch_timer_count(void)
{
	uint8_t high = TIM3->CNTRH;
	uint8_t low = TIM3->CNTRL;

	return ((uint16_t)high << 8) | low;
}

static uint16_t touch_wait_for(uint8_t high)
{
	uint16_t count;

	TIM3->CNTRH = 0;
	TIM3->CNTRL = 0;
	TIM3->EGR = TIM3_EGR_UG;

	while ((GPIO_ReadInputPin(GPIOC, TOUCH_KEY_PIN) != RESET) != high) {
		count = touch_timer_count();
		if (count >= TOUCH_TIMEOUT)
			return TOUCH_TIMEOUT;
	}

	return touch_timer_count();
}

static uint16_t touch_measure(void)
{
	uint8_t sample;
	uint16_t total = 0;

	for (sample = 0; sample < 8; sample++) {
		GPIOC->ODR &= (uint8_t)~TOUCH_DRIVE_PINS;
		GPIOC->DDR |= TOUCH_DRIVE_PINS;
		GPIOC->CR1 &= (uint8_t)~TOUCH_DRIVE_PINS;
		GPIOC->ODR |= TOUCH_LOAD_PIN;
		GPIOC->DDR &= (uint8_t)~TOUCH_DRIVE_PINS;
		total += touch_wait_for(1);

		GPIOC->ODR |= TOUCH_DRIVE_PINS;
		GPIOC->DDR |= TOUCH_DRIVE_PINS;
		GPIOC->CR1 |= TOUCH_DRIVE_PINS;
		GPIOC->ODR &= (uint8_t)~TOUCH_LOAD_PIN;
		GPIOC->CR1 &= (uint8_t)~TOUCH_DRIVE_PINS;
		GPIOC->DDR &= (uint8_t)~TOUCH_DRIVE_PINS;
		total += touch_wait_for(0);
	}

	GPIOC->ODR &= (uint8_t)~TOUCH_LOAD_PIN;
	GPIOC->ODR &= (uint8_t)~TOUCH_DRIVE_PINS;
	GPIOC->DDR |= TOUCH_DRIVE_PINS;
	return total >> 3;
}

void touch_init(void)
{
	uint8_t sample;
	uint32_t total = 0;

	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER3, ENABLE);
	TIM3->PSCR = 0;
	TIM3->ARRH = 0xFF;
	TIM3->ARRL = 0xFF;
	TIM3->CR1 = TIM3_CR1_CEN;

	GPIOC->CR1 |= TOUCH_LOAD_PIN;
	GPIOC->DDR |= TOUCH_LOAD_PIN;
	GPIOC->ODR &= (uint8_t)~TOUCH_LOAD_PIN;

	for (sample = 0; sample < 16; sample++)
		total += touch_measure();

	touch_baseline = (uint16_t)(total >> 4);
	touch_was_detected = 0;
	touch_press_pending = 0;
	uart_puts("TOUCH: ready\r\n");
}

void touch_poll(void)
{
	uint16_t measurement = touch_measure();
	uint16_t threshold = touch_baseline + TOUCH_DETECT_DELTA;
	uint8_t detected = measurement > threshold;

	if (detected != touch_was_detected) {
		uart_puts(detected ? "TOUCH: pressed\r\n" : "TOUCH: released\r\n");
		if (detected)
			touch_press_pending = 1;
		touch_was_detected = detected;
	}

	if (!detected)
		touch_baseline = (uint16_t)((touch_baseline * 7U + measurement) >> 3);
}

unsigned char touch_take_press(void)
{
	unsigned char pressed = touch_press_pending;

	touch_press_pending = 0;
	return pressed;
}