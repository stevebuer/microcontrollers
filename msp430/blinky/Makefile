TI_HOME    ?= $(HOME)/ti
TOOLCHAIN  ?= $(TI_HOME)/msp430
GCC_DIR    ?= $(TOOLCHAIN)/bin
SUPPORT_DIR ?= $(TOOLCHAIN)/include

DEVICE  = MSP430G2553
FLASH_DEVICE ?= rf2500

MSPDEBUG = $(shell command -v mspdebug 2>/dev/null || echo)

# Prefer a working system toolchain. Reject legacy TI binaries that are present
# but unusable because they are missing required shared libraries.
ifeq ($(shell command -v msp430-elf-gcc >/dev/null 2>&1 && echo yes || echo no),yes)
CC      = msp430-elf-gcc
GDB     = msp430-elf-gdb
OBJCOPY = msp430-elf-objcopy
AGENT   = $(shell command -v gdb_agent_console 2>/dev/null || echo $(GCC_DIR)/gdb_agent_console)
else ifeq ($(shell command -v msp430-gcc >/dev/null 2>&1 && echo yes || echo no),yes)
CC      = msp430-gcc
GDB     = msp430-gdb
OBJCOPY = msp430-objcopy
AGENT   = $(shell command -v gdb_agent_console 2>/dev/null || echo $(GCC_DIR)/gdb_agent_console)
else ifeq ($(shell test -x "$(GCC_DIR)/msp430-elf-gcc" -a -x "$(GCC_DIR)/msp430-elf-gdb" -a "$(shell ldd "$(GCC_DIR)/msp430-elf-gdb" >/dev/null 2>&1 && echo yes || echo no)" = yes && echo yes || echo no),yes)
CC      = $(GCC_DIR)/msp430-elf-gcc
GDB     = $(GCC_DIR)/msp430-elf-gdb
OBJCOPY = $(GCC_DIR)/msp430-elf-objcopy
AGENT   = $(GCC_DIR)/gdb_agent_console
else
CC      = $(GCC_DIR)/msp430-elf-gcc
GDB     = $(GCC_DIR)/msp430-elf-gdb
OBJCOPY = $(GCC_DIR)/msp430-elf-objcopy
AGENT   = $(GCC_DIR)/gdb_agent_console
endif

SRC_DIR   = src
INC_DIR   = inc
BUILD_DIR = build

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
TARGET  = $(BUILD_DIR)/$(DEVICE)
MAP     = $(TARGET).map

CFLAGS = -I $(SUPPORT_DIR) -I $(INC_DIR) -mmcu=$(DEVICE) -Og -Wall -Wno-main -g -std=gnu11
LFLAGS = -L $(SUPPORT_DIR) -Wl,-Map,$(MAP),--gc-sections

.PHONY: all clean flash debug

all: $(TARGET).elf

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJECTS) -o $@

clean:
	$(RM) -r $(BUILD_DIR)

# Prefer mspdebug for the connected Launchpad/eZ430 board when available. Fall
# back to the TI gdb agent if the local TI toolchain is installed.
flash: $(TARGET).elf
	@if [ -n "$(MSPDEBUG)" ]; then \
		$(MSPDEBUG) $(FLASH_DEVICE) "prog $(TARGET).elf"; \
	elif [ -x "$(GDB)" ] && [ -x "$(AGENT)" ] && $(GDB) --batch -ex "quit" >/dev/null 2>&1; then \
		$(GDB) --batch \
			-ex "target remote | $(AGENT)" \
			-ex "load" \
			-ex "monitor reset" \
			-ex "detach" \
			$(TARGET).elf; \
	else \
		echo "No working MSP430 flash tool found. Install mspdebug or fix the broken TI toolchain (missing libncursesw.so.5)."; \
		exit 1; \
	fi

debug: $(TARGET).elf
	@if [ -x "$(GDB)" ] && [ -x "$(AGENT)" ] && $(GDB) --batch -ex "quit" >/dev/null 2>&1; then \
		$(GDB) -ex "target remote | $(AGENT)" $(TARGET).elf; \
	else \
		echo "No working MSP430 debug tool found. Install a working TI MSP430 toolchain or mspdebug for flashing."; \
		exit 1; \
	fi

