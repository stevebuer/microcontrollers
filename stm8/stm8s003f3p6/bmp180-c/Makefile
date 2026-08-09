SHELL = /bin/bash

SRC_DIR = src
INC_DIR = inc
BUILD_DIR = build

CC = sdcc
MCU = stm8s003f3
FLASH = stm8flash

CFLAGS = -mstm8 --std-sdcc11 -I$(INC_DIR)

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.rel,$(SRC))

TARGET = $(BUILD_DIR)/bmp180.ihx

all: $(TARGET)

# Link step (SDCC links automatically when given .rel files)
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# Compile each .c into .rel inside build/
$(BUILD_DIR)/%.rel: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

flash: $(TARGET)
	$(FLASH) -c stlinkv2 -p $(MCU) -s flash -w $(TARGET)

clean:
	rm -rf $(BUILD_DIR)

