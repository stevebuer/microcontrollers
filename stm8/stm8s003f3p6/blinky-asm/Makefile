SDCC = sdcc
AS = sdasstm8

all: main.ihx

main.ihx: startup.rel main.rel gpio.rel delay.rel
	$(SDCC) -mstm8 --out-fmt-ihx $^

%.rel: %.asm
	$(AS) -plosgff $<

clean:
	rm -f *.rel *.lst *.sym *.ihx *.lk *.map *.rst *.cdb
