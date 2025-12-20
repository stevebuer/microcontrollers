##
## Makefile for AX.25 Decoder (STM32F103C8T6)
##
## Author: Steve
## Date: 2025
##

# Project name
PROJECT = ax25-decoder

# Build directory
BUILD_DIR = build

# Toolchain
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)as
LD = $(PREFIX)ld
OBJCOPY = $(PREFIX)objcopy
OBJDUMP = $(PREFIX)objdump
SIZE = $(PREFIX)size
GDB = $(PREFIX)gdb

# MCU settings
MCU = cortex-m3
MCU_FLAGS = -mcpu=$(MCU) -mthumb

# Linker script (to be added)
LDSCRIPT = STM32F103C8Tx_FLASH.ld

# Source directories
SRC_DIR = src
INC_DIR = inc
FREERTOS_DIR = FreeRTOS
HAL_DIR = STM32_HAL

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# TODO: Add FreeRTOS source files
# FREERTOS_SRCS = \
#     $(FREERTOS_DIR)/Source/tasks.c \
#     $(FREERTOS_DIR)/Source/queue.c \
#     $(FREERTOS_DIR)/Source/list.c \
#     $(FREERTOS_DIR)/Source/timers.c \
#     $(FREERTOS_DIR)/Source/portable/GCC/ARM_CM3/port.c \
#     $(FREERTOS_DIR)/Source/portable/MemMang/heap_4.c

# TODO: Add STM32 HAL source files
# HAL_SRCS = \
#     $(HAL_DIR)/Src/stm32f1xx_hal.c \
#     $(HAL_DIR)/Src/stm32f1xx_hal_rcc.c \
#     $(HAL_DIR)/Src/stm32f1xx_hal_gpio.c \
#     $(HAL_DIR)/Src/stm32f1xx_hal_adc.c \
#     $(HAL_DIR)/Src/stm32f1xx_hal_uart.c \
#     $(HAL_DIR)/Src/stm32f1xx_hal_cortex.c

# Include paths
INCLUDES = \
    -I$(INC_DIR) \
    # -I$(FREERTOS_DIR)/Source/include \
    # -I$(FREERTOS_DIR)/Source/portable/GCC/ARM_CM3 \
    # -I$(HAL_DIR)/Inc \
    # -I$(HAL_DIR)/Inc/Legacy

# Compiler flags
CFLAGS = \
    $(MCU_FLAGS) \
    -O2 \
    -Wall \
    -Wextra \
    -Werror \
    -g3 \
    -ffunction-sections \
    -fdata-sections \
    -specs=nano.specs \
    $(INCLUDES)

# Preprocessor definitions
DEFS = \
    -DSTM32F103xB \
    -DUSE_HAL_DRIVER \
    -DHSE_VALUE=8000000

CFLAGS += $(DEFS)

# Linker flags
LDFLAGS = \
    $(MCU_FLAGS) \
    -specs=nano.specs \
    -T$(LDSCRIPT) \
    -Wl,--gc-sections \
    -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map,--cref

# Libraries
LIBS = -lc -lm -lnosys

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
# OBJS += $(FREERTOS_SRCS:$(FREERTOS_DIR)/%.c=$(BUILD_DIR)/%.o)
# OBJS += $(HAL_SRCS:$(HAL_DIR)/%.c=$(BUILD_DIR)/%.o)

# Output files
ELF = $(BUILD_DIR)/$(PROJECT).elf
HEX = $(BUILD_DIR)/$(PROJECT).hex
BIN = $(BUILD_DIR)/$(PROJECT).bin
LST = $(BUILD_DIR)/$(PROJECT).lst

# Default target
all: $(BUILD_DIR) $(ELF) $(HEX) $(BIN) $(LST) size

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "CC $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Link
$(ELF): $(OBJS)
	@echo "LD $@"
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

# Generate hex file
$(HEX): $(ELF)
	@echo "OBJCOPY $@"
	$(OBJCOPY) -O ihex $< $@

# Generate binary file
$(BIN): $(ELF)
	@echo "OBJCOPY $@"
	$(OBJCOPY) -O binary $< $@

# Generate listing file
$(LST): $(ELF)
	@echo "OBJDUMP $@"
	$(OBJDUMP) -h -S $< > $@

# Print size information
size: $(ELF)
	@echo "Size information:"
	$(SIZE) $<

# Flash using OpenOCD
flash: $(HEX)
	openocd -f interface/stlink.cfg \
	        -f target/stm32f1x.cfg \
	        -c "program $(HEX) verify reset exit"

# Debug with GDB
debug: $(ELF)
	$(GDB) $(ELF) \
	    -ex "target extended-remote :3333" \
	    -ex "monitor reset halt"

# Start OpenOCD server
openocd:
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Clean and rebuild
rebuild: clean all

# Show configuration
config:
	@echo "Project: $(PROJECT)"
	@echo "MCU: STM32F103C8T6 ($(MCU))"
	@echo "Toolchain: $(PREFIX)"
	@echo "Sources: $(words $(SRCS)) files"
	@echo "Build dir: $(BUILD_DIR)"

# Help
help:
	@echo "AX.25 Decoder Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  all      - Build project (default)"
	@echo "  clean    - Remove build artifacts"
	@echo "  rebuild  - Clean and build"
	@echo "  flash    - Flash to MCU using OpenOCD"
	@echo "  debug    - Start GDB debug session"
	@echo "  openocd  - Start OpenOCD server"
	@echo "  size     - Show size information"
	@echo "  config   - Show configuration"
	@echo "  help     - Show this help"
	@echo ""
	@echo "Setup requirements:"
	@echo "  1. Install ARM toolchain: sudo apt-get install gcc-arm-none-eabi"
	@echo "  2. Install OpenOCD: sudo apt-get install openocd"
	@echo "  3. Download FreeRTOS and place in $(FREERTOS_DIR)/"
	@echo "  4. Download STM32 HAL and place in $(HAL_DIR)/"
	@echo "  5. Create linker script $(LDSCRIPT)"

.PHONY: all clean rebuild flash debug openocd size config help
