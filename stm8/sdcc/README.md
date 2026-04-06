# STM8 development using SDCC and open source tools

Several small examples in ASM and C.

# SPL SDCC Ports

ST Micro SPL is designed for Comisc / Raisonance compilers.

Several ports are avaialble:

* one
* two
* three

# ASM

SDCC stm8 assmebler is [sdasstm8](https://manpages.debian.org/bookworm/sdcc/sdasstm8.1.en.html)

```
steve@kitsap:~/GITHUB/microcontrollers/stm8/sdcc$ sdasstm8

sdas Assembler V02.00 + NoICE + SDCC mods  (STMicroelectronics STM8)

Copyright (C) 2012  Alan R. Baldwin
This program comes with ABSOLUTELY NO WARRANTY.

Usage: [-Options] file
Usage: [-Options] outfile file1 [file2 file3 ...]
  -d   Decimal listing
  -q   Octal   listing
  -x   Hex     listing (default)
  -g   Undefined symbols made global
  -n   Don't resolve global assigned value symbols
  -a   All user symbols made global
  -b   Display .define substitutions in listing
  -bb  and display without .define substitutions
  -c   Disable instruction cycle count in listing
  -j   Enable NoICE Debug Symbols
  -y   Enable SDCC  Debug Symbols
  -l   Create list   file/outfile[.lst]
  -o   Create object file/outfile[.rel]
  -s   Create symbol file/outfile[.sym]
  -p   Disable automatic listing pagination
  -u   Disable .list/.nlist processing
  -w   Wide listing format for symbol table
  -z   Disable case sensitivity for symbols
  -f   Flag relocatable references by  `   in listing file
  -ff  Flag relocatable references by mode in listing file
  -I   Add the named directory to the include file
       search path.  This option may be used more than once.
       Directories are searched in the order given.
```

# C
