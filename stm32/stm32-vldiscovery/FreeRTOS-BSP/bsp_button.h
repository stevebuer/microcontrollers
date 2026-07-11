#ifndef BSP_BUTTON_H
#define BSP_BUTTON_H

#include <stdint.h>

typedef enum {
    BUTTON_USER
} BSP_Button_t;

void BSP_Button_Init(BSP_Button_t btn);
uint8_t BSP_Button_Read(BSP_Button_t btn);
void BSP_Button_EnableIRQ(BSP_Button_t btn);

#endif
