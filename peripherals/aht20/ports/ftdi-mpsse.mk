CC ?= gcc
PKG_CONFIG ?= pkg-config

THIS_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
ROOT_DIR := $(abspath $(THIS_DIR)/..)
BUILD_DIR ?= $(THIS_DIR)/build-ftdi

TARGET := $(BUILD_DIR)/aht20_ftdi

CFLAGS ?= -O2 -g -Wall -Wextra -std=c99
CPPFLAGS += -I$(ROOT_DIR)

FTDI_CFLAGS := $(shell $(PKG_CONFIG) --cflags libftdi1 2>/dev/null)
FTDI_LIBS := $(shell $(PKG_CONFIG) --libs libftdi1 2>/dev/null)
ifeq ($(strip $(FTDI_LIBS)),)
FTDI_LIBS := -lftdi1
endif

CPPFLAGS += $(FTDI_CFLAGS)
LDLIBS += $(FTDI_LIBS)

SRCS := \
	$(ROOT_DIR)/aht20.c \
	$(THIS_DIR)/aht20_port_ftdi_mpsse.c

OBJS := \
	$(BUILD_DIR)/aht20.o \
	$(BUILD_DIR)/aht20_port_ftdi_mpsse.o

.PHONY: all run clean info

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/aht20.o: $(ROOT_DIR)/aht20.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/aht20_port_ftdi_mpsse.o: $(THIS_DIR)/aht20_port_ftdi_mpsse.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

run: $(TARGET)
	$(TARGET)

info:
	@echo "TARGET     = $(TARGET)"
	@echo "CPPFLAGS   = $(CPPFLAGS)"
	@echo "CFLAGS     = $(CFLAGS)"
	@echo "LDFLAGS    = $(LDFLAGS)"
	@echo "LDLIBS     = $(LDLIBS)"

clean:
	rm -rf $(BUILD_DIR)
