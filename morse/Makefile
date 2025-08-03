#CC=gcc
CC=sdcc

all:

	$(CC) micromorse.c keyer-alsa.c -o micromorse

8051:

	$(CC) -c micromorse.c
	$(CC) -c keyer-8051.c
	$(CC) micromorse.rel keyer-8051.rel

alsa:

	gcc -o alsa-sine alsa-sine.c -lasound -lm
