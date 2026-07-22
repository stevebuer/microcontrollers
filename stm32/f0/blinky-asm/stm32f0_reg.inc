@
@ STM32F030 register definitions (RM0360)
@

@ Peripheral register bases

.equ PERIPH_BASE, 0x40000000
.equ RCC_BASE, (PERIPH_BASE + 0x21000)
.equ GPIOA_BASE, (PERIPH_BASE + 0x8000000)

@ RCC for Advanced High-Performance Bus

.equ RCC_AHBENR, (RCC_BASE + 0x14)
.equ RCC_IOPAEN, (1 << 17)

@ IO Port A Mode Register

.equ GPIOA_MODER, (GPIOA_BASE + 0x0)

@ Port A Bit Set-Reset Register (BSRR)

.equ GPIOA_BSRR, (GPIOA_BASE + 0x18)

@ vim: filetype=armv5
