;
; sdasstm8 blinky example
;
	.module main

	.area DATA

enable:
	.db 0

	.area CODE

_main::
	call _gpio_init

loop:
	call _gpio_set
	ld a, #100
	call _delay_ms

	call _gpio_clear
	ld a, #100
	call _delay_ms

	jp loop
