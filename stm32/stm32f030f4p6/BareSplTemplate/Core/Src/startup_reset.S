/*
 * m0 minimal startup with vector table in C
 */

.syntax unified
.cpu cortex-m0
.thumb

.global Reset_Handler

.extern _estack
.extern _start

.section .text.Reset_Handler

Reset_Handler:
	ldr   r0, =_estack     /* load initial stack pointer */
	mov   sp, r0
	bl    _start           /* jump to C runtime init */

1:  
	b     1b               /* infinite loop if _start returns */

/* crt0 */

.global _start
_start:

	/* copy .data section from Flash (_sidata) to RAM (_sdata) */

	ldr r1, =_sidata     /* source: Flash */
	ldr r2, =_sdata      /* destination: RAM */
	ldr r3, =_edata      /* end of .data in RAM */

copy_data:
	cmp r2, r3
	bcc copy_data_do
	b   copy_bss

copy_data_do:
	ldr r0, [r1]
	adds r1, #4

	str r0, [r2]
	adds r2, #4

	b   copy_data

copy_bss:
	ldr r2, =_sbss
	ldr r3, =_ebss

zero_bss:
	cmp r2, r3
	bcc zero_bss_do
	b   call_main

zero_bss_do:
	movs r0, #0
	str r0, [r2]
	adds r2, #4
	b   zero_bss

call_main:
    bl main

hang:
    b hang               /* main() should never return */
