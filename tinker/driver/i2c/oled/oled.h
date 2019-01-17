#ifndef __OLED_H
#define __OLED_H



void oled_write_byte(unsigned dat,unsigned cmd);
void oled_show_char(unsigned charx,unsigned char y,
                            unsigned char chr,unsigned char char_size);
void oled_show_string(unsigned char x,unsigned char y,
                                unsigned char *chr,unsigned char char_size);

void oled_init(void);
void oled_clear(void);
void oled_on(void);
void oled_off(void);


#endif