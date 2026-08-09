#ifndef CMD_H
#define CMD_H

void cmd_set_i2c_scan_hook(void (*scan_fn)(void));
void cmd_init(void);
void cmd_poll(void);

#endif
