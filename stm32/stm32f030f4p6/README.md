# STM32F030F4P6 Development Board

<img src="stm32f0.jpg" width="200">

## Development Notes

Cortex Microcontroller Software Interface Standard (CMSIS) [Github repository](https://github.com/STMicroelectronics/STM32CubeF0).

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

Programming is performed via STLink adapter, supported by *stlink-tools* package on Debian.

* st-flash
* st-info
* st-trace
* st-util

## References

* [STM32F000 Datasheet](FIXME)
