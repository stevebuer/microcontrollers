SHELL := /bin/bash

CC := sdcc
AR := sdar

MCU ?= STM8S003
SPL_CANDIDATES := \
	$(HOME)/GITHUB/STM8-SPL-SDCC/Libraries/STM8S_StdPeriph_Driver \
	$(HOME)/STM8/STM8S/stm8s-sdcc
SPL_DETECTED := $(firstword $(foreach d,$(SPL_CANDIDATES),$(if $(wildcard $(d)/inc),$(d))))
SPL ?= $(or $(SPL_DETECTED),$(firstword $(SPL_CANDIDATES)))

SRC_DIR := src
INC_DIR := inc
BUILD_DIR := build

# Optional project include directory. Put your own stm8s_conf.h there and pass:
# make USER_INC_DIR=/path/to/project/inc
USER_INC_DIR ?=

LIB_BASENAME := spl-iolib
LIB := $(BUILD_DIR)/lib$(LIB_BASENAME).lib

INCLUDES := $(if $(USER_INC_DIR),-I$(USER_INC_DIR),) -I$(INC_DIR) -I$(SPL)/inc
CFLAGS := -mstm8 -D$(MCU) --std-sdcc11 --opt-code-size $(INCLUDES)

SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.rel,$(SOURCES))

all: check-spl $(LIB)

check-spl:
	@if [[ ! -d "$(SPL)/inc" ]]; then \
		echo "error: SPL include path not found: $(SPL)/inc"; \
		echo "checked: $(SPL_CANDIDATES)"; \
		echo "hint: set SPL=<path-to-stm8s-sdcc-or-stdperiph-driver-root>"; \
		exit 1; \
	fi

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.rel: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJECTS)
	rm -f $@
	$(AR) -rc $@ $^

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean check-spl


