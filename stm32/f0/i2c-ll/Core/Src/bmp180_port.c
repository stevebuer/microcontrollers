#include "bmp180_port.h"
#include "stm32f0xx_ll_i2c.h"
#include "stm32f0xx_ll_utils.h"   // for LL_mDelay

/* -------- bus I/O, matching Bosch's expected signatures -------- */

s8 BMP180_I2C_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
    uint32_t timeout;

    // --- Write phase: send register address, no stop yet ---
    LL_I2C_HandleTransfer(I2C1, dev_addr, LL_I2C_ADDRSLAVE_7BIT,
                           1 + cnt, LL_I2C_MODE_AUTOEND,
                           LL_I2C_GENERATE_START_WRITE);

    timeout = 100000;
    while (!LL_I2C_IsActiveFlag_TXIS(I2C1)) { if (--timeout == 0) return -1; }
    LL_I2C_TransmitData8(I2C1, reg_addr);

    for (u8 i = 0; i < cnt; i++)
    {
        timeout = 100000;
        while (!LL_I2C_IsActiveFlag_TXIS(I2C1)) { if (--timeout == 0) return -1; }
        LL_I2C_TransmitData8(I2C1, reg_data[i]);
    }

    timeout = 100000;
    while (!LL_I2C_IsActiveFlag_STOP(I2C1)) { if (--timeout == 0) return -1; }
    LL_I2C_ClearFlag_STOP(I2C1);

    return 0;
}

s8 BMP180_I2C_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
    uint32_t timeout;

    // --- Write phase: send register address, keep bus (softend, no stop) ---
    LL_I2C_HandleTransfer(I2C1, dev_addr, LL_I2C_ADDRSLAVE_7BIT,
                           1, LL_I2C_MODE_SOFTEND,
                           LL_I2C_GENERATE_START_WRITE);

    timeout = 100000;
    while (!LL_I2C_IsActiveFlag_TXIS(I2C1)) { if (--timeout == 0) return -1; }
    LL_I2C_TransmitData8(I2C1, reg_addr);

    timeout = 100000;
    while (!LL_I2C_IsActiveFlag_TC(I2C1)) { if (--timeout == 0) return -1; }

    // --- Read phase: repeated start, read cnt bytes, autoend generates stop ---
    LL_I2C_HandleTransfer(I2C1, dev_addr, LL_I2C_ADDRSLAVE_7BIT,
                           cnt, LL_I2C_MODE_AUTOEND,
                           LL_I2C_GENERATE_START_READ);

    for (u8 i = 0; i < cnt; i++)
    {
        timeout = 100000;
        while (!LL_I2C_IsActiveFlag_RXNE(I2C1)) { if (--timeout == 0) return -1; }
        reg_data[i] = LL_I2C_ReceiveData8(I2C1);
    }

    timeout = 100000;
    while (!LL_I2C_IsActiveFlag_STOP(I2C1)) { if (--timeout == 0) return -1; }
    LL_I2C_ClearFlag_STOP(I2C1);

    return 0;
}

void BMP180_delay_msek(u32 msek)
{
    LL_mDelay(msek);
}

/* -------- struct wiring + init -------- */

s8 BMP180_PortInit(struct bmp180_t *bmp180)
{
    bmp180->bus_write  = BMP180_I2C_bus_write;
    bmp180->bus_read   = BMP180_I2C_bus_read;
    bmp180->dev_addr   = BMP180_I2C_ADDR;   // Bosch's header defines this as 0x77
    bmp180->delay_msec = BMP180_delay_msek;

    return bmp180_init(bmp180);
}
