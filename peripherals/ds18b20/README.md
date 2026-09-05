# DS18B20 Dallas 1-Wire Temperature Sensor

Device returns 2 bytes (16 bit value) representing temperature in Celsius in 2's complement.

## Bit Fields

* 5 bits (11-15): sign (all bits redundant sign)
* 7 bits (4-10): integer portion
* 4 bits (0-3): fractional portion (1/16 degree C)
