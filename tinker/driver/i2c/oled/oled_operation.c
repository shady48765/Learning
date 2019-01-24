#include "oled_operation.h"

#define SSD1306_CMD_ADDR          0x00
#define SSD1306_DATA_ADDR         0x40

#define MAX_WIDTH				 128
#define MAX_HEIGHT				 64

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
	oled_init();
	oled_clean();
}

void oled_init(void)
{
#if 0
    oled_write_data_cmd(CMD_MODE, 0xae); // SSD1306_DISPLAYOFF
    oled_write_data_cmd(CMD_MODE, 0xd5); // SSD1306_SETDISPLAYCLOCKDIV
    oled_write_data_cmd(CMD_MODE, 0x80); // Suggested value 0x80
    oled_write_data_cmd(CMD_MODE, 0xa8); // SSD1306_SETMULTIPLEX
    oled_write_data_cmd(CMD_MODE, 0x3f); // 1/64
    oled_write_data_cmd(CMD_MODE, 0xd3); // SSD1306_SETDISPLAYOFFSET
    oled_write_data_cmd(CMD_MODE, 0x00); // 0 no offset
    oled_write_data_cmd(CMD_MODE, 0x40); // SSD1306_SETSTARTLINE line #0
    oled_write_data_cmd(CMD_MODE, 0x20); // SSD1306_MEMORYMODE
    oled_write_data_cmd(CMD_MODE, 0x00); // 0x0 act like ks0108
    oled_write_data_cmd(CMD_MODE, 0xa1); // SSD1306_SEGREMAP | 1
    oled_write_data_cmd(CMD_MODE, 0xc8); // SSD1306_COMSCANDEC
    oled_write_data_cmd(CMD_MODE, 0xda); // SSD1306_SETCOMPINS
    oled_write_data_cmd(CMD_MODE, 0x12);
    oled_write_data_cmd(CMD_MODE, 0x81); // SSD1306_SETCONTRAST
    oled_write_data_cmd(CMD_MODE, 0xcf);
    oled_write_data_cmd(CMD_MODE, 0xd9); // SSD1306_SETPRECHARGE
    oled_write_data_cmd(CMD_MODE, 0xf1);
    oled_write_data_cmd(CMD_MODE, 0xdb); // SSD1306_SETVCOMDETECT
    oled_write_data_cmd(CMD_MODE, 0x30);
    oled_write_data_cmd(CMD_MODE, 0x8d); // SSD1306_CHARGEPUMP
    oled_write_data_cmd(CMD_MODE, 0x14); // Charge pump on
    oled_write_data_cmd(CMD_MODE, 0x2e); // SSD1306_DEACTIVATE_SCROLL
    oled_write_data_cmd(CMD_MODE, 0xa4); // SSD1306_DISPLAYALLON_RESUME
    oled_write_data_cmd(CMD_MODE, 0xa6); // SSD1306_NORMALDISPLAY

#endif



    // Init OLED
    oled_write_cmd(0xAE); //display off
    oled_write_cmd(0x20); //Set Memory Addressing Mode   
    oled_write_cmd(0x10); // 00,Horizontal Addressing Mode; 01,Vertical Addressing Mode;
                                // 10,Page Addressing Mode (RESET); 11,Invalid
    oled_write_cmd(0xB0); //Set Page Start Address for Page Addressing Mode,0-7

#ifdef SSD1306_MIRROR_VERT
    oled_write_cmd(0xC0); // Mirror vertically
#else
    oled_write_cmd(0xC8); //Set COM Output Scan Direction
#endif
    oled_write_cmd(0x00); //---set low column address
    oled_write_cmd(0x10); //---set high column address

    oled_write_cmd(0x40); //--set start line address - CHECK

    oled_write_cmd(0x81); //--set contrast control register - CHECK
    oled_write_cmd(0xFF);

#ifdef SSD1306_MIRROR_HORIZ
    oled_write_cmd(0xA0); // Mirror horizontally
#else
    oled_write_cmd(0xA1); //--set segment re-map 0 to 127 - CHECK
#endif

#ifdef SSD1306_INVERSE_COLOR
    oled_write_cmd(0xA7); //--set inverse color
#else
    oled_write_cmd(0xA6); //--set normal color
#endif
    oled_write_cmd(0xA8); //--set multiplex ratio(1 to 64) - CHECK
    oled_write_cmd(0x3F); //
    oled_write_cmd(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    oled_write_cmd(0xD3); //-set display offset - CHECK
    oled_write_cmd(0x00); //-not offset
    oled_write_cmd(0xD5); //--set display clock divide ratio/oscillator frequency
    oled_write_cmd(0xF0); //--set divide ratio
    oled_write_cmd(0xD9); //--set pre-charge period
    oled_write_cmd(0x22); //
    oled_write_cmd(0xDA); //--set com pins hardware configuration - CHECK
    oled_write_cmd(0x12);
    oled_write_cmd(0xDB); //--set vcomh
    oled_write_cmd(0x20); //0x20,0.77xVcc
    oled_write_cmd(0x8D); //--set DC-DC enable
    oled_write_cmd(0x14); //
    oled_write_cmd(0xAF); //--turn on SSD1306 panel

    // Clear screen
    ssd1306_Fill(Black);
    
    // Flush buffer to screen
    ssd1306_UpdateScreen();





     oled_clean();
}

void oled_clean(void)
{
    unsigned char loop, index;
    for(loop = 0;loop < 8; loop++) {
        oled_write_data_cmd (CMD_MODE, 0xb0 + loop);     //设置页地址（0~7）
        oled_write_data_cmd (CMD_MODE, 0x00);            //设置显示位置—列低地址
        oled_write_data_cmd (CMD_MODE, 0x10);            //设置显示位置—列高地址
        for(index = 0; index < 128; index++)
            oled_write_data_cmd(DATA_MODE, 0);
    }
}


void oled_write_data_cmd(unsigned char data_cmd_flags, unsigned char data)
{
    if(data_cmd_flags) {
		oled_i2c_send_byte(oled_i2c_info->oled_client, SSD1306_DATA_ADDR, data);
    } else {
        oled_i2c_send_byte(oled_i2c_info->oled_client, SSD1306_CMD_ADDR, data);
    }
}


void oled_write_cmd(unsigned char cmd)
{
    oled_i2c_send_byte(oled_i2c_info->oled_client, SSD1306_CMD_ADDR, data);
}

void oled_write_data(unsigned char data)
{
    oled_i2c_send_byte(oled_i2c_info->oled_client, SSD1306_DATA_ADDR, data);
}

void oled_refresh(void)
{
    
}