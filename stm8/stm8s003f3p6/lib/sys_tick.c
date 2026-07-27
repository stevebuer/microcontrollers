/*
 * stm8 systick
 */

#include "stm8s.h"

volatile uint32_t systick_ms = 0;

void systick_init(void)
{
    TIM4_PSCR = 0x07;      	/* Prescaler = 128 */
    TIM4_ARR  = 124;       	/* 1ms period */
    TIM4_CNTR = 0;         	/* Reset counter */
    TIM4_IER |= TIM4_IER_UIE; 	/* Enable update interrupt */
    TIM4_CR1 |= TIM4_CR1_CEN; 	/* Start timer */
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

@far @interrupt void TIM4_UPD_OVF_IRQHandler(void)
{
    TIM4_SR &= ~TIM4_SR_UIF; 	/* Clear interrupt flag */
    systick_ms++;
}

