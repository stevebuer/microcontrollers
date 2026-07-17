/* atm32f0 blinky main */

    .syntax unified
    .cpu cortex-m0
    .thumb

    .global main
main:
	bl setup_gpio

loop:
	bl toggle_led
	movs r0, #100
	bl delay_ms
	b loop
