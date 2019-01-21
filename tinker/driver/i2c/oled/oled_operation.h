#ifndef __OLED_OPERATION_
#define __OLED_OPERATION_

#include "font.h"

void oled_write_byte(unsigned dat,unsigned cmd);
void oled_show_char(unsigned charx,unsigned char y,
                            unsigned char chr,unsigned char char_size);
void oled_show_string(unsigned char x,unsigned char y,
                                unsigned char *chr,unsigned char char_size);

void OLED_Set_Pos(unsigned char x, unsigned char y);

void oled_init(void);
void oled_clear(void);
void oled_on(void);
void oled_off(void);
char Write_IIC_Data(unsigned char IIC_Data);
int iic_write_reg(unsigned char cmd, unsigned char val);
char Write_IIC_Command(unsigned char IIC_Command);

#endif
