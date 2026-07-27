SHELL=/bin/bash

all: blink.ihx blink2.ihx

blink.ihx: blink.c

	sdcc -mstm8 $<

blink2.ihx: blink2.c
	
	sdcc -mstm8 $<

flash:

	#stm8flash -c stlinkv2 -p stm8s003f3 -s flash -w blink.ihx
	stm8flash -c stlinkv2 -p stm8s003f3 -s flash -w blink2.ihx

clean:

	rm -f *.{asm,ihx,lk,lst,map,rel,rst,sym}
