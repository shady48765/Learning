#include "oled_operation.h"

#define SSD1306_CMD_ADDR          0x00
#define SSD1306_DATA_ADDR         0x40

#define MAX_WIDTH                128
#define MAX_HEIGHT               64

#define CMD_MODE            0  //write command
#define DATA_MODE           1  //write data

void oled_mark_dot(unsigned char x, unsigned y)
{
    int index;
    for(index = 0; index < x; index++) {
        oled_write_data_cmd(0, DATA_MODE);
    }
    oled_write_data_cmd(1, DATA_MODE);
}

void oled_write_horizontal_line(unsigned char line_index , unsigned char start, unsigned char end)
{

}


void oled_write_vertical_line(unsigned char vertical_index , unsigned char start, unsigned char end)
{

}

void oled_power_on(void)
{
    int loop, index;
    oled_init();
    oled_clean();
    for (loop = 0; loop < 8; loop++) {
        oled_write_data_cmd(0xb0 + loop, CMD_MODE); //设置页地址（0~7）
        oled_write_data_cmd(0x00, CMD_MODE); //设置显示位置—列低地址
        oled_write_data_cmd(0x10, CMD_MODE); //设置显示位置—列高地址
        for (index = 1; index < 128; index++)
        oled_write_data_cmd(0, DATA_MODE);
    }
}

void oled_init(void)
{
     mdelay(100);
     oled_write_data_cmd(0XAE, CMD_MODE);     //关闭显示
     oled_write_data_cmd(0X40, CMD_MODE);     //---set low column address
     oled_write_data_cmd(0XB0, CMD_MODE);     //---set high column address

     oled_write_data_cmd(0XC8, CMD_MODE);     //-not offset

     oled_write_data_cmd(0X81, CMD_MODE);     //设置对比度
     oled_write_data_cmd(0XFF, CMD_MODE);

     oled_write_data_cmd(0XA1, CMD_MODE);     //段重定向设置

     oled_write_data_cmd(0XA6, CMD_MODE);     //

     oled_write_data_cmd(0XA8, CMD_MODE);     //设置驱动路数
     oled_write_data_cmd(0X1F, CMD_MODE);

     oled_write_data_cmd(0XD3, CMD_MODE);
     oled_write_data_cmd(0X00, CMD_MODE);

     oled_write_data_cmd(0XD5, CMD_MODE);
     oled_write_data_cmd(0XF0, CMD_MODE);

     oled_write_data_cmd(0XD9, CMD_MODE);
     oled_write_data_cmd(0X22, CMD_MODE);

     oled_write_data_cmd(0XDA, CMD_MODE);
     oled_write_data_cmd(0X02, CMD_MODE);

     oled_write_data_cmd(0XDB, CMD_MODE);
     oled_write_data_cmd(0X49, CMD_MODE);

     oled_write_data_cmd(0X8D, CMD_MODE);
     oled_write_data_cmd(0X14, CMD_MODE);

     oled_write_data_cmd(0XAF, CMD_MODE);
     oled_clean();
}

void oled_clean(void)
{
    unsigned char loop, index;
    for(loop = 0;loop < 8; loop++) {
        oled_write_data_cmd (0xb0 + loop, CMD_MODE);     //设置页地址（0~7）
        oled_write_data_cmd (0x00, CMD_MODE);            //设置显示位置—列低地址
        oled_write_data_cmd (0x10, CMD_MODE);            //设置显示位置—列高地址
        for(index = 0; index < 128; index++)
            oled_write_data_cmd(0, DATA_MODE);
    }
}


void oled_write_data_cmd(unsigned char data, unsigned char data_cmd_flags)
{
    if(data_cmd_flags) {
        oled_i2c_send_byte(oled_i2c_info->oled_client, SSD1306_DATA_ADDR, data);
    } else {
        oled_i2c_send_byte(oled_i2c_info->oled_client, SSD1306_CMD_ADDR, data);
    }
}
