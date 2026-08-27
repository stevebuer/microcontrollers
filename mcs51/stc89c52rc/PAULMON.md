# STC89 Microcontroller

Small development board. STC89C52RC 40 pin DIP.

* 8K code flash rom
* 6K flash EEPROM

## Development Board

Minimal development board from eBay. It came with a 12 Mhz crystal -- higher baud rates give errors. 2400 baud ok.

Installed paulmon with no errors.

```
steve@kitsap:~/STCGAL$ myvenv/bin/stcgal -P stc89 -b 2400 -p /dev/ttyUSB0 paulmon21.hex 
```

<img src="stc89_board.jpg">

## Programming

<a href="https://github.com/grigorig/stcgal">stcgal</a>

<pre>
steve@kitsap:~/STCGAL$ myvenv/bin/stcgal 
Waiting for MCU, please cycle power: done
Protocol detected: stc89
Target model:
  Name: STC89C52RC/LE52RC
  Magic: F002
  Code flash: 8.0 KB
  EEPROM flash: 6.0 KB
Target frequency: 11.907 MHz
Target BSL version: 3.2C
Target options:
  cpu_6t_enabled=False
  bsl_pindetect_enabled=False
  eeprom_erase_enabled=False
  clock_gain=high
  ale_enabled=True
  xram_enabled=True
  watchdog_por_enabled=False
Disconnected!
</pre>

## References

* [stc89c52rc datasheet](https://www.stcmicro.com/datasheet/STC89C51RC-en.pdf)
* [stc89c52 tutorial](https://www.reidemeister.com/?p=395)
