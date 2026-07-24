/* Cortex M0 GPIO driver */

#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

/* GPIO register layout */

typedef struct {
    volatile uint32_t MODER;    // mode register
    volatile uint32_t OTYPER;   // output type
    volatile uint32_t OSPEEDR;  // output speed
    volatile uint32_t PUPDR;    // pull-up/pull-down
    volatile uint32_t IDR;      // input data
    volatile uint32_t ODR;      // output data
    volatile uint32_t BSRR;     // bit set/reset
    volatile uint32_t LCKR;     // config lock
    volatile uint32_t AFRL;     // alt function low
    volatile uint32_t AFRH;     // alt function high
    volatile uint32_t BRR;      // bit reset
} GPIO_t;

/* Base addresses for GPIO ports */

#define GPIOA ((GPIO_t*)0x48000000)
#define GPIOB ((GPIO_t*)0x48000400)
#define GPIOC ((GPIO_t*)0x48000800)
#define GPIOD ((GPIO_t*)0x48000C00)
#define GPIOF ((GPIO_t*)0x48001400)

/* Reset and Clock Control (RCC) register for enabling GPIO clocks */

#define RCC_AHBENR (*(volatile uint32_t*)0x40021014)

/* GPIO configuration enums */

typedef enum {
    GPIO_INPUT  = 0,
    GPIO_OUTPUT = 1,
    GPIO_AF     = 2,
    GPIO_ANALOG = 3
} GPIO_mode_t;

typedef enum {
    GPIO_NOPULL = 0,
    GPIO_PULLUP = 1,
    GPIO_PULLDOWN = 2
} GPIO_pull_t;

/* API */

void gpio_enable(GPIO_t *port);
void gpio_mode(GPIO_t *port, uint8_t pin, GPIO_mode_t mode);
void gpio_pull(GPIO_t *port, uint8_t pin, GPIO_pull_t pull);
void gpio_write(GPIO_t *port, uint8_t pin, uint8_t value);
uint8_t gpio_read(GPIO_t *port, uint8_t pin);

#endif
