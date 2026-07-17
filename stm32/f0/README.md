# STM32F0 MCU Series

[ST Micro](https://www.st.com/en/microcontrollers-microprocessors/stm32f0-series.html) 32 bit ARM Cortex M0 at 48 Mhz.

* Value Line F0x0
* Access Line F0x1
* USB Line F0x2
* Low Voltage F0x8

<img src="stm32f030f4p6.jpg" width="200">

## Development Notes

[STM32F0 (CMSIS) Github](https://github.com/STMicroelectronics/STM32CubeF0)

### Exmaple and Project Templates

* STM32CubeMX
* SPL

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

### Misc

Shell Variables

```
export CC=arm-none-eabi-gcc
export AS=arm-none-eabi-as
export OBJCOPY=arm-none-eabi-objcopy
export CFLAGS="-ICore/Inc -IUser"
alias mcc="$CC $CFLAGS"
```

## References

* [STM32F030F4 Datasheet](https://www.st.com/resource/en/datasheet/stm32f030f4.pdf)
* STM32F0
