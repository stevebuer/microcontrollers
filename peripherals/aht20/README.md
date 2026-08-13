# AHT20 Temperature & Humidity Sensor

MCU driver for [Asair i2C sensor](https://asairsensors.com/product/aht20-integrated-temperature-and-humidity-sensor).

The AHT20 reports 20 bits each for temperature and relative humidity measurements.

Temperature: -40 ℃ to +85 ℃ with ±0.3 ℃ accuracy and 0.01 ℃ resolution.

Rel. Humidity: 0 to 100% RH with ±2% accuracy and 0.024 resolution. 

## I2C Interface

I2C Fixed Address: 0x38

### Commands

| Command | Byte | Parameters | Notes |
|---|---|---|---|
| Initialize | `0xBE` | `0x08`, `0x00` | Wait 10ms after issuing. Only needed if the calibration bit (status Bit[3]) is not set at power-on. |
| Trigger Measurement | `0xAC` | `0x33`, `0x00` | Wait 80ms for the measurement to complete. |
| Soft Reset | `0xBA` | none | Restores default settings, completes in no more than 20ms. No return value. |
| Read Status | `0x71` | — | Single-byte read of the status register |

### Status Byte

| Bit | Meaning |
|---|---|
| 7 | BUSY (1 = measuring, 0 = idle) |
| 6:5 | MODE (00 = normal, 01 = cycle, 10/11 = command) |
| 3 | CAL — calibration enabled (1 = calibrated) |
| others | reserved |

### Power-Up / Read Sequence

1. Wait 40ms after power-on.
2. Read status (`0x71`); if calibration bit (Bit 3) is not 1, send Init command (`0xBE 0x08 0x00`) and wait 10ms.
3. Send Trigger Measurement (`0xAC 0x33 0x00`).
4. Wait 80ms, then poll status — Bit 7 (BUSY) must read 0 before continuing.
5. Read 6 data bytes (1 status + 2.5 bytes humidity + 2.5 bytes temperature, packed), followed by an optional 7th CRC byte.

### Data Format (6-byte read)

- Byte 0: status
- Bytes 1–2 + upper nibble of byte 3: 20-bit raw humidity
- Lower nibble of byte 3 + bytes 4–5: 20-bit raw temperature
- Byte 6 (optional): CRC-8 checksum

```
RH(%)   = (raw_humidity / 2^20) * 100
Temp(°C) = (raw_temperature / 2^20) * 200 - 50
```

## References

* [Asair AHT20 Data Sheet](https://asairsensors.com/wp-content/uploads/2021/09/Data-Sheet-AHT20-Humidity-and-Temperature-Sensor-ASAIR-V1.0.03.pdf)
