# ST Micro STM8 8-bit microcontroller

16 Mhz Harvard architecture 8-bit MCU.

<img src="stm8s003f3p6.png" width="300">

My device sample is an STM8S003F3P6 (mainstream value line) with the following features:

* 8K flash program memory
* 1K byte RAM
* 128 byte data EEPROM
* UART, SPI i2c
* 3 timers
* ADC
* Interrupt controller 
* Boot ROM
* SWIM debugger
* Test LED on Pin/Port B5

## Architecture and Instruction Set

* 6 registers: X & Y (16 bit indexes), PC (24 bit program counter), SP (16 bit stack pointer), A (8-bit accumulator), CC (8-bit condition code)
* 20 addressing modes
* 80 instructions

* 16 MB linear program space
* 16 MB linear data space

* MUL/DIV Instructions

## Development tools

### ST Visual Develop / ST Assembler-Linker / ST Visual Programmer

Various tools are available for the ST7 & STM8 chips. I have installed ST Visual Develop, which has the [ST Assembler-Linker](https://www.st.com/resource/en/user_manual/um0144-st-assemblerlinker-stmicroelectronics.pdf) as the default tool chain. It supports some other C compilers and toolchains such as Raisonance and Cosmic. The interface is somewhat dated, but I am going
to give it a try. I need to read up a little on the assembler syntax. The IDE is bundled with the ST Visual Programmer application which hopefully will work with my cheap ST-LINK-V2 programmer. 

<img src="st-link-v2-ebay.png" width="300">

### Small Device C Compiler (SDCC)

The [SDCC](https://sdcc.sourceforge.net) supports the STM8 architecture. I need to read the manual on this. 

## Programming

The [ST Link Open Source](https://github.com/stlink-org/stlink) is available for Linux as well as the [Open On-Chip Debugger](https://openocd.org). Need to test.

## References

* [STM8 Wikipedia](https://en.wikipedia.org/wiki/STM8)
* [Data Sheet](https://www.st.com/resource/en/datasheet/stm8s003f3.pdf)
* [Programming Guide](file:///home/steve/Downloads/pm0044-stm8-cpu-programming-manual-stmicroelectronics.pdf)

