# STM32F103C8T6 Development Board

* 8 Mhz xtal
* 72 Mhz (max) Arm Cortex-M3 
* 64 KB Flash
* 20 KB SRAM
* USB 2.0, CAN, i2C, SPI, USART, iRDA, LIN
* JTAG + SWD

<img src="stm32f103_programming.jpg" width="200">

<img src="stm32f103_bluepill.jpg" width="200">

Todo: document differences between these two boards.

LED on GPIO pin 13.

Todo: USB Port Program loading via pin config?

## Development Notes

Cortex Microcontroller Software Interface Standard (CMSIS) [Github repository](https://github.com/STMicroelectronics/STM32CubeF1).

## Code

* Bare Metal Blinky
* USB Device Mode
* RTOS AX.25 Decoder

## Toolchains

### Linux

* GCC-ARM
* STM32 Standard Peripheral Library
* STLink software

### Windows

* IAR Embedded Workbench

## RTOS

FreeRTOS supports this architecture and has a couple of examples.

## Programming

### ST Link

Programming is performed via STLink adapter, supported by *stlink-tools* package on Debian.

* st-flash
* st-info
* st-trace
* st-util

### OpenOCD JTAG

Todo OpenOCD config + test

## References

* [STM32F103C8T6 Datasheet](https://www.st.com/resource/en/datasheet/stm32f103cb.pdf)
