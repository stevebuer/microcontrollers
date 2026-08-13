/**
 * @file    aht20_port_stm8s.c
 * @brief   AHT20 platform port for STM8S using the STM8S Standard
 *          Peripheral Library (SPL) I2C driver.
 *
 * Assumes:
 * 
 *   1. I2C1 has already been configured and enabled 
 *   2. Software millisecond tick for delay_ms -- swap `stm8s_delay_ms` for systick.
 */

#include "aht20.h"
#include "stm8s_conf.h"

/* wait limit for bus timeout */

#define AHT20_I2C_TIMEOUT_LOOPS 20000u 

/* global systick: fixme not atomic on stm8 */

extern volatile uint32_t g_ms_ticks;

static int stm8s_i2c_write(uint8_t addr, const uint8_t *data, uint16_t len)
{
	uint32_t guard;

	guard = AHT20_I2C_TIMEOUT_LOOPS;

	I2C_GenerateSTART(ENABLE);

	while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
		if (--guard == 0u)
			return -1;

	guard = AHT20_I2C_TIMEOUT_LOOPS;

	I2C_Send7bitAddress((uint8_t)(addr << 1), I2C_DIRECTION_TX);

	while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		if (--guard == 0u)
			return -1;

	for (uint16_t i = 0; i < len; i++) {

		guard = AHT20_I2C_TIMEOUT_LOOPS;
		
		I2C_SendData(data[i]);

		while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
			if (--guard == 0u)
				return -1;
	}

	I2C_GenerateSTOP(ENABLE);

	return 0;
}

static int stm8s_i2c_read(uint8_t addr, uint8_t *data, uint16_t len)
{
	uint32_t guard;

	guard = AHT20_I2C_TIMEOUT_LOOPS;

	I2C_GenerateSTART(ENABLE);

	while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
		if (--guard == 0u)
			return -1;

	guard = AHT20_I2C_TIMEOUT_LOOPS;

	I2C_Send7bitAddress((uint8_t)(addr << 1), I2C_DIRECTION_RX);

	while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		if (--guard == 0u)
			return -1;

	for (uint16_t i = 0; i < len; i++) {

        	if (i == (uint16_t) (len - 1)) {

            		I2C_AcknowledgeConfig(I2C_ACK_NONE);
            		I2C_GenerateSTOP(ENABLE);
        	} else
            		I2C_AcknowledgeConfig(I2C_ACK_CURR);

		guard = AHT20_I2C_TIMEOUT_LOOPS;

		while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))
			if (--guard == 0u)
				return -1;
        
		data[i] = I2C_ReceiveData();
    	}
	
	/* restore default ACK state for next transfer */

	I2C_AcknowledgeConfig(I2C_ACK_CURR); 

	return 0;
}

static void stm8s_delay_ms(uint32_t ms)
{
	uint32_t target = g_ms_ticks + ms;

	while (g_ms_ticks < target);
}
