PROJECT = onewire
BUILD_DIR = build

SRC = main.c onewire.c
REL = $(addprefix $(BUILD_DIR)/,$(SRC:.c=.rel))

CC = sdcc
CFLAGS = --Werror -mmcs51 --opt-code-size

all: $(PROJECT).hex

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.rel: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(PROJECT).hex: $(REL)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(PROJECT).hex $(REL)
	cp $(BUILD_DIR)/$(PROJECT).hex $(PROJECT).hex

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(PROJECT).hex

flash:
	stcgal -P stc89 -p /dev/ttyUSB0 -b 9600 $(PROJECT).hex

.PHONY: all clean flash
