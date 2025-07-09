# PIC 12F675-I/P

8-bit microcontroller in an 8-pin DIP.

Features:

* 35 instructions @ 4Mhz
* 1024 x 12 bit flash program memory
* 64 bytes static ram
* 128 bytes data eeprom
* 6 I/O pins
* 1 8/16 bit timer
* 1 comparator
* 4 10-bit A/D channels

Purchased a pack of 5 chips for $3.99 (+$4 ship) from eBay 6/2025.

## Assembly

* w : working register (accumulator)
* f : register file address (0x00 -> 0x7f)
* b : bit address
* k : literal
* x : don't care
* d : dest 0 (w) or 1 (f)

Common instructions: byte oriented, bit oriented, literal & control

* BCF (bit clear f)
* BSF (bit set f)
* CLEARF (clear f)
* CLEARW (clear w)
* ADDWF (add w and f)
* MOVF
* MOVWF

[Blink Example](blink.asm)

## C

## Programmer

## References

* [Datasheet](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/41190G.pdf)
* [PIC mid-range reference manual](https://ww1.microchip.com/downloads/en/DeviceDoc/33023a.pdf)
* [PIC Tutorial](https://groups.csail.mit.edu/lbr/stack/pic/pic-prog-assembly.pdf)
