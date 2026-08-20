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

# VID:PID 10c4:8044 is an EC3-type adapter, driven over libusb via port=USB (not a tty device)
EC2_PORT = USB

all: $(BUILD) $(TARGET).ihx

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.rel: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(TARGET).ihx: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET).ihx

clean:
	rm -rf $(BUILD) *.ihx *.lk *.map *.mem *.rst *.sym

flash: $(TARGET).ihx
	LD_LIBRARY_PATH=/usr/local/lib:$$LD_LIBRARY_PATH ec2writeflash --port=$(EC2_PORT) --hex --run $(TARGET).ihx

