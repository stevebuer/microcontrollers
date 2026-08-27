# 8051 and related microcontrollers

Also known as MCS-51, an 8-bit Harvard architecture microcontroller with [255 instructions](https://aeb.win.tue.nl/comp/8051/set8051.html), 
and 16-bit memory addressing. 128 bytes internal RAM, varying amounts of internal ROM. Supports External RAM and ROM.

## Devices

* AT89C52
* [AT89S52](at89s52/README.md)
* [C8051F340](c8051f/README.md)
* CH552

## Instruction Set

The 8051 has 49 instruction mnemonics in 5 categories:

* Data Transfer: e.g., MOV, PUSH, POP, XCH
* Arithmetic: e.g., ADD, SUBB, MUL, DIV, INC, DEC
* Logical: e.g., ANL, ORL, XRL, CLR, CPL
* Boolean (Bit Manipulation): e.g., SETB, CLR, CPL, ANL, ORL
* Program Branching: e.g., SJMP, LJMP, JC, JNC, CALL, RET

## Memory Map

Program Memory (ROM) and Data Memory (RAM) are in separate 64k address spaces.

**ROM**

Can be accessed with [MOVC](https://aeb.win.tue.nl/comp/8051/set8051.html#51movc) instruction.

$0000 - $0FFF (0 - 4095) is 4k ROM<br>
$1000 - $1FFF (4096 - 8191) is 8k ROM

$1000 - $FFFF (4096 - 65535) for external chips.

Alternatively any amount of ROM/EEPROM up to 64K can be connected externally in the full $0 - $FFFF.

Code can only execute from internal or external, chosen at start time. However, MOVC can
access data on external ROM such as lookup tables.

**RAM**

Can be accessed with the [MOVX](https://aeb.win.tue.nl/comp/8051/set8051.html#51movx) instruction.

$00 - $7F (0 - 127) zero page - 128 bytes<br>
$0080 - $00FF (128 - 255) Special Function Registers (SFR) & more RAM in 8052

Any amount of RAM up to 64 KB can be connected externally.

Compilers use small (internal RAM), large (external RAM), and tiny memory models.

## Development tools

* [Metalink Assembler](http://www.om3bc.com/datasheets/ASM51MAN.pdf)
* [AS31 Assembler](https://www.pjrc.com/tech/8051/tools/as31-doc.html)
* [SDCC](https://sdcc.sourceforge.net)
* [Keil](https://www.keil.com/c51/default.asp)
* [CamelForth](https://www.camelforth.com/news.php)

## References

* [8051 Timer Tricks](https://embeddedflakes.com/8051-timer-mode-tricks)
* [8051 Instruction Set](https://aeb.win.tue.nl/comp/8051/set8051.html)
