all:

	gpasm -p 12f675 blink.asm

blinky: blinky.c

	# sdcc -mpic14 -p12f675 blink.c
	sdcc -p12f675 blinky.c

clean:

	rm -f *.cod *.hex *.lst
