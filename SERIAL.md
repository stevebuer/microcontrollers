# Serial Interfaces

Notes on serial interfaces

## Adafruit 954 USB Serial to TTL (3.3V)

FTDI PL2303HX

Pinout

* red: 5V, 500mA
* black: GND
* white: RXD 
* green: TXD 

[Adafruit 954 Datasheet](https://www.farnell.com/datasheets/1651403.pdf)


## Mincom

```
minicom -D /dev/ttyUSB0 -b 115200
```
