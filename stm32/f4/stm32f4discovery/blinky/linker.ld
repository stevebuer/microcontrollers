/* Minimal linker script for STM32F407VGT6 (STM32F407G-DISC1) */

MEMORY
{
   RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
   CCMRAM (xrw)	: ORIGIN = 0x10000000, LENGTH = 64K 
   FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 1024K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
  .isr_vector :
  {
    . = ALIGN(4); 
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } > FLASH

  .text :
  {
    . = ALIGN(4);
    *(.text*)
    *(.rodata*)
    . = ALIGN(4);

    _etext = .; 
  } > FLASH

  _sidata = LOADADDR(.data);

  .data :
  {
   . = ALIGN(4);
    _sdata = .;
    *(.data)
    *(.data*)

   . = ALIGN(4);
    _edata = .;
  } > RAM AT> FLASH

  .bss :
  {
  . = ALIGN(4);
    _sbss = .;
    *(.bss*)
    *(COMMON)

   . = ALIGN(4);
    _ebss = .;
  } > RAM
}
