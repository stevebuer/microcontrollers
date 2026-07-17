/* Entry point */
ENTRY(Reset_Handler)

/* STM32F030F4P6 memory map */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 16K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 4K
}

/* Initial stack pointer = top of RAM */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    /* Vector table must be first in Flash */
    .text :
    {
        KEEP(*(.isr_vector))   /* Vector table */
        *(.text*)              /* Code */
        *(.rodata*)            /* Read‑only data */
    } > FLASH

    /* Initialized data (if you ever add C later) */
    .data :
    {
        *(.data*)
    } > RAM AT > FLASH

    /* Zero‑initialized data */
    .bss :
    {
        *(.bss*)
        *(COMMON)
    } > RAM
}
