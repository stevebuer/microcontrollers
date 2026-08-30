PROJECT=led-pattern

all:
	sdcc --Werror -mmcs51 --opt-code-size -o $(PROJECT).hex $(PROJECT).c

clean:
	rm -f *.hex *.lk *.map *.mem *.rst *.sym *.cdb *.omf *.lst *.rel *.asm

flash:
	stcgal -P stc89 -p /dev/ttyUSB0 -b 9600 $(PROJECT).hex
