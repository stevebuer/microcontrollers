/**
 * @file    aht20_port_stm32.c
 * @brief   AHT20 platform port for STM32 using the STM32Cube HAL
 *
 */

#include "aht20.h"
#include "stm32f4xx_hal.h"

#define AHT20_I2C_TIMEOUT_MS 100u

static int stm32_i2c_write(void *ctx, uint8_t addr, const uint8_t *data, uint16_t len)
{
	I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)ctx;

	/* HAL wants the 8-bit (already-shifted) address. */

	HAL_StatusTypeDef st = HAL_I2C_Master_Transmit(hi2c, (uint16_t) (addr << 1), (uint8_t *) data, len, AHT20_I2C_TIMEOUT_MS);

	return (st == HAL_OK) ? 0 : -1;
}

static int stm32_i2c_read(void *ctx, uint8_t addr, uint8_t *data, uint16_t len)
{
	I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *) ctx;

	HAL_StatusTypeDef st = HAL_I2C_Master_Receive(hi2c, (uint16_t) (addr << 1), data, len, AHT20_I2C_TIMEOUT_MS);

	return (st == HAL_OK) ? 0 : -1;
}

static void stm32_delay_ms(void *ctx, uint32_t ms)
{
	(void) ctx;

	HAL_Delay(ms);
}

/**
 * @brief Build a platform struct bound to a given I2C handle.
 *
 * Usage:
 *   extern I2C_HandleTypeDef hi2c1;
 *   aht20_t sensor;
 *   aht20_platform_t plat = aht20_port_stm32_create(&hi2c1);
 *   aht20_init(&sensor, &plat);
 */

aht20_platform_t aht20_port_stm32_create(I2C_HandleTypeDef *hi2c)
{

	aht20_platform_t plat = {

		.i2c_write = stm32_i2c_write, 
		.i2c_read = stm32_i2c_read, 
		.delay_ms = stm32_delay_ms, 
		.ctx = hi2c, 
	};

	return plat;
}
