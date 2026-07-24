# STM32F0 GPIO

Based on STM32F030x4x6 (RM0360)

## Registers

Each GPIO Port has four 32-bit config registers.

* GPIOx_MODER
* GPIOx_OTYPER
* GPIOx_OSPEEDR
* GPIOx_PUPDR

Ports A & B also having locking registers and alternate function selection registers.

I/O Port Data Registers (16 bit)

* GPIOx_IDR
* GPIOx_ODR
