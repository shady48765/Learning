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













// void oled_write_byte(unsigned dat,unsigned cmd);
// void oled_show_char(unsigned char x,unsigned char y,
//                             unsigned char chr,unsigned char char_size);

// void oled_show_string(unsigned char x,unsigned char y,
//                                 unsigned char *chr,unsigned char char_size);

// void oled_set_pos(unsigned char x, unsigned char y);

// void oled_init(void);
// void oled_clear(void);
// void oled_on(void);
// void oled_off(void);
// char write_i2c_data(unsigned char i2c_data);
// int iic_write_reg(unsigned char cmd, unsigned char val);
// char write_i2c_cmd(unsigned char i2c_cmd);
// int i2c_master_reg8_send(const struct i2c_client *client, const char reg, 
//                                         const char *buf, int count, int scl_rate);



// extern struct _oled_i2c_info * oled_i2c_info;
// extern struct _oled_dts_info * oled_dts_info;
// extern struct _oled_device_info * oled_dev_info;

#endif
