@ 
@ stm32f0 gpio routines for blinky
@ 

.syntax unified
.cpu cortex-m0
.thumb

.include "stm32f0_reg.inc"

.global init_gpio
.global led_on
.global led_off

init_gpio:

	@ Enable GPIOC clock

	ldr r0, =RCC_AHBENR
	ldr r1, [r0]

	ldr r2, =RCC_IOPCEN
	orrs r1, r1, r2

	@ Set PC8 as output

	ldr r0, =GPIOC_MODER
	ldr r1, [r0]

	// bic r1, r1, #(3 << (8 * 2))   @ clear MODER8

	ldr r2, =(1<<8)
	eors r1, r1, r2

	bx lr

led_on:

	ldr r0, =1
	bx lr

led_off:
	
	ldr r0, =0 
	bx lr
