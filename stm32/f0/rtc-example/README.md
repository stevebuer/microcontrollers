# Onboard Real Time Clock (RTC)

There are two revisions of the RTC peripheral and some variations between actual device capabilities across the families.

* V1 vs. V2

**RM0360** describes the registers and operation for F0.

1. LSI-clocked RTC for approximate elapsed-time / wakeup timer use (no crystal needed)
2. Add an external 32.768 kHz crystal for accurate calendar time, if the board breaks out PC14/PC15


