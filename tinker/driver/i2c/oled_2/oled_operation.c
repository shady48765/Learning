#include "oled_operation.h"

#define SSD1306_CMD         0x00
#define SSD1306_DAT         0x40
#define Max_Column          128



#define OLED_CMD  0  //write command
#define OLED_DATA 1  //write data

void oled_mark_dot(unsigned char x, unsigned y)
{
    
}
EXPORT_SYMBOL(oled_mark_dot);

void oled_write_line(unsigned char line , unsigned char start, unsigned char end)
{
    
}
EXPORT_SYMBOL(oled_write_line);

void oled_power_on(void)
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
     oled_clean();
}
EXPORT_SYMBOL(oled_power_on);

void oled_clean(void)
{
    unsigned char loop, index;
    for(loop = 0;loop < 8; loop++) {
        oled_write_byte (0xb0 + loop, OLED_CMD);     //设置页地址（0~7）
        oled_write_byte (0x00, OLED_CMD);            //设置显示位置—列低地址
        oled_write_byte (0x10, OLED_CMD);            //设置显示位置—列高地址
        for(index = 0; index < 128; index++)
            oled_write_byte(0, OLED_DATA);
    }
}
EXPORT_SYMBOL(oled_clean);


void oled_write_byte(unsigned dat,unsigned cmd)
{
    if(cmd) {
        i2c_write_data(dat);
    } else {
        i2c_write_cmd(dat);
    }
}
EXPORT_SYMBOL(oled_write_byte);

char i2c_write_data(unsigned char i2c_data)
{
    return i2c_write_reg(SSD1306_DAT, i2c_data);
}
EXPORT_SYMBOL(i2c_write_data);

int i2c_write_reg(unsigned char cmd, unsigned char val)
{
 return i2c_msg_send(oled_i2c_info->oled_client, cmd, &val, sizeof(val), 100000);
}
EXPORT_SYMBOL(i2c_write_reg);

char i2c_write_cmd(unsigned char i2c_cmd)
{
	return i2c_write_reg(SSD1306_CMD, i2c_cmd);
}
EXPORT_SYMBOL(i2c_write_cmd);

int i2c_msg_send(const struct i2c_client *client, const char reg, 
                        const char *buf, int count, int scl_rate)
{
    struct i2c_adapter *adap=client->adapter;
    struct i2c_msg msg;
    int ret;
    char *tx_buf = (char *)kzalloc(count + 1, GFP_KERNEL);
    if(!tx_buf)
        return -ENOMEM;
    tx_buf[0] = reg;
    memcpy(tx_buf+1, buf, count); 

    msg.addr = client->addr;
    msg.flags = client->flags;
    msg.len = count + 1;
    msg.buf = (char *)tx_buf;

    ret = i2c_transfer(adap, &msg, 1); 
    kfree(tx_buf);
    return (ret == 1) ? count : ret;
}
EXPORT_SYMBOL(i2c_msg_send);








// //i2c发送函数 *client 设备  reg 寄存器  *buf 数据 count  数据大小  scl_rate i2c速率
// int i2c_msg_send(const struct i2c_client *client, const char reg, 
//                                                 const char *buf, int count, int scl_rate)
// {
//     struct i2c_adapter *adap=client->adapter;
//     struct i2c_msg msg;
//     int ret;
//     char *tx_buf = (char *)kzalloc(count + 1, GFP_KERNEL);
//     if(!tx_buf)
//         return -ENOMEM;
//     tx_buf[0] = reg;
//     memcpy(tx_buf+1, buf, count); 
 
//     msg.addr = client->addr;
//     msg.flags = client->flags;
//     msg.len = count + 1;
//     msg.buf = (char *)tx_buf;
 
//     ret = i2c_transfer(adap, &msg, 1); 
//     kfree(tx_buf);
//     return (ret == 1) ? count : ret;
 
// }

// int i2c_write_reg(unsigned char cmd, unsigned char val)
// {
// 	return i2c_msg_send(oled_i2c_info->oled_client, cmd, &val, sizeof(val), 100000);
// }

// char i2c_write_cmd(unsigned char i2c_cmd)
// {
// 	return i2c_write_reg(SSD1306_CMD, i2c_cmd);
// }
// char i2c_write_data(unsigned char i2c_data)
// {
// 	return i2c_write_reg(SSD1306_DAT, i2c_data);
// }

// void oled_write_byte(unsigned dat,unsigned cmd)
// {
// 	if(cmd) {
// 		//i2c_write_data(dat);
// 		i2c_write_data(dat);

