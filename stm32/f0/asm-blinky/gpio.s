/* stm32f0 gpio for blinky */

    .syntax unified
    .cpu cortex-m0
    .thumb

    .equ RCC_AHBENR, 0x40021014
    .equ GPIOC_MODER, 0x48000800
    .equ GPIOC_ODR, 0x48000814

    .equ RCC_IOPCEN, (1 << 19)
    .equ GPIO_MODER_OUT, (1 << (8 * 2)) @ MODER8 = 01 (output)

.global _start
_start:

    /* Enable GPIOC clock */

    ldr r0, =RCC_AHBENR
    ldr r1, [r0]
    orr r1, r1, #RCC_IOPCEN
    str r1, [r0]

    /* Set PC8 as output */

    ldr r0, =GPIOC_MODER
    ldr r1, [r0]
    bic r1, r1, #(3 << (8 * 2))   @ clear MODER8
    orr r1, r1, #GPIO_MODER_OUT
    str r1, [r0]

loop:

    /* Toggle PC8 */

    ldr r0, =GPIOC_ODR
    ldr r1, [r0]
    eor r1, r1, #(1 << 8)
    str r1, [r0]

    /* Delay */

    // movs r2, #0
    call _delay
/*
delay:
    adds r2, r2, #1
    cmp r2, #0xFF
    bne delay

    b loop
*/
