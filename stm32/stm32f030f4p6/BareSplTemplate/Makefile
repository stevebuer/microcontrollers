# project name
PROJECT = firmware

# toolchain
CC ?= arm-none-eabi-gcc
OBJCOPY ?= arm-none-eabi-objcopy
SIZE ?= arm-none-eabi-size

# directories
CORE_INC = Core/Inc
CORE_SRC = Core/Src
USER_SRC = User
LINKER = Linker/stm32f030x6.ld

# sources
C_SOURCES = \
	$(CORE_SRC)/system_stm32f0xx.c \
	$(USER_SRC)/main.c

ASM_SOURCES = \
	$(CORE_SRC)/startup_stm32f030x6.s

# flags
CFLAGS = -mcpu=cortex-m0 -mthumb -Os -ffunction-sections -fdata-sections -I$(CORE_INC)
LDFLAGS = -T$(LINKER) -nostartfiles -Wl,--gc-sections

# build targets
all: $(PROJECT).bin

$(PROJECT).elf: $(C_SOURCES) $(ASM_SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@
	$(SIZE) $<

clean:
	rm -f $(PROJECT).elf $(PROJECT).bin

# flash target
flash: $(PROJECT).bin
	st-flash write $(PROJECT).bin 0x08000000
