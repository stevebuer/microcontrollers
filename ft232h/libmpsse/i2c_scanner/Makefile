CC ?= gcc
TARGET := i2c_scanner

CFLAGS ?= -O2 -Wall -Wextra -std=c11
CPPFLAGS ?= -I/usr/local/include
LDFLAGS ?= -L/usr/local/lib -Wl,-rpath,/usr/local/lib
LDLIBS ?= -lmpsse

SRC := main.c
OBJ := $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all clean