#include "bmp180_port.h"
#include "app_uart.h"
#include "stm32f0xx_ll_utils.h"

extern struct bmp180_t bmp180;

void TempReadLoop_Run(void)
{
    while (1)
    {
        u16 ut = bmp180_get_uncomp_temperature();
        s16 temperature = bmp180_get_temperature(ut);

        AppUart_Printf("Temp: %d.%d C\r\n", temperature / 10, temperature % 10);

        LL_mDelay(60000);
    }
}
