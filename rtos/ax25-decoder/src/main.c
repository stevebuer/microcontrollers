/**
 * @file main.c
 * @brief AX.25 Decoder - Main Application
 * @author Steve
 * @date 2025
 * 
 * Main application entry point for AX.25 packet decoder using FreeRTOS
 * on STM32F103C8T6 (Blue Pill) microcontroller.
 */

#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "ax25_decoder.h"
#include "afsk_demod.h"
#include "hdlc.h"

/* Task priorities */
#define PRIORITY_ADC_SAMPLING   (tskIDLE_PRIORITY + 3)
#define PRIORITY_DEMODULATOR    (tskIDLE_PRIORITY + 2)
#define PRIORITY_DECODER        (tskIDLE_PRIORITY + 2)
#define PRIORITY_DISPLAY        (tskIDLE_PRIORITY + 1)

/* Task stack sizes */
#define STACK_SIZE_ADC          256
#define STACK_SIZE_DEMOD        512
#define STACK_SIZE_DECODER      512
#define STACK_SIZE_DISPLAY      256

/* Queue sizes */
#define AUDIO_QUEUE_SIZE        4
#define BIT_QUEUE_SIZE          64
#define FRAME_QUEUE_SIZE        2

/* Task handles */
TaskHandle_t xAdcTaskHandle = NULL;
TaskHandle_t xDemodTaskHandle = NULL;
TaskHandle_t xDecoderTaskHandle = NULL;
TaskHandle_t xDisplayTaskHandle = NULL;

/* Queue handles */
QueueHandle_t xAudioQueue = NULL;
QueueHandle_t xBitQueue = NULL;
QueueHandle_t xFrameQueue = NULL;

/* Semaphore for ADC */
SemaphoreHandle_t xAdcSemaphore = NULL;

/* Function prototypes */
static void SystemClock_Config(void);
static void GPIO_Init(void);
static void ADC_Init(void);
static void UART_Init(void);

/* Task function prototypes */
void vAdcSamplingTask(void *pvParameters);
void vDemodulatorTask(void *pvParameters);
void vDecoderTask(void *pvParameters);
void vDisplayTask(void *pvParameters);

/**
 * @brief Main entry point
 */
int main(void)
{
    /* Reset of all peripherals, Initializes Flash and Systick */
    HAL_Init();
    
    /* Configure the system clock to 72 MHz */
    SystemClock_Config();
    
    /* Initialize peripherals */
    GPIO_Init();
    ADC_Init();
    UART_Init();
    
    /* Create queues */
    xAudioQueue = xQueueCreate(AUDIO_QUEUE_SIZE, sizeof(AudioBuffer_t));
    xBitQueue = xQueueCreate(BIT_QUEUE_SIZE, sizeof(uint8_t));
    xFrameQueue = xQueueCreate(FRAME_QUEUE_SIZE, sizeof(AX25Frame_t));
    
    /* Create semaphores */
    xAdcSemaphore = xSemaphoreCreateBinary();
    
    /* Verify queue and semaphore creation */
    if (xAudioQueue == NULL || xBitQueue == NULL || 
        xFrameQueue == NULL || xAdcSemaphore == NULL)
    {
        /* Creation failed - halt */
        while(1);
    }
    
    /* Create tasks */
    xTaskCreate(vAdcSamplingTask,
                "ADC",
                STACK_SIZE_ADC,
                NULL,
                PRIORITY_ADC_SAMPLING,
                &xAdcTaskHandle);
                
    xTaskCreate(vDemodulatorTask,
                "DEMOD",
                STACK_SIZE_DEMOD,
                NULL,
                PRIORITY_DEMODULATOR,
                &xDemodTaskHandle);
                
    xTaskCreate(vDecoderTask,
                "DECODE",
                STACK_SIZE_DECODER,
                NULL,
                PRIORITY_DECODER,
                &xDecoderTaskHandle);
                
    xTaskCreate(vDisplayTask,
                "DISPLAY",
                STACK_SIZE_DISPLAY,
                NULL,
                PRIORITY_DISPLAY,
                &xDisplayTaskHandle);
    
    /* Start the scheduler */
    vTaskStartScheduler();
    
    /* Should never reach here */
    while(1)
    {
        /* Error - scheduler failed to start */
    }
}

/**
 * @brief ADC Sampling Task
 * Continuously samples audio input and fills audio buffers
 */
