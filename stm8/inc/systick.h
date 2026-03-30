/*
 * stm8 systick
 */

#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

extern volatile uint32_t systick_ms;

void systick_init(void);
void systick_delay(uint32_t ms);
uint32_t systick_get(void);

#define millis() systick_get()

#endif
