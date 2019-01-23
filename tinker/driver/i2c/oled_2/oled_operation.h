#ifndef __OLED_OPERATION_
#define __OLED_OPERATION_

#include "font.h"
#include "oled_driver.h"





void oled_mark_dot(unsigned char x, unsigned y);
void oled_write_line(unsigned char line , unsigned char start, unsigned char end);
void oled_power_on(void);
void oled_clean(void);
void oled_write_byte(unsigned dat,unsigned cmd);

int i2c_msg_send(const struct i2c_client *client, const char reg, 
                        const char *buf, int count, int scl_rate);
int i2c_write_reg(unsigned char cmd, unsigned char val);
char i2c_write_data(unsigned char i2c_data);
void oled_write_byte(unsigned dat,unsigned cmd);
char i2c_write_cmd(unsigned char i2c_cmd);











#endif
