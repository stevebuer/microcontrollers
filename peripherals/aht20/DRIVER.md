# AHT20 Driver

A small, portable C driver for the AHT20 I2C temperature/humidity sensor.
The core (`aht20.c` / `aht20.h`) has **no MCU-specific code** — it talks to
the sensor entirely through three function pointers you provide (I2C write,
I2C read, millisecond delay). This means the same driver core runs unmodified
on STM8S, STM32, AVR, or even your desktop for testing; only a thin ~30-line
"port" file changes per platform.

## Layout

```
microcontrollers/peripheral/aht20/
├── aht20.h                    # Public API + platform callback types
├── aht20.c                    # Protocol, CRC8, unit conversion — MCU-agnostic
├── ports/
│   ├── aht20_port_stm32.c     # Example port using STM32Cube HAL
│   └── aht20_port_stm8s.c     # Example port using the STM8S SPL I2C driver
└── tests/
    └── test_aht20.c           # Host-side unit tests with a mocked I2C bus
```

## Design

The driver takes a `aht20_platform_t` of callbacks:

```c
typedef struct {
    aht20_i2c_write_fn i2c_write;
    aht20_i2c_read_fn  i2c_read;
    aht20_delay_ms_fn  delay_ms;
    void *ctx; /* your I2C handle/instance, passed back to each callback */
} aht20_platform_t;
```

You fill this in once per platform (see `ports/`), then the rest of your
application code — init, trigger measurement, read, convert — is identical
regardless of which chip it's running on.

## Usage (STM32 example)

```c
#include "aht20.h"

extern I2C_HandleTypeDef hi2c1; /* configured elsewhere, e.g. via CubeMX */
aht20_platform_t aht20_port_stm32_create(I2C_HandleTypeDef *hi2c); /* from ports/aht20_port_stm32.c */

aht20_t sensor;

void app_init(void)
{
    aht20_platform_t plat = aht20_port_stm32_create(&hi2c1);
    if (aht20_init(&sensor, &plat) != AHT20_OK) {
        /* handle: sensor not present, not calibrated, bus error, etc */
    }
}

void app_loop(void)
{
    float temp_c, hum_pct;
    if (aht20_measure(&sensor, &temp_c, &hum_pct) == AHT20_OK) {
        /* use temp_c / hum_pct */
    }
}
```

Swap `aht20_port_stm32_create` for `aht20_port_stm8s_create` (from
`ports/aht20_port_stm8s.c`) to run the exact same `app_init`/`app_loop` on
STM8S — that's the point of the split.

## API

| Function                | Purpose                                                   |
|--------------------------|-----------------------------------------------------------|
| `aht20_init`             | Send init sequence, confirm calibration bit               |
| `aht20_soft_reset`       | Reset sensor to power-on defaults (re-init required after)|
| `aht20_is_calibrated`    | Read the calibration-loaded status bit                    |
| `aht20_measure`          | Trigger + read, return `float` °C and %RH                 |
| `aht20_read_raw`         | Trigger + read, return raw 20-bit ADC counts (no FPU needed)|

All functions return an `aht20_status_t`: `AHT20_OK`, `AHT20_ERR_IO`,
`AHT20_ERR_TIMEOUT`, `AHT20_ERR_CRC`, `AHT20_ERR_NOT_CALIBRATED`, or
`AHT20_ERR_PARAM`.

Every read is verified with the sensor's CRC8 (poly `0x31`, init `0xFF`)
before conversion, so a corrupted I2C transaction is caught rather than
silently turned into a bogus reading.

## Testing

The `tests/` directory mocks the I2C callbacks so the protocol logic, CRC
check, and unit conversion can be verified on your host machine, no hardware
or cross-compiler needed:

```sh
gcc -std=c99 -Wall -Wextra -I. -o test_aht20 tests/test_aht20.c aht20.c
./test_aht20
```

Expected output:

```
PASS: init sequence + calibration check
PASS: init reports uncalibrated sensor
PASS: measure() converts raw counts to 30.00 C / 50.00 %RH
PASS: measure() rejects a corrupted frame via CRC8

All tests passed.
```

## Porting to a new MCU

Write a `aht20_port_<target>.c` implementing three functions matching
`aht20_i2c_write_fn`, `aht20_i2c_read_fn`, and `aht20_delay_ms_fn`, then
build a `aht20_platform_t` from them (see `ports/` for two examples). The
core driver never needs to change.

## Notes / limitations

- I2C address is fixed at `0x38` (AHT20 doesn't support address selection).
- `aht20_measure` uses `float`; if you're on a core without hardware FPU
  (STM8S included) and want to avoid the software float library, call
  `aht20_read_raw` instead and do fixed-point math in your own code.
- Timing constants (`AHT20_MEASURE_DELAY_MS` etc.) are datasheet minimums
  plus headroom; the driver also polls the busy bit rather than trusting the
  delay alone, so it's tolerant of a slightly slow platform delay function.
