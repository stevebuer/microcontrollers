# SDASSTM8 Blinky

## SDCC/SDAS Calling Convention / ABI

* Version 0: first argument on stack
* Version 1: first argument in register A

* Need to test with sdcc example! Which sdcc version changed it?
* bitfield compile example and document.

## SDAS Syntax Notes

Some relevent SDAS syntax notes:

* SDAS does not use the .global directive. Global symbols are specified with a '::' double colon e.g.
```asm
my_function::
	mov a, #250
	dec a
```
* Need a **.area DATA** directive even if empty, for proper linking

## Todo

* Interrupt vector table
* A GPIO abstraction that supports any pin on any port
* A TIM4‑based accurate delay module
* A linker‑area‑correct version using .area VECTORS, .area CODE, .area DATA

---

Loading from stack pointer relative
```asm
__asm
    ld a, (3, sp)   // load first arg from stack
    dec a
    clrw x
    ld xl, a
__endasm;
```
