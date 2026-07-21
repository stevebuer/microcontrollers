@
@ STM32F030 register definitions
@

.equ RCC_AHBENR, 0x40021014
.equ GPIOC_MODER, 0x48000800
.equ GPIOC_ODR, 0x48000814
.equ RCC_IOPCEN, (1 << 19)

@ MODER8 = 01 (output)
.equ GPIO_MODER8_OUT, (1 << (8 * 2)) 

