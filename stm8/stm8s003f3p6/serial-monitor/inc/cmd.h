#ifndef CMD_H
#define CMD_H

void cmd_set_i2c_scan_hook(void (*scan_fn)(void));
void cmd_set_i2c_rw_hooks(unsigned char (*read_reg_fn)(unsigned char addr, unsigned char reg, unsigned char* value),
	unsigned char (*write_reg_fn)(unsigned char addr, unsigned char reg, unsigned char value));
void cmd_init(void);
void cmd_poll(void);

#endif
