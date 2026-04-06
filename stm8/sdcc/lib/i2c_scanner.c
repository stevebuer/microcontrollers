/*
 * stm8 spl i2c device scanner
 */

#include "stm8s.h"
#include "stm8s_i2c.h"
#include <stdint.h>

static void i2c_wait(void)
{
	while (I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));
}

void i2c_init(void)
{
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);

	I2C_Init(100000, 0x00, I2C_DUTYCYCLE_2,
		I2C_ACK_NONE, I2C_ADDMODE_7BIT, 16);

	I2C_Cmd(ENABLE);
}

uint8_t i2c_probe(uint8_t addr)
{
    	i2c_wait();

    	I2C_GenerateSTART(ENABLE);

    	while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));

    	I2C_Send7bitAddress(addr << 1, I2C_DIRECTION_TRANSMITTER);

    	/* If ACK received → device exists */

	if (I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {

		I2C_GenerateSTOP(ENABLE);
		return 1;
	}

	/* No ACK → clear flags and stop */

	I2C->SR2;
	I2C_GenerateSTOP(ENABLE);

	return 0;
}

void i2c_scan(void)
{
	for (uint8_t addr = 1; addr < 127; addr++) {

		if (i2c_probe(addr)) {
			// Replace with your own print/log function
			// printf("Found device at 0x%02X\n", addr);
        	}
    	}
}
