# Steve's minimal Cortex M0 startup code

## vector_table.c 

Cortex M0 vector table in C.

## startup_reset.S

SP = vector_table[0]
PC = vector_table[1]

## crt0

Choose one of: start_asm.S and start_c.c

## system.c

* clock configuration
* vector table relocation
* enabling the PLL
* flash latency setup
* system tick setup
* low‑power mode helpers
* board‑specific initialization
