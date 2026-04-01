# SDCC Programming Templates and Notes

## sbit

```
__sbit __at (ADDRESS) NAME;
```

Port bits

```
__sbit __at (0x90) P1_0;   // Port 1 bit 0
__sbit __at (0x91) P1_1;
__sbit __at (0x92) P1_2;
```

SFR bits

```
__sbit __at (0xAF) EA;     // IE.7 – global interrupt enable
__sbit __at (0x88) TF0;    // TCON.5 – timer 0 overflow
__sbit __at (0x89) TR0;    // TCON.4 – timer 0 run control
```
