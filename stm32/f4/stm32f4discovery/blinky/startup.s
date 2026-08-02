/* minimal startup with some debugging handlers */

.syntax unified
.cpu cortex-m4
.thumb

.global Reset_Handler
.global _estack

/* Minimal vector table. Only Reset is real; everything else
 * points at a default infinite-loop handler since we're not
 * using any interrupts in this blink test. */
.section .isr_vector, "a", %progbits
.word _estack           /* initial stack pointer */
.word Reset_Handler      /* Reset */
.word Default_Handler    /* NMI */
.word HardFault_Handler  /* HardFault */
.word MemManage_Handler  /* MemManage */
.word BusFault_Handler   /* BusFault */
.word UsageFault_Handler /* UsageFault */
.word 0                  /* reserved */
.word 0
.word 0
.word 0
.word Default_Handler    /* SVCall */
.word Default_Handler    /* DebugMon */
.word 0
.word Default_Handler    /* PendSV */
.word Default_Handler    /* SysTick */
/* Peripheral IRQ slots — reserved space, unused since no
 * interrupts are enabled. F407 has up to 82 IRQ lines. */
.space 4 * 91

.section .text.Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    ldr r0, =_estack
    mov sp, r0

    /* Copy .data section from flash to RAM */
    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata
copy_loop:
    cmp r1, r2
    bge copy_done
    ldr r3, [r0], #4
    str r3, [r1], #4
    b copy_loop
copy_done:
    /* zero .bss section */
    ldr r1, =_sbss
    ldr r2, =_ebss
    movs r3, #0
zero_loop:
    cmp r1, r2
    bge zero_done
    str r3, [r1], #4
    b zero_loop
zero_done:
    bl main
    b .
.size Reset_Handler, . - Reset_Handler

.section .text.Default_Handler
.type Default_Handler, %function
Default_Handler:
    b .
.size Default_Handler, . - Default_Handler

.section .text.HardFault_Handler
.type HardFault_Handler, %function
.global HardFault_Handler
HardFault_Handler:
    b .
.size HardFault_Handler, . - HardFault_Handler

.section .text.MemManage_Handler
.type MemManage_Handler, %function
.global MemManage_Handler
MemManage_Handler:
    b .
.size MemManage_Handler, . - MemManage_Handler

.section .text.BusFault_Handler
.type BusFault_Handler, %function
.global BusFault_Handler
BusFault_Handler:
    b .
.size BusFault_Handler, . - BusFault_Handler

.section .text.UsageFault_Handler
.type UsageFault_Handler, %function
.global UsageFault_Handler
UsageFault_Handler:
    b .
.size UsageFault_Handler, . - UsageFault_Handler
