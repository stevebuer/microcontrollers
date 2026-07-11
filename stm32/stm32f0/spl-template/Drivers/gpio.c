/* Cortex M0 GPIO driver */

#include "gpio.h"

/* Enable the AHB clock for the given GPIO port */

void gpio_enable(GPIO_t *port) 
{
    if (port == GPIOA) RCC_AHBENR |= (1u << 17);
    else if (port == GPIOB) RCC_AHBENR |= (1u << 18);
    else if (port == GPIOC) RCC_AHBENR |= (1u << 19);
    else if (port == GPIOD) RCC_AHBENR |= (1u << 20);
    else if (port == GPIOF) RCC_AHBENR |= (1u << 22);
}

/* Configure pin mode (input/output/AF/analog) */

void gpio_mode(GPIO_t *port, uint8_t pin, GPIO_mode_t mode)
{
    port->MODER &= ~(3u << (pin * 2));       // clear mode bits
    port->MODER |=  ((uint32_t)mode << (pin * 2));
}

/* configure pull-up / pull-down resistors */

void gpio_pull(GPIO_t *port, uint8_t pin, GPIO_pull_t pull)
{
    port->PUPDR &= ~(3u << (pin * 2));       // clear pull bits
    port->PUPDR |=  ((uint32_t)pull << (pin * 2));
}

/* write a pin (atomic set/reset) */

void gpio_write(GPIO_t *port, uint8_t pin, uint8_t value) 
{
    if (value)
        port->BSRR = (1u << pin);            // set bit
    else
        port->BRR  = (1u << pin);            // reset bit
}

/* read a pin */

uint8_t gpio_read(GPIO_t *port, uint8_t pin) 
{
    return (port->IDR >> pin) & 1u;
}

