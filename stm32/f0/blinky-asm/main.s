@
@ stm32f0 blinky main
@

.syntax unified
.cpu cortex-m0
.thumb

.global main

main:
	bl init_gpio
1:
	bl led_on @ change to toggle

	ldr r0, =1000
	bl delay_ms

	bl led_off
	
	ldr r0, =1000
	bl delay_ms

	b 1b
