/*
 * BSP code based on STM32 SPL
 */

#include "bsp_button.h"
#include "stm32f10x.h"   // SPL header

/* User button on PA0 is connected to EXTI Line 0, triggers EXTIO_IRQn */

__weak void BSP_Button_Callback(BSP_Button_t btn) {
    // Application overrides this
}

/* Interrupt handler for EXTI0 */

void EXTI0_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        BSP_Button_Callback(BUTTON_USER);
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

/* Initialize */

void BSP_Button_Init(BSP_Button_t btn) {
    GPIO_InitTypeDef gpio;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // PA0 as input with pull-down
    gpio.GPIO_Pin = GPIO_Pin_0;
    gpio.GPIO_Mode = GPIO_Mode_IPD;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &gpio);
}

/* Read */

uint8_t BSP_Button_Read(BSP_Button_t btn) {
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
}

/* Enable IRQ */

void BSP_Button_EnableIRQ(BSP_Button_t btn) {
    EXTI_InitTypeDef exti;
    NVIC_InitTypeDef nvic;

    // Enable AFIO clock for EXTI
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // Connect EXTI0 line to PA0
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

    // Configure EXTI line
    exti.EXTI_Line = EXTI_Line0;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Rising;  // rising edge = button press
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);

    // NVIC setup
    nvic.NVIC_IRQChannel = EXTI0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0x0F;
    nvic.NVIC_IRQChannelSubPriority = 0x0F;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
}

