TARGET  = blinky
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

SOURCES = main.c
STARTUP = startup.s
CFLAGS  = -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -g -Wall -ffunction-sections -fdata-sections

LINKSCRIPT = linker.ld
LDFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -T $(LINKSCRIPT) -nostdlib -Wl,--gc-sections

all: $(TARGET).bin
	$(SIZE) $(TARGET).elf

$(TARGET).elf: $(SOURCES) $(STARTUP) $(LINKSCRIPT)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCES) $(STARTUP) -o $(TARGET).elf

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

flash: $(TARGET).bin
	st-flash write $(TARGET).bin 0x08000000

clean:
	rm -f *.o *.elf *.bin

.PHONY: all flash clean
