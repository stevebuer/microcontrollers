@
@ stm32f0 blinky main
@

.syntax unified
.cpu cortex-m0
.thumb

.global main

main:
	bl init_gpio

loop1:
	// change this to a toggle?

	bl led_on

	movs r0, #100 @ this register will get overwritten?
	bl delay_ms

	bl led_off
	
	movs r0, #100 @ this register will get overwritten?
	bl delay_ms

	b loop1
