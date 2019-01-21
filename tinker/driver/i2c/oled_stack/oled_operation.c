#include "oled_operation.h"
#include "oled_font.h"
#include "driver_oled.h"

#define SSD1306_CMD    	0x00
#define SSD1306_DAT    	0x40
#define Max_Column      128




#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

int iic_write_reg(unsigned char cmd, unsigned char val)
{
	return i2c_master_reg8_send(oled_client, cmd, &val, sizeof(val), 100000);
}

char Write_IIC_Command(unsigned char IIC_Command)
{
	return iic_write_reg(SSD1306_CMD, IIC_Command);
}
char Write_IIC_Data(unsigned char IIC_Data)
{
	return iic_write_reg(SSD1306_DAT, IIC_Data);
}

void oled_write_byte(unsigned dat,unsigned cmd)
{
	if(cmd) {
		//Write_IIC_Data(dat);
		Write_IIC_Data(dat);

	} else {
		Write_IIC_Command(dat);
 		//  Write_IIC_Command(dat);
	}
	//	mdelay(1);
}

void oled_show_char(unsigned char x,unsigned char y,
                            unsigned char chr,unsigned char char_size)
{
	unsigned char c = 0,loop = 0;
	c = chr -' ';
	if(x > Max_Column - 1) {
		x = 0;
		y = y + 2;
	}
	if(char_size == 16) {
		OLED_Set_Pos(x, y);
		for(loop = 0; loop < 8; loop++)
			oled_write_byte(F8X16[c * 16 + loop], OLED_DATA);

		OLED_Set_Pos(x, y + 1);
		for(loop = 0; loop < 8; loop++)
			oled_write_byte(F8X16[c * 16 + loop + 8], OLED_DATA);
	} else {
		OLED_Set_Pos(x, y);
		for(loop = 0; loop < 6; loop++)
			oled_write_byte(F6x8[c][loop], OLED_DATA);
	}
}

void oled_show_string(unsigned char x,unsigned char y,
                        unsigned char *chr,unsigned char char_size)

{
    unsigned char index = 0;
    while (chr[index] != '\0') {
        oled_show_char(x, y, chr[index], char_size);
        x += 8;
        if(x>120){
            x = 0;
            y += 2;
        }
        index++;
    }
}


void oled_init(void)
{
	mdelay(100);
	oled_write_byte(0xAE, OLED_CMD);     //关闭显示

	oled_write_byte(0x40, OLED_CMD);     //---set low column address
	oled_write_byte(0xB0, OLED_CMD);     //---set high column address

	oled_write_byte(0xC8, OLED_CMD);     //-not offset

	oled_write_byte(0x81, OLED_CMD);     //设置对比度
	oled_write_byte(0xff, OLED_CMD);

	oled_write_byte(0xa1, OLED_CMD);     //段重定向设置

	oled_write_byte(0xa6, OLED_CMD);     //

	oled_write_byte(0xa8, OLED_CMD);     //设置驱动路数
	oled_write_byte(0x1f, OLED_CMD);

	oled_write_byte(0xd3, OLED_CMD);
	oled_write_byte(0x00, OLED_CMD);

	oled_write_byte(0xd5, OLED_CMD);
	oled_write_byte(0xf0, OLED_CMD);

	oled_write_byte(0xd9, OLED_CMD);
	oled_write_byte(0x22, OLED_CMD);

	oled_write_byte(0xda, OLED_CMD);
	oled_write_byte(0x02, OLED_CMD);

	oled_write_byte(0xdb, OLED_CMD);
	oled_write_byte(0x49, OLED_CMD);

	oled_write_byte(0x8d, OLED_CMD);
	oled_write_byte(0x14, OLED_CMD);

	oled_write_byte(0xaf, OLED_CMD);
	oled_clear();

}

void oled_clear(void)
{
	u8 loop, index;
	for(loop = 0;loop < 8; loop++) {
		oled_write_byte (0xb0 + loop,OLED_CMD);     //设置页地址（0~7）
		oled_write_byte (0x00,OLED_CMD);            //设置显示位置—列低地址
		oled_write_byte (0x10,OLED_CMD);            //设置显示位置—列高地址
		for(index = 0; index < 128; index++)oled_write_byte(0, OLED_DATA);
	} //更新显示

}

void oled_on(void)
{
    u8 loop, index;
    for(loop = 0; loop < 8; loop++) {
		oled_write_byte (0xb0 + loop,OLED_CMD);
		oled_write_byte (0x00,OLED_CMD);
		oled_write_byte (0x10,OLED_CMD);
		for(index = 0; index < 128; index++)oled_write_byte(0xff, OLED_DATA);
    }

}
void oled_off(void)
{
    return ;
}
