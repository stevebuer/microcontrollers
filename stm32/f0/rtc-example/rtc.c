#include "stm32f0xx_hal.h"

RTC_HandleTypeDef hrtc;

/**
 * @brief  Initializes RTC using external 32.768kHz LSE crystal on PC14/PC15.
 * @retval HAL_OK if LSE started and RTC initialized successfully,
 *         HAL_ERROR if LSE failed to start (falls back to LSI) or RTC init failed.
 */
HAL_StatusTypeDef RTC_Init_LSE(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /* Enable access to the RTC/backup domain registers */
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();

    /* Configure and start LSE */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.LSEState       = RCC_LSE_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE; // don't touch PLL here

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        /* LSE failed to start (e.g. bad crystal/layout) — fall back to LSI */
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
        RCC_OscInitStruct.LSEState       = RCC_LSE_OFF;
        RCC_OscInitStruct.LSIState       = RCC_LSI_ON;
        if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
        {
            return HAL_ERROR;
        }

        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
        PeriphClkInit.RTCClockSelection    = RCC_RTCCLKSOURCE_LSI;
    }
    else
    {
        /* LSE started fine — use it as RTC source */
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
        PeriphClkInit.RTCClockSelection    = RCC_RTCCLKSOURCE_LSE;
    }

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        return HAL_ERROR;
    }

    /* Enable RTC clock */
    __HAL_RCC_RTC_ENABLE();

    /* Init the RTC peripheral: 32.768kHz -> 1Hz via 128 * 256 prescalers */
    hrtc.Instance            = RTC;
    hrtc.Init.HourFormat     = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv   = 127;
    hrtc.Init.SynchPrediv    = 255;
    hrtc.Init.OutPut         = RTC_OUTPUT_DISABLE;

    if (HAL_RTC_Init(&hrtc) != HAL_OK)
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) != 0xA5A5)
{
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};

    sTime.Hours = 12;
    sTime.Minutes = 0;
    sTime.Seconds = 0;
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

    sDate.WeekDay = RTC_WEEKDAY_MONDAY;
    sDate.Month   = RTC_MONTH_JANUARY;
    sDate.Date    = 1;
    sDate.Year    = 26;
    HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 0xA5A5);
}
