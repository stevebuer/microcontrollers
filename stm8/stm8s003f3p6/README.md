# STM8 development using SDCC and SPL Port

Several small examples in ASM and C.

## SPL Ports for SDCC

ST Micro SPL is designed for Comisc / Raisonance compilers.

Several ports are avaialble for SDCC:

* [Bruno Schwander Port](https://github.com/bschwand/STM8-SPL-SDCC)
* [Georg Icking-Konert Patch](https://github.com/gicking)

## SDCC

Compiling for STM8

```
sdcc -mstm8 --std-c99 firmware.c
```

SDCC stm8 assmebler is [sdasstm8](https://manpages.debian.org/bookworm/sdcc/sdasstm8.1.en.html)

## Flash Tools

```
stm8flash -c stlinkv2 -p stm8s103 -w firmware.ihx
```
