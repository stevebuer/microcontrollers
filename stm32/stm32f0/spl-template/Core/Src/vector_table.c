/* vector table implementation in C if preferred over ASM */

#include <stdint.h>
#include "stm32f0xx.h"

/* Forward declaration of handlers */

void Reset_Handler(void);
void Default_Handler(void);

/* Weak aliases for all exception handlers */

void NMI_Handler(void)              __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)              __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)           __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)          __attribute__((weak, alias("Default_Handler")));

/* Weak aliases for all IRQ handlers (F0 example set) */

void WWDG_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void RTC_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void FLASH_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void RCC_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void EXTI0_1_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void EXTI2_3_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void EXTI4_15_IRQHandler(void)      __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel2_3_IRQHandler(void)__attribute__((weak, alias("Default_Handler")));
void ADC1_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)__attribute__((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void TIM3_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void TIM14_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void TIM16_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void TIM17_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void I2C1_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void SPI1_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void USART1_IRQHandler(void)        __attribute__((weak, alias("Default_Handler")));

/* Top of stack defined by linker script */

extern uint32_t _estack;

/* Vector table placed at start of flash */
 
__attribute__((section(".isr_vector")))

const void *vector_table[] = {

    (void *)&_estack,                     /* Initial stack pointer */
    Reset_Handler,                        /* Reset handler */
    NMI_Handler,
    HardFault_Handler,
    0, 0, 0, 0, 0, 0, 0,                   /* Reserved */
    SVC_Handler,
    0, 0,
    PendSV_Handler,
    SysTick_Handler,

    /* IRQ handlers */
    WWDG_IRQHandler,
    0,
    RTC_IRQHandler,
    FLASH_IRQHandler,
    RCC_IRQHandler,
    EXTI0_1_IRQHandler,
    EXTI2_3_IRQHandler,
    EXTI4_15_IRQHandler,
    0,
    DMA1_Channel1_IRQHandler,
    DMA1_Channel2_3_IRQHandler,
    ADC1_IRQHandler,
    TIM1_BRK_UP_TRG_COM_IRQHandler,
    TIM1_CC_IRQHandler,
    0,
    TIM3_IRQHandler,
    0, 0,
    TIM14_IRQHandler,
    0,
    TIM16_IRQHandler,
    TIM17_IRQHandler,
    I2C1_IRQHandler,
    0,
    SPI1_IRQHandler,
    0,
    USART1_IRQHandler,
    0, 0
};

/* Reset handler implemented elsewhere (startup or C file) */

void Reset_Handler(void) 
{
	extern void _start(void);
	_start();
}

/* Default handler */

void Default_Handler(void) 
{
	while (1) {}
}

