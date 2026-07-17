#include "bsp_led.h"
#include "stm32f10x.h"   // SPL header

// Internal helper to map LED enum to pin
static uint16_t BSP_LED_GetPin(BSP_LED_t led) {
    switch (led) {
        case LED_GREEN: return GPIO_Pin_12;
        case LED_BLUE:  return GPIO_Pin_13;
        default:        return 0;
    }
}

void BSP_LED_Init(BSP_LED_t led) {
    GPIO_InitTypeDef gpio;

    // Enable GPIOD clock (LEDs are on PD12 and PD13)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    gpio.GPIO_Pin   = BSP_LED_GetPin(led);
    gpio.GPIO_Mode  = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(GPIOD, &gpio);
}

void BSP_LED_On(BSP_LED_t led) {
    GPIO_SetBits(GPIOD, BSP_LED_GetPin(led));
}

void BSP_LED_Off(BSP_LED_t led) {
    GPIO_ResetBits(GPIOD, BSP_LED_GetPin(led));
}

void BSP_LED_Toggle(BSP_LED_t led) {
    uint16_t pin = BSP_LED_GetPin(led);
    GPIOD->ODR ^= pin;
}

