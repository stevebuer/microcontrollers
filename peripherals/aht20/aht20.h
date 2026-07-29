/**
 * @file    aht20.h
 * @brief   Driver for the AHT20 I2C temperature/humidity sensor
 *
 * Datasheet reference: AHT20 (ASAIR), I2C address 0x38, 7-bit.
 */

#ifndef AHT20_H
#define AHT20_H

#include <stdint.h>
#include <stdbool.h>

/* 7-bit I2C address of the AHT20 (fixed) */

#define AHT20_I2C_ADDR 0x38u

typedef enum {

	AHT20_OK = 0,
	AHT20_ERR_IO,              /*  I2C write/read callback returned an error */
	AHT20_ERR_TIMEOUT,         /* Sensor never cleared the busy bit in time */
	AHT20_ERR_CRC,             /* CRC8 check on the read data failed */
	AHT20_ERR_NOT_CALIBRATED,  /* Sensor reports it has not loaded its cal bit */
	AHT20_ERR_PARAM            /* Bad argument (NULL pointer, etc) */

} aht20_status_t;

/**
 * @brief I2C write callback.
 * @param addr  7-bit device address (always AHT20_I2C_ADDR in practice)
 * @param data  Bytes to write
 * @param len   Number of bytes to write
 * @return 0 on success, nonzero on failure
 */
typedef int (*aht20_i2c_write_fn)(uint8_t addr, const uint8_t *data, uint16_t len);

/**
 * @brief I2C read callback.
 * @param addr  7-bit device address (always AHT20_I2C_ADDR in practice)
 * @param data  Buffer to read into
 * @param len   Number of bytes to read
 * @return 0 on success, nonzero on failure
 */
typedef int (*aht20_i2c_read_fn)(uint8_t addr, uint8_t *data, uint16_t len);

/**
 * @brief Blocking millisecond delay callback.
 * @param ms    Milliseconds to delay
 */
typedef void (*aht20_delay_ms_fn)(uint32_t ms);

/** Set of platform callbacks the driver core uses. Fill this in once per MCU. */

typedef struct {

	aht20_i2c_write_fn i2c_write;
	aht20_i2c_read_fn i2c_read;
	aht20_i2c_delay_fn i2c_delay;
	void *ctx;
	bool initialized;

} aht20_handle_t;

/**
 * @brief Initialize the driver instance and the sensor.
 *
 * Validates the platform callbacks, sends the AHT20 init sequence, and waits
 * for the sensor to report calibrated. Call once at startup, after the
 * platform I2C peripheral itself is already up and running.
 */
aht20_status_t aht20_init(const aht20_handle_t *h);

/**
 * @brief Soft-reset the sensor (equivalent to power-on reset).
 *
 * Sensor needs re-initializing (aht20_init) after this returns.
 */
aht20_status_t aht20_soft_reset(aht20_handle_t *h);

/** 
 * @brief Read the sensor's calibration-loaded status bit. i
 */
aht20_status_t aht20_is_calibrated(aht20_handle_t *h, bool *calibrated);

/**
 * @brief Trigger a measurement and return converted temperature/humidity.
 * @param temperature_c  Output, degrees Celsius. Pass NULL to skip.
 * @param humidity_pct   Output, %RH. Pass NULL to skip.
 */
aht20_status_t aht20_measure(aht20_handle_t *h, float *temperature_c, float *humidity_pct);

/**
 * @brief Trigger a measurement and return the raw 20-bit ADC counts.
 *
 * Useful if you want to do the fixed-point conversion yourself (e.g. on a
 * part with no FPU) instead of using the float math in aht20_measure().
 */
aht20_status_t aht20_read_raw(aht20_handle_t *dev, uint32_t *raw_humidity, uint32_t *raw_temperature);

#endif /* AHT20_H */
