# STM32F0 CMSIS / LL Template

Template for F0 with CMSIS / LL

## ASM sources

.s raw assembly
.S assembly with c-preprocessor

## Directory structure

<pre>
project/
│
├── Core/
│   ├── Inc/
│   │   └── stm32f0xx.h
│   │   └── stm32f030x6.h
│   │   └── system_stm32f0xx.h
│   │
│   └── Src/
│       └── system_stm32f0xx.c
│       └── startup_stm32f030x6.s
│
├── User/
│   ├── main.c
│   └── your_drivers.c/.h (optional)
│
└── Makefile
</pre>
