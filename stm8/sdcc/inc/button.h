#ifndef BUTTON_H
#define BUTTON_H

#include "stm8s.h"

void button_init(GPIO_TypeDef* port, GPIO_Pin_TypeDef pin);
uint8_t button_read(GPIO_TypeDef* port, GPIO_Pin_TypeDef pin);

#endif

---

#ifndef BUTTON_H
#define BUTTON_H

#include "stm8s.h"
#include <stdint.h>

typedef enum {
    BUTTON_EVENT_NONE = 0,
    BUTTON_EVENT_SHORT,
    BUTTON_EVENT_LONG
} button_event_t;

void button_init(GPIO_TypeDef* port, GPIO_Pin_TypeDef pin);
button_event_t button_update(void);

#endif

