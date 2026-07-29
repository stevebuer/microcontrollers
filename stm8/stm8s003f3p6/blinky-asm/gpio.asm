    .module gpio

    .area CODE

; Exported symbols
    .globl _gpio_init
    .globl _gpio_set
    .globl _gpio_clear

; --- Register definitions (Port D example) ---
PD_ODR  = 0x500F
PD_DDR  = 0x5011
PD_CR1  = 0x5012

; --- gpio_init: configure PD0 as push-pull output ---
_gpio_init:
    bset PD_DDR, #0      ; PD0 output
    bset PD_CR1, #0      ; push-pull
    ret

; --- gpio_set: set PD0 high ---
_gpio_set:
    bset PD_ODR, #0
    ret

; --- gpio_clear: set PD0 low ---
_gpio_clear:
    bres PD_ODR, #0
    ret

