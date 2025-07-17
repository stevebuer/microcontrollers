all: blink.ihx

blink.ihx: blink.c

	sdcc -mstm8 $<

flash:

	stm8flash -c stlinkv2 -p stm8s003f3 -s flash -w blink.ihx
