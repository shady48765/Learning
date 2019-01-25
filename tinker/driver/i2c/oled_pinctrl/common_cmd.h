#ifndef __COMMON_CMD_H
#define __COMMON_CMD_H

#include <linux/fs.h>                       // ioctl and file_operations support

#define MAGIC_NO            'G'
#define SET 				0x61
#define RESET				0x62
#define BLINK				0x63
#define STATUS				0x64
#define TEST				0x65
#define INIT                0x66

#define OLED_TEST 			_IO(MAGIC_NO, TEST)
#define OLED_INIT           _IO(MAGIC_NO, INIT)
#define OLED_SET 			_IOW(MAGIC_NO, SET, unsigned int)
#define OLED_RESET			_IOW(MAGIC_NO, RESET, unsigned int)
#define OLED_BLINK			_IOW(MAGIC_NO, BLINK, unsigned int)			// param is timer delay
#define OLED_STATUS			_IOR(MAGIC_NO, STATUS, int)



#endif
