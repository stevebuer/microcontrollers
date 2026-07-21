@ 
@ stm32f0 delay routines for 8 Mhz
@

.syntax unified
.cpu cortex-m0
.thumb

.global delay_short
.global delay_ms
.global delay_custom

@
@ fixed count delay
@

delay_short:
	
	movs r1, #0x40        
1:  
	subs r1, r1, #1
	bne 1b
	bx lr

@
@ delay for r0 iterations
@

delay_custom:

	cmp r0, #0
	beq 2f
1:  
	subs r0, r0, #1
	bne 1b
2:  
	bx lr

@
@ delay for r0 milliseconds
@

delay_ms:

	push {r1, r2}
	movs r2, #0          @ outer loop counter
1:  
	cmp r2, r0
	beq 3f

	movs r1, #200        @ inner loop — adjust for clock
2:
	subs r1, r1, #1
	bne 2b

	adds r2, r2, #1
	b   1b
3:  
	pop {r1, r2}
	bx lr
