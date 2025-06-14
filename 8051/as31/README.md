# AS31 Assembler for the 8051 MCU

These programs are written for the as31 assembler on Linux.

## Programming

These were tested on an AT89S52 chip. Programming was done with the Willar Programmer running on Windows 10.

The Avrdude program appparently supports this chip as well. Need to test.

```
avrdude -c usbasp -p at89s52 -e -U flash:w:program.hex
```

