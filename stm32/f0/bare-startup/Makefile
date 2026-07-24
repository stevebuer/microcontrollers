PROJECT = firmware-minimal-startup

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

CORE_INC = Core/Inc
CORE_SRC = Core/Src
USER_SRC = User
LINKER = stm32f030f4.ld

CFLAGS = -mcpu=cortex-m0 -mthumb -Os -ffunction-sections -fdata-sections -I$(CORE_INC)
LDFLAGS = -T$(LINKER) -nostartfiles -Wl,--gc-sections

C_SOURCES = \
	$(CORE_SRC)/system.c \
	$(CORE_SRC)/start_c.c \
	$(CORE_SRC)/vector_table.c \
	$(USER_SRC)/main.c \
	$(USER_SRC)/app.c

ASM_SOURCES = \

all: check-linker $(PROJECT).bin

check-linker:
	@test -s $(LINKER) || { echo "error: linker script '$(LINKER)' is missing or empty"; false; }

$(PROJECT).elf: $(C_SOURCES) $(ASM_SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@
	$(SIZE) $<

clean:
	rm -f $(PROJECT).elf $(PROJECT).bin

flash: $(PROJECT).bin
	st-flash write $(PROJECT).bin 0x08000000

.PHONY: all clean flash check-linker
