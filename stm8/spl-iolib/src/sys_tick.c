/*
 * stm8 systick
 */

#include "stm8s.h"

volatile uint32_t systick_ms = 0;

void systick_init(void)
{
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, 124);
    TIM4_ClearFlag(TIM4_FLAG_UPDATE);
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
    TIM4_Cmd(ENABLE);
}

void systick_delay(uint32_t ms)
{
    uint32_t target = systick_ms + ms;

    while ((int32_t)(target - systick_ms) > 0) {
        /* busy wait */
    }
}

uint32_t systick_get(void)
{
    uint32_t t;
    disableInterrupts();
    t = systick_ms;
    enableInterrupts();
    return t;
}

INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
    TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
    systick_ms++;
}

