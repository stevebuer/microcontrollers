# FreeRTOS Configuration for AX.25 Decoder

## Required Files

### FreeRTOS Kernel
Download from: https://www.freertos.org/

Required source files:
```
FreeRTOS/Source/
├── tasks.c
├── queue.c
├── list.c
├── timers.c
├── portable/
│   ├── GCC/ARM_CM3/
│   │   ├── port.c
│   │   └── portmacro.h
│   └── MemMang/
│       └── heap_4.c
└── include/
    ├── FreeRTOS.h
    ├── task.h
    ├── queue.h
    ├── semphr.h
    ├── timers.h
    └── ...
```

### STM32 HAL Drivers
Download from: https://www.st.com/

Or use STM32CubeMX to generate the HAL configuration.

Required HAL files:
```
STM32_HAL/
├── Inc/
│   ├── stm32f1xx_hal.h
│   ├── stm32f1xx_hal_conf.h
│   ├── stm32f1xx_hal_adc.h
│   ├── stm32f1xx_hal_gpio.h
│   ├── stm32f1xx_hal_uart.h
│   └── ...
└── Src/
    ├── stm32f1xx_hal.c
    ├── stm32f1xx_hal_adc.c
    ├── stm32f1xx_hal_gpio.c
    ├── stm32f1xx_hal_uart.c
    └── ...
```

## FreeRTOS Configuration (FreeRTOSConfig.h)

Key settings for AX.25 decoder:

```c
#define configUSE_PREEMPTION              1
#define configUSE_IDLE_HOOK               0
#define configUSE_TICK_HOOK               0
#define configCPU_CLOCK_HZ                72000000
#define configTICK_RATE_HZ                1000
#define configMAX_PRIORITIES              5
#define configMINIMAL_STACK_SIZE          128
#define configTOTAL_HEAP_SIZE             8192
#define configMAX_TASK_NAME_LEN           16
#define configUSE_16_BIT_TICKS            0
#define configUSE_MUTEXES                 1
#define configUSE_COUNTING_SEMAPHORES     1
#define configUSE_QUEUE_SETS              0
#define configUSE_TIME_SLICING            1
```

## Task Configuration

### Task Priorities (highest to lowest)
1. **ADC Sampling** (Priority 3) - Real-time audio capture
2. **Demodulator** (Priority 2) - Process audio samples
3. **Decoder** (Priority 2) - Frame assembly and decoding
4. **Display** (Priority 1) - Output results

### Task Stack Sizes
- ADC Sampling: 256 words
- Demodulator: 512 words
- Decoder: 512 words
- Display: 256 words

### Queue Sizes
- Audio Queue: 4 buffers (128 samples each)
- Bit Queue: 64 bits
- Frame Queue: 2 frames (330 bytes each)

## Memory Budget

Total estimated RAM usage:
- FreeRTOS heap: 8 KB
- Task stacks: ~3 KB
- Queues: ~2 KB
- Global variables: ~2 KB
- **Total: ~15 KB** (fits in 20 KB SRAM)

## Timing Analysis

### ADC Sampling
- Sample rate: 9600 Hz
- Period: 104 μs
- Must be highest priority to avoid missing samples

### Demodulator
- Processes buffers of 128 samples
- Buffer time: 13.3 ms
- CPU usage: ~20-30%

### Decoder
- Processes bits as they arrive
- Variable CPU load based on traffic
- CPU usage: ~10-20%

## Next Steps

1. Download and integrate FreeRTOS
2. Create FreeRTOSConfig.h
3. Download and integrate STM32 HAL
4. Create linker script
5. Generate startup code
6. Implement peripheral initialization
