# 8051 Serial Port

Serial Port is controller by five registers: SCON, TMOD, TH1, TCON, PCON.

SBUF is a 1 byte buffer for transmit and receive. It is technically two hardware buffers, with the same address.

Operation of the serial port involves the following steps:

1. Confgure SCON
2. Configure TMOD (timer 1 mode 2 for baud generation)
3. Configure TH1 (timer 1 reload value)
4. Configure TCON (bit 6 on)
5. Configre PCON (optional for double baud rate)
6. Read from and write to SBUF

Baud rate is determined by: 

## SFR: SCON 0x98

This register is bit addressable.

7-6: SM0, SM1 (serial mode select)

* 00: Mode 0 (shift register)
* 01: Mode 1 (8‑bit UART, variable baud)
* 10: Mode 2 (9‑bit UART, fixed baud)
* 11: Mode 3 (9‑bit UART, variable baud)

5: SM2 — Multiprocessor communication enable
4: REN — Receive enable
3: TBB — Transmit bit 8 (Modes 2/3)
2: RB8 — Received bit 8 (Modes 2/3)
1: TI — Transmit interrupt flag
0: RI — Receive interrupt flag

Typical configuration: 0x50

* SM0/SM1: 01 (mode 1)
* REN = 1 (receive enable)
* TI/RI = 0 (starting condition)

## SFR: TMOD 0x89

## SFR: TH1 0x8D

## SFR: TCON 0x88

## SFR: PCON 0x87

## SFR: SBUF 0x99

### ASM 

Serial Control

```
; config register as a byte
MOV SCON, #01010000B
; or configure register by bits
CLR SM0
SETB SM1
SETB REN
CLR TI
CLR RI
```

Timer Mode

```
; T1 mode 2
MOV TMOD, #89H
; or configure bit by bit
SETB 
```

Timer reload value

```
; T1 reload value
MOV TH1, #11111101B
```
