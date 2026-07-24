#include "app_uart.h"
#include "stm32f0xx_ll_usart.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_bus.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define APP_UART_INSTANCE   USART1

void AppUart_Init(void)
{
    // GPIO: PA9 = TX, PA10 = RX, AF1 on F0
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin        = LL_GPIO_PIN_9 | LL_GPIO_PIN_10;
    GPIO_InitStruct.Mode       = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Alternate  = LL_GPIO_AF_1;
    GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull       = LL_GPIO_PULL_UP;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_USART1);

    LL_USART_InitTypeDef USART_InitStruct = {0};
    USART_InitStruct.BaudRate            = 115200;
    USART_InitStruct.DataWidth           = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits            = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity              = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection   = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling        = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(APP_UART_INSTANCE, &USART_InitStruct);

    LL_USART_Enable(APP_UART_INSTANCE);
}

static void AppUart_SendByte(uint8_t byte)
{
    uint32_t timeout = 100000;
    while (!LL_USART_IsActiveFlag_TXE(APP_UART_INSTANCE))
    {
        if (--timeout == 0) return;
    }
    LL_USART_TransmitData8(APP_UART_INSTANCE, byte);
}

void AppUart_SendString(const char *str)
{
    while (*str)
    {
        AppUart_SendByte((uint8_t)*str++);
    }
}

void AppUart_Printf(const char *fmt, ...)
{
    char buf[128];
    va_list args;

    va_start(args, fmt);
    int len = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    if (len > 0)
    {
        if ((size_t)len >= sizeof(buf))
            len = sizeof(buf) - 1;

        for (int i = 0; i < len; i++)
        {
            AppUart_SendByte((uint8_t)buf[i]);
        }
    }
}
