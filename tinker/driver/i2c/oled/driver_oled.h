#ifndef __DRIVER_OLED_H
#define __DRIVER_OLED_H

#include <linux/cdev.h>
#include <linux/of.h>
#include <linux/devices.h>
#include <linux/fs.h>                       // ioctl and file_operations support 
#include <linux/gpio.h>                     /* For Legacy integer based GPIO */
#include <linux/init.h>
#include <linux/interrupt.h>                /* For IRQ */
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/module.h>
#include <linux/of.h>                       /* For DT*/
#include <linux/of_gpio.h>                  /* For of_gpio* functions */
#include <linux/of_device.h>
#include <linux/pinctrl/pinctrl.h>
#include <linux/pinctrl/consumer.h>
#include <linux/pinctrl/pinctrl-state.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>          /* For platform devices */
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>


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

struct i2c_device oled_i2c;

struct data_info {
        
};

struct data_report {
    
}


#endif