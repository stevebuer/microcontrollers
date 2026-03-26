#ifndef BSP_LED_H
#define BSP_LED_H

#include <stdint.h>

typedef enum {
    LED_GREEN,
    LED_BLUE
} BSP_LED_t;

void BSP_LED_Init(BSP_LED_t led);
void BSP_LED_On(BSP_LED_t led);
void BSP_LED_Off(BSP_LED_t led);
void BSP_LED_Toggle(BSP_LED_t led);

#endif
