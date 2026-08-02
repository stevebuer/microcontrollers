# ARM Cortex Debugging

```
sudo apt install gdb-multiarch
```

## Terminal 1

```
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg
```

## Terminal 2

```
gdb-multiarch blinky.elf
(gdb) target remote localhost:3333
(gdb) monitor reset halt
(gdb) continue
```
