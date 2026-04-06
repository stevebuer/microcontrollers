; stm8 delay

_delay_ms::
	push a			; callee save
        pushw x

ms_loop:
        ldw x, #250		; inner loop count (needs tuning)

inner_loop:
        nop
        decw x
        jrne inner_loop

        dec a			; outer loop
        jrne ms_loop

        popw x			; callee restore
        pop a
        ret