void vAdcSamplingTask(void *pvParameters)
{
    AudioBuffer_t audioBuffer;
    uint32_t sampleIndex = 0;
    
    for(;;)
    {
        /* Wait for ADC conversion complete */
        if (xSemaphoreTake(xAdcSemaphore, portMAX_DELAY) == pdTRUE)
        {
            /* Read ADC value */
            audioBuffer.samples[sampleIndex] = HAL_ADC_GetValue(&hadc1);
            sampleIndex++;
            
            /* When buffer is full, send to demodulator */
            if (sampleIndex >= AUDIO_BUFFER_SIZE)
            {
                audioBuffer.length = sampleIndex;
                audioBuffer.timestamp = xTaskGetTickCount();
                
                /* Send to demodulator (non-blocking) */
                xQueueSend(xAudioQueue, &audioBuffer, 0);
                
                sampleIndex = 0;
            }
            
            /* Start next conversion */
            HAL_ADC_Start_IT(&hadc1);
        }
    }
}

/**
 * @brief AFSK Demodulator Task
 * Demodulates Bell 202 AFSK to extract digital bits
 */
void vDemodulatorTask(void *pvParameters)
{
    AudioBuffer_t audioBuffer;
    uint8_t bit;
    
    /* Initialize demodulator */
    AFSK_Init();
    
    for(;;)
    {
        /* Wait for audio buffer */
        if (xQueueReceive(xAudioQueue, &audioBuffer, portMAX_DELAY) == pdTRUE)
        {
            /* Process audio buffer and extract bits */
            for (uint32_t i = 0; i < audioBuffer.length; i++)
            {
                if (AFSK_ProcessSample(audioBuffer.samples[i], &bit))
                {
                    /* Bit is ready - send to decoder */
                    xQueueSend(xBitQueue, &bit, portMAX_DELAY);
                }
            }
        }
    }
}

/**
 * @brief Frame Decoder Task
 * Assembles bits into HDLC frames and decodes AX.25 packets
 */
void vDecoderTask(void *pvParameters)
{
    uint8_t bit;
    AX25Frame_t frame;
    
    /* Initialize HDLC decoder */
    HDLC_Init();
    
    for(;;)
    {
        /* Wait for bit */
        if (xQueueReceive(xBitQueue, &bit, portMAX_DELAY) == pdTRUE)
        {
            /* Process bit through HDLC decoder */
            if (HDLC_ProcessBit(bit, &frame))
            {
                /* Complete frame received - decode AX.25 */
                if (AX25_DecodeFrame(&frame))
                {
                    /* Valid frame - send to display */
                    xQueueSend(xFrameQueue, &frame, portMAX_DELAY);
                }
            }
        }
    }
}

/**
 * @brief Display Task
 * Outputs decoded packets to UART/display
 */
void vDisplayTask(void *pvParameters)
{
    AX25Frame_t frame;
    char displayBuffer[256];
    
    for(;;)
    {
        /* Wait for decoded frame */
        if (xQueueReceive(xFrameQueue, &frame, portMAX_DELAY) == pdTRUE)
        {
            /* Format frame for display */
            AX25_FormatFrame(&frame, displayBuffer, sizeof(displayBuffer));
            
            /* Output to UART */
            HAL_UART_Transmit(&huart1, (uint8_t*)displayBuffer, 
                             strlen(displayBuffer), HAL_MAX_DELAY);
            
            /* TODO: Output to OLED display if available */
        }
    }
}

/**
 * @brief System Clock Configuration
 * Configure system clock to 72 MHz
 */
static void SystemClock_Config(void)
{
    /* TODO: Implement clock configuration */
    /* HSE -> PLL -> 72 MHz */
}

/**
 * @brief GPIO Initialization
 */
static void GPIO_Init(void)
{
    /* TODO: Initialize GPIO pins */
    /* LED on PC13 for status */
    /* ADC input on PA0 */
}

/**
 * @brief ADC Initialization
 */
static void ADC_Init(void)
{
    /* TODO: Configure ADC1 */
    /* Single channel, continuous conversion */
    /* Sample rate: 9600 Hz or higher */
    /* DMA for efficient transfer */
}

/**
 * @brief UART Initialization
 */
static void UART_Init(void)
{
    /* TODO: Configure UART1 */
    /* 115200 baud, 8N1 */
    /* TX on PA9, RX on PA10 */
}

/**
 * @brief ADC conversion complete callback
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    /* Give semaphore from ISR */
    xSemaphoreGiveFromISR(xAdcSemaphore, &xHigherPriorityTaskWoken);
    
    /* Yield if needed */
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/**
 * @brief Error handler
 */
void Error_Handler(void)
{
    /* Disable interrupts */
    __disable_irq();
    
    /* Infinite loop */
    while(1)
    {
        /* TODO: Blink LED rapidly to indicate error */
    }
}