// 	} else {
// 		i2c_write_cmd(dat);
//  		//  i2c_write_cmd(dat);
// 	}
// 	//	mdelay(1);
// }

// void oled_set_pos(unsigned char x, unsigned char y)
// {       oled_write_byte(0xb0+y,OLED_CMD);
//         oled_write_byte(((x&0xf0)>>4)|0x10,OLED_CMD);
//         oled_write_byte((x&0x0f),OLED_CMD);
// }

// void oled_show_char(unsigned char x,unsigned char y,
//                             unsigned char chr,unsigned char char_size)
// {
//     unsigned char c = 0,loop = 0;
//     c = chr -' ';
//     if(x > Max_Column - 1) {
//     	x = 0;
//     	y = y + 2;
//     }
//     if(char_size == 16) {
//     	oled_set_pos(x, y);
//     	for(loop = 0; loop < 8; loop++)
//     		oled_write_byte(F8X16[c * 16 + loop], OLED_DATA);

//     	oled_set_pos(x, y + 1);
//     	for(loop = 0; loop < 8; loop++)
//     		oled_write_byte(F8X16[c * 16 + loop + 8], OLED_DATA);
//     } else {
//     	oled_set_pos(x, y);
//     	for(loop = 0; loop < 6; loop++)
//     		oled_write_byte(F6x8[c][loop], OLED_DATA);
//     }
// }

// void oled_show_string(unsigned char x,unsigned char y,
//                         unsigned char *chr,unsigned char char_size)

// {
//     unsigned char index = 0;
//     while (chr[index] != '\0') {
//         oled_show_char(x, y, chr[index], char_size);
//         x += 8;
//         if(x>120){
//             x = 0;
//             y += 2;
//         }
//         index++;
//     }
// }


// void oled_init(void)
// {
// 	mdelay(100);
// 	oled_write_byte(0xAE, OLED_CMD);     //关闭显示

// 	oled_write_byte(0x40, OLED_CMD);     //---set low column address
// 	oled_write_byte(0xB0, OLED_CMD);     //---set high column address

// 	oled_write_byte(0xC8, OLED_CMD);     //-not offset

// 	oled_write_byte(0x81, OLED_CMD);     //设置对比度
// 	oled_write_byte(0xff, OLED_CMD);

// 	oled_write_byte(0xa1, OLED_CMD);     //段重定向设置

// 	oled_write_byte(0xa6, OLED_CMD);     //

// 	oled_write_byte(0xa8, OLED_CMD);     //设置驱动路数
// 	oled_write_byte(0x1f, OLED_CMD);

// 	oled_write_byte(0xd3, OLED_CMD);
// 	oled_write_byte(0x00, OLED_CMD);

// 	oled_write_byte(0xd5, OLED_CMD);
// 	oled_write_byte(0xf0, OLED_CMD);

// 	oled_write_byte(0xd9, OLED_CMD);
// 	oled_write_byte(0x22, OLED_CMD);

// 	oled_write_byte(0xda, OLED_CMD);
// 	oled_write_byte(0x02, OLED_CMD);

// 	oled_write_byte(0xdb, OLED_CMD);
// 	oled_write_byte(0x49, OLED_CMD);

// 	oled_write_byte(0x8d, OLED_CMD);
// 	oled_write_byte(0x14, OLED_CMD);

// 	oled_write_byte(0xaf, OLED_CMD);
// 	oled_clear();

// }

// void oled_clear(void)
// {
// 	unsigned char loop, index;
// 	for(loop = 0;loop < 8; loop++) {
// 		oled_write_byte (0xb0 + loop,OLED_CMD);     //设置页地址（0~7）
// 		oled_write_byte (0x00,OLED_CMD);            //设置显示位置—列低地址
// 		oled_write_byte (0x10,OLED_CMD);            //设置显示位置—列高地址
// 		for(index = 0; index < 128; index++)oled_write_byte(0, OLED_DATA);
// 	} //更新显示

// }

// void oled_on(void)
// {
//     unsigned char loop, index;
//     for(loop = 0; loop < 8; loop++) {
// 		oled_write_byte (0xb0 + loop,OLED_CMD);
// 		oled_write_byte (0x00,OLED_CMD);
// 		oled_write_byte (0x10,OLED_CMD);
// 		for(index = 0; index < 128; index++)oled_write_byte(0xff, OLED_DATA);
//     }
// }

// void oled_off(void)
// {
//     return ;
// }
