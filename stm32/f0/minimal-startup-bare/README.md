# SPL Template

Bare Metal Template for F0 with SPL

## ASM files

.s raw assembly (no macros, #include, #define, etc.)
.S assembly with c-preprocessor

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
└── Makefile / CMakeLists.txt / project file
</pre>
