#ifndef I2C_BUS_H
#define I2C_BUS_H

#define I2C_PROBE_NONE    0
#define I2C_PROBE_FOUND   1
#define I2C_PROBE_TIMEOUT 2

void i2c_bus_init(void);
void i2c_bus_scan(void);
unsigned char i2c_bus_read_reg(unsigned char addr, unsigned char reg, unsigned char* value);
unsigned char i2c_bus_write_reg(unsigned char addr, unsigned char reg, unsigned char value);

#endif