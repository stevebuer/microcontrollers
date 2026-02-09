# ST Micro STM32

I have several samples of this architecture including:

* [STM32F030F4P6](stm32f030f4p6)
* [STM32VLDISCOVERY](stm32vldiscovery)
* [STM32F103C8T6 Development Boards](stm32f103c8t6)

<img src="STM32FxCxT6-devboard.jpg" width="200">

# Development Notes

STM32Cube is their configuration and development system that includes:

* STM32CubeMX graphical configuration and code generation tool.
* Cortex Microcontroller Software Interface Standard (CMSIS) for each family e.g. STM32CubeF1 which provides:
   * Board Support Package (for devices on ST Dev Boards: displays, etc)
   * STM32 HAL
   * Lower Layer (LL) APIs which provide expert access for certain peripherals
   * Middleware components: RTOS, USB, Filesytem, Graphics
   * Example programs

# Development Tools

* STM32Cube IDE
* Keil MDK-ARM
* IAR Embedded Workbench
* PlatformIO (vsCode)
* GCC-ARM, Makefile, OpenOCD, STM32CubeMX

# Programming

<img src="stlink_v2.jpg" width="200">

Programming of the chips is accomplished via an ST-Link adapter and software.

On Linux the *stlink-tools* package is available.

# References
