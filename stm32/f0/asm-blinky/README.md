# ARM Cortex‑M Assembly Basics

## 1. **Registers**
ARM Cortex‑M has a *real* register file:

- **r0–r3** → argument/return registers  
- **r4–r11** → callee‑saved general registers  
- **r12** → scratch  
- **r13** → stack pointer (SP)  
- **r14** → link register (LR)  
- **r15** → program counter (PC)  

## 2. **Instruction set**

Cortex‑M0 uses **Thumb‑1** instructions:

- 16‑bit instructions  
- simple, compact  
- no barrel shifter on every instruction  
- no conditional execution (except `b<cond>`)

Examples:

```asm
movs r0, #1
adds r1, r1, #4
lsls r2, r2, #3
str  r0, [r1]
ldr  r3, =0x48000014
```

## 3. **Memory‑mapped I/O**

Example: enabling GPIOA clock on STM32F0

```asm
ldr r0, =0x40021014     @ RCC_AHBENR
ldr r1, =0x00020000     @ IOPAEN bit
str r1, [r0]
```

Setting PA4 as output:

```asm
ldr r0, =0x48000000     @ GPIOA_MODER
ldr r1, =0x00000100     @ MODER4 = 01
str r1, [r0]
```

Toggling PA4:

```asm
ldr r0, =0x48000014     @ GPIOA_ODR
ldr r1, [r0]
eors r1, r1, #0x10
str r1, [r0]
```

## 4. **Vector table + reset handler**

```asm
.section .isr_vector, "a"
.word _estack
.word Reset_Handler
```

Reset handler:

```asm
.global Reset_Handler
Reset_Handler:
    ldr r0, =_estack
    mov sp, r0
    bl main
```

## 5. **A minimal ARM blinky loop**
This is the “hello world” of embedded ARM:

```asm
.global main
main:
loop:
    ldr r0, =0x48000014   @ GPIOA_ODR
    ldr r1, [r0]
    eors r1, r1, #0x10
    str r1, [r0]

    movs r2, #0xFF
delay1:
    movs r3, #0xFF
delay2:
    subs r3, r3, #1
    bne delay2
    subs r2, r2, #1
    bne delay1

    b loop
```

## ARM Cortex ASM Blinky Modules

### ✔ `startup.s`
- vector table  
- Reset_Handler  
- stack initialization  

### ✔ `main.s`
- blinky loop  
- delay loop  

### ✔ `gpio.s`
- GPIO setup  

### ✔ `delay.s`
- Wait routines

### ✔ `stm32f030f4.ld`
- Flash/RAM layout  
- vector table placement  

### ✔ `Makefile`
- builds startup + main  
- produces `.elf` and `.bin`  
