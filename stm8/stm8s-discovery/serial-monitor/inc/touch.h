#ifndef TOUCH_H
#define TOUCH_H

void touch_init(void);
void touch_poll(void);
unsigned char touch_take_press(void);

#endif