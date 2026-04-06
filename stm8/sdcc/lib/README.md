# Small STM8 Lib

## Button

To Do: callback based API?

```
button_init(GPIOB, GPIO_PIN_4);

if (button_read(GPIOB, GPIO_PIN_4)) {
    // button pressed
}
```
### Long Press

```
button_init(GPIOB, GPIO_PIN_4);

while (1) {
    button_event_t e = button_update();

    if (e == BUTTON_EVENT_SHORT) {
        // short press action
    }

    if (e == BUTTON_EVENT_LONG) {
        // long press action
    }

    // rest of your loop
}
```
