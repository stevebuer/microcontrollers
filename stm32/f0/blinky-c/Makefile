PROJECT = c-blinky

include cmsis.mk

MCU=STM32F051x8

C_SOURCES = gpio.c uart.c main.c
ASM_SOURCES = startup_stm32f051x8.s
LINKER_SCRIPT=STM32F051xx_FLASH.ld

CC      = arm-none-eabi-gcc
AS      = arm-none-eabi-as
LD      = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

CPU = -mcpu=cortex-m0 -mthumb
CFLAGS += $(CPU) -O2 -ffreestanding -nostdlib -Wall -I./CMSIS/Include -I./Device
LDFLAGS = $(CPU) -T $(LINKER_SCRIPT) -nostdlib

OBJ = $(C_SOURCES:.c=.o) $(ASM_SOURCES:.s=.o)

all: $(PROJECT).bin

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CC) $(CPU) -c $< -o $@

$(PROJECT).elf: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@

$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f *.o *.elf *.bin *.lst

dump:
	$(OBJDUMP) -d $(PROJECT).elf > $(PROJECT).lst
