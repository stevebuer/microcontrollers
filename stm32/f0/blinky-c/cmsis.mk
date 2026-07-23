# CMSIS makefile includes

MCU=STM32F051x8

CUBE_FW_VERSION=V1.11.6
CUBE_FW = $(HOME)/STM32Cube/Repository/STM32Cube_FW_F0_$(CUBE_FW_VERSION)

CFLAGS += -D$(MCU)

CFLAGS += -I$(CUBE_FW)/Drivers/CMSIS/Device/ST/STM32F0xx/Include
CFLAGS += -I$(CUBE_FW)/Drivers/CMSIS/Include
CFLAGS += -I$(CUBE_FW)/Drivers/STM32F0xx_HAL_Driver/Inc

# Only compile the LL .c files you actually use -- don't pull in the whole Src/ tree
#SRCS += $(CUBE_FW)/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_gpio.c
#SRCS += $(CUBE_FW)/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_rcc.c
