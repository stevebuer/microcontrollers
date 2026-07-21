PROJECT = c-blinky

C_SOURCES = main.c
ASM_SOURCES = startup.s

CC      = arm-none-eabi-gcc
AS      = arm-none-eabi-as
LD      = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

CPU = -mcpu=cortex-m0 -mthumb
CFLAGS = $(CPU) -O2 -ffreestanding -nostdlib -Wall -I./CMSIS/Include -I./Device

LDFLAGS = $(CPU) -T linker.ld -nostdlib

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

