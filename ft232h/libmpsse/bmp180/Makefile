PROJECT = bmp180

CC ?= gcc

INCLUDE = -I/usr/local/include -I../../../peripherals/bmp180/bosch
LDFLAGS = -L/usr/local/lib -Wl,-rpath,/usr/local/lib
LDLIBS = -lmpsse
SRC = main.c ../../../peripherals/bmp180/bosch/bmp180.c

all:

	$(CC) $(INCLUDE) $(SRC) $(LDFLAGS) $(LDLIBS) -o $(PROJECT)


clean:

	rm -f $(PROJECT)
