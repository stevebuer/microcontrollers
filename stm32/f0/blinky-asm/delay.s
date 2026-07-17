    .syntax unified
    .cpu cortex-m0
    .thumb

    .global delay_short
    .global delay_ms
    .global delay_custom

delay_short:
    movs r1, #0x40        @ adjust as needed
1:  subs r1, r1, #1
    bne 1b
    bx lr

/* ---------------------------------------------------------
 * delay_custom
 * Delay for r0 iterations.
 *   r0 = loop count
 * --------------------------------------------------------- */
delay_custom:
    cmp r0, #0
    beq 2f
1:  subs r0, r0, #1
    bne 1b
2:  bx lr

/* ---------------------------------------------------------
 * delay_ms
 * Crude millisecond delay.
 *   r0 = number of ms
 * Adjust INNER_LOOP for your clock (8 MHz, 48 MHz, etc.)
 * --------------------------------------------------------- */

delay_ms:
    push {r1, r2}

    movs r2, #0          @ outer loop counter
1:  cmp r2, r0
    beq 3f

    movs r1, #200        @ INNER_LOOP — tune for your clock
2:  subs r1, r1, #1
    bne 2b

    adds r2, r2, #1
    b   1b

3:  pop {r1, r2}
    bx lr

