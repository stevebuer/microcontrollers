# BSP for STM32VLDISCOVERY

```
/* UI task blocks on the queue */

for (;;) {
    ButtonEvent_t evt;
    xQueueReceive(buttonQueue, &evt, portMAX_DELAY);

    switch (evt) {
        case BUTTON_PRESSED:
            BSP_LED_Toggle(LED_GREEN);
            break;
    }
}
```

## References

* SPL: Standard Peripheral Library v3.5.0
* STM32VLDISCOVERY Data Sheet
