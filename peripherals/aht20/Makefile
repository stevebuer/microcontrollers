CC = gcc
SRC = aht20.c
OBJ = $(patsubst %.c,%.o,$(SRC))

all: $(OBJ)

$(OBJ): $(SRC)

	gcc -c -o $(OBJ) $(SRC)

clean:

	rm -f *.o
