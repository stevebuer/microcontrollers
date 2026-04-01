# 8051 Register File

<img src="https://www.8052mcu.com/images/sfr8051.gif">

SFR map from [8052mcu.com](https://www.8052mcu.com/tutsfr.htm)

## Bit Addressable Rule

Only SFR whose byte address ends in 0 or 8 are bit-addressable.

## Registers

* A — Accumulator
* B — Multiply/divide register
* R0–R7 — Working registers (4 banks numbered 0-3, selected by PSW RS1/RS0)
* DPTR — Data pointer (16 bit)
* PC — Program counter
* SP — Stack pointer

## Special Function Registers

0x80 -> 0xFF

### Byte vs Bit Address Decoders

Numeric ranges are the same (overlap), but separate decoders and instructions.

* MOV (byte address)
* SETB, CLR, JB, (bit address)

### Port Latches

* P0 0x80
* P1 0x90
* P2 0xA0
* P3 0xB0

### Peripheral Control Registers

* PCON 0x87 (Power control + SMOD)
* TCON 0x88 (Timer control)
* TMOD 0x89 (Timer mode)
* SCON 0x98 (Serial control - bit addressable)
* IE
* IP

### SFR Bit Addresses

| SFR | Byte Addr | Bit Addrs |
| --- | --- | --- |
| P0 | 0x80 | 0x80–0x87 |
| TCON | 0x88 | 0x88–0x8F |
| P1 | 0x90 | 0x90–0x97 |
| SCON | 0x98 | 0x98–0x9F |
| P2 | 0xA0 | 0xA0–0xA7 |
| IE | 0xA8 | 0xA8–0xAF |
| P3 | 0xB0 | 0xB0–0xB7 |
| IP | 0xB8 | 0xB8–0xBF |


