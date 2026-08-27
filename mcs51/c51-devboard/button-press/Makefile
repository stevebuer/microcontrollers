PROJECT=button-press

all:
	sdcc --Werror -mmcs51 --opt-code-size -o $(PROJECT).hex $(PROJECT).c

clean:
	rm -f *.hex *.lk *.map *.mem *.rst *.sym *.cdb *.omf *.lst *.rel *.asm
