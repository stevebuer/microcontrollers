/* stm32f0 startup */

    .syntax unified
    .cpu cortex-m0
    .thumb

    .section .isr_vector, "a", %progbits

    .word   _estack         /* Initial stack pointer */
    .word   Reset_Handler   /* Reset vector */

    .word   0 /* NMI */
    .word   0 /* HardFault */
    .word   0 /* Reserved */
    .word   0 /* Reserved */
    .word   0 /* Reserved */
    .word   0 /* Reserved */
    .word   0 /* Reserved */
    .word   0 /* Reserved */
    .word   0 /* Reserved */
    .word   0 /* SVCall */
    .word   0 /* Reserved */
    .word   0 /* Reserved */
    .word   0 /* PendSV */
    .word   0 /* SysTick */

    .global Reset_Handler
Reset_Handler:
    ldr   r0, =_estack
    mov   sp, r0

    bl    main

hang:
    b hang
