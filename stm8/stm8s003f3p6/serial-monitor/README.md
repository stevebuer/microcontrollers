# STM8 Serial Monitor

Simple command monitor over UART for STM8S003 with I2C utilities and a tiny stack-based command set.

## Command Reference

All numeric values for I2C commands are hexadecimal bytes without `0x`.

| Command | Description |
|---|---|
| `h` or `?` | Show help |
| `i` | Scan I2C bus (`0x01`..`0x7E`) |
| `r <addr> <reg>` | Read one byte from I2C register |
| `w <addr> <reg> <val>` | Write one byte to I2C register |
| `ow ...` | Dallas 1-Wire placeholder command group |
| `c` | Clear stack |
| `s` | Show stack |
| `+` | Add top two stack values (`n n -- n`) |
| `d <n>` | Push decimal number |
| `x <hex>` | Push hex number (no `0x`) |

## I2C Command Notes

- `addr` is the 7-bit I2C address (example: BMP180 is `77`).
- `reg` and `val` are one-byte hex values.
- Read/write commands return:
  - `ERR: i2c no-ack` when no device acknowledges the address.
  - `ERR: i2c timeout` on bus or transaction timeout.

## OneWire Placeholder Notes

- `ow` currently provides placeholders for next-session work:
  - `ow help`
  - `ow scan`
  - `ow readrom`
  - `ow reset`
- These are intentionally not implemented yet and print placeholder text.

## Example Session

<pre>
micro-mon: v0.3
  h|?          help
  i            scan i2c bus
  r a r        read i2c register (hex bytes)
  w a r v      write i2c register (hex bytes)
  ow ...       dallas 1-wire placeholders
  c            clear stack
  s            show stack
  +            add top two (n n -- n)
  d <n>        push decimal number
  x <hex>      push hex number (no 0x)
cmd> i
I2C scan:
  found: 0x38
  found: 0x77
cmd> r 77 d0
  0x55
cmd> w 38 00 10
ok
cmd>
</pre>
