#include "button.h"
#include "systick.h"

static GPIO_TypeDef* btn_port;
static GPIO_Pin_TypeDef btn_pin;

static uint8_t last_state = 1;
static uint32_t last_change = 0;
static uint32_t press_time = 0;
static uint8_t long_fired = 0;

#define DEBOUNCE_MS     30
#define LONGPRESS_MS    800

void button_init(GPIO_TypeDef* port, GPIO_Pin_TypeDef pin)
{
    btn_port = port;
    btn_pin = pin;

    GPIO_Init(port, pin, GPIO_MODE_IN_PU_NO_IT);
    last_state = button_read(port, pin);
    last_change = millis();
    press_time = 0;
    long_fired = 0;
}

uint8_t button_read(GPIO_TypeDef* port, GPIO_Pin_TypeDef pin)
{
    return GPIO_ReadInputPin(port, pin) == RESET;
}

button_event_t button_update(void)
{
    if (btn_port == 0) {
        return BUTTON_EVENT_NONE;
    }

    uint8_t raw = button_read(btn_port, btn_pin);
    uint32_t now = millis();

    if (raw != last_state && (now - last_change) > DEBOUNCE_MS) {
        last_state = raw;
        last_change = now;

        if (raw) {
            press_time = now;
            long_fired = 0;
        } else {
            if (!long_fired && (now - press_time) < LONGPRESS_MS) {
                return BUTTON_EVENT_SHORT;
            }
        }
    }

    if (last_state && !long_fired && (now - press_time) >= LONGPRESS_MS) {
        long_fired = 1;
        return BUTTON_EVENT_LONG;
    }

    return BUTTON_EVENT_NONE;
}

