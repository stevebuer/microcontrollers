###############################################################################
# Project configuration
###############################################################################

# Name of the final firmware file (without extension)
TARGET      := firmware

# MCU family (used for include paths and optional flags)
MCU         := STM8S103

# Path to the stm8s-sdcc SPL (clone from https://github.com/stecman/stm8s-sdcc)
SPL_PATH    := ~/STM8/STM8S/stm8s-sdcc

# Project source files
SRC_DIR     := src
SOURCES     := $(wildcard $(SRC_DIR)/*.c)

# SPL source files (you may trim this list to only what you use)
SPL_SOURCES := $(wildcard $(SPL_PATH)/src/*.c)

#
# Sources can also be specified individually such as
#
# SPL_SOURCES := \
#  $(SPL_PATH)/src/stm8s_gpio.c \
#  $(SPL_PATH)/src/stm8s_clk.c \
#  $(SPL_PATH)/src/stm8s_uart1.c

# Output directory
BUILD_DIR   := build

###############################################################################
# Compiler / Linker settings
###############################################################################

CC          := sdcc
OBJCOPY     := packihx

CFLAGS      := -mstm8 \
               --std-sdcc99 \
               --opt-code-size \
               -I$(SRC_DIR) \
               -I$(SPL_PATH)/inc

LDFLAGS     := -mstm8

###############################################################################
# Build rules
###############################################################################

# Object files
OBJECTS     := $(patsubst %.c,$(BUILD_DIR)/%.rel,$(notdir $(SOURCES))) \
               $(patsubst %.c,$(BUILD_DIR)/%.rel,$(notdir $(SPL_SOURCES)))

# Default target
all: $(BUILD_DIR)/$(TARGET).ihx $(BUILD_DIR)/$(TARGET).bin

# Create build directory if missing
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile C → .rel
$(BUILD_DIR)/%.rel: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.rel: $(SPL_PATH)/src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link → .ihx
$(BUILD_DIR)/$(TARGET).ihx: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# Convert .ihx → .bin
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).ihx
	$(OBJCOPY) $< > $(BUILD_DIR)/$(TARGET).hex
	hex2bin $(BUILD_DIR)/$(TARGET).hex

###############################################################################
# Cleaning
###############################################################################

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
