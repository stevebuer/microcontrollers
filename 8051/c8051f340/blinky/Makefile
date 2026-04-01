#
# Silicon Labs C8051F340 Development Board
#

MCU      = c8051f340
CC       = sdcc
CFLAGS   = --model-small --std-sdcc11 --opt-code-size --debug
LDFLAGS  =

SRC_DIR  = src
INC_DIR  = include
BUILD    = build

SRCS     = $(wildcard $(SRC_DIR)/*.c)
OBJS     = $(patsubst $(SRC_DIR)/%.c,$(BUILD)/%.rel,$(SRCS))

TARGET   = firmware

all: $(BUILD) $(TARGET).ihx

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.rel: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(TARGET).ihx: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET).ihx

clean:
	rm -rf $(BUILD) *.ihx *.lk *.map *.mem *.rst *.sym

flash:
	@echo "Use Silicon Labs IDE or Simplicity Studio to flash $(TARGET).ihx"

