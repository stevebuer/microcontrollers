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

@ Enable Port A bit

.equ RCC_IOPAEN, (1 << 17)

@ Two bit mode register for pin 8 = 01 (output)

.equ GPIOA_PIN4_MASK, (3 << (4 * 2)) 	
.equ GPIOA_PIN4_OUT, (1 << (4 * 2)) 	

init_gpio:

	@ Enable GPIOA clock bit in RCC AHB

	ldr r0, =RCC_AHBENR
	ldr r1, [r0]

	ldr r2, =RCC_IOPAEN
	orrs r1, r1, r2

	str r1, [r0]

	@ Set MODER = 01 (output)

	ldr r0, =GPIOA_MODER
	ldr r1, [r0]

	ldr r2, =GPIOA_PIN4_MASK
	bics r1, r1, r2

	ldr r2, =GPIOA_PIN4_OUT
	eors r1, r1, r2
	
	str r1, [r0]

	bx lr

led_on:

	@ write to BSRR bit 4

	ldr  r0, =GPIOA_BSRR

	movs r1, #1
	lsls r1, r1, #4

	str  r1, [r0]

	bx lr

led_off:

	@ write to BSRR bit 20
	
	ldr  r0, =GPIOA_BSRR
	
	movs r1, #1
	lsls r1, r1, #20
	
	str  r1, [r0]
	
	bx lr
