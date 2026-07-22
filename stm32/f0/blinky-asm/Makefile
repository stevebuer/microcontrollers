# Toolchain

AS      = arm-none-eabi-as
LD      = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

# MCU

CPU     = -mcpu=cortex-m0 -mthumb

# Files

TARGET  = blinky
LINKER  = stm32f030f4.ld
SRCS    = startup.s delay.s gpio.s uart.s main.s
OBJS    = $(SRCS:.s=.o)

all: $(TARGET).bin

%.o: %.s
	$(AS) $(CPU) -o $@ $<

$(TARGET).elf: $(OBJS) $(LINKER)
	$(LD) -T $(LINKER) -o $@ $(OBJS)

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f *.o *.elf *.bin
