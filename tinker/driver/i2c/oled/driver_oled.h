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
#include <linux/proc.h>
#include <linux/time.h>	// for get system current time

#include <linux/proc_fs.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>

#include "common_cmd.h"

#define OLED_DEBUG					0




#define TAG                         " < OLED > "
#define USR_MSG_LEVEL               KERN_WARNING
#define USR_ERR_LEVEL               KERN_ERR
#define usr_msg(fmt, args...)       printk(USR_MSG_LEVEL TAG " (function : %s), [line : %d] "fmt"\n",__func__, __LINE__, ##args)
#define err_msg(fmt, args...)       printk(USR_ERR_LEVEL TAG " (function : %s), [line : %d] "fmt"\n",__func__, __LINE__, ##args)


#define    OLED_DEV_NAME         "OLED_SSD1306"


typedef enum {
    off = 0,
    on  = 1
}status;


static int oled_open(struct inode *inode, struct file *filp);
static int oled_close(struct inode *inode, struct file *filp);
static ssize_t oled_write(struct file *flip, const char __user *buff,
                                    size_t counter, loff_t *fops);
static long oled_ioctl (struct file *flip, unsigned int cmd, unsigned long param);
static int oled_get_dts_info(void);


static struct file_operations fops = {
    .open 			= oled_open,
    .release 		= oled_close,
    .write 			= oled_write,
    .unlocked_ioctl = oled_ioctl,
};
	
/* oled device information struction ---------------------------------------*/
static struct _oled_device_info {
    unsigned int    oled_major;
    dev_t           oled_devno;
    struct cdev     oled_cdev;
    struct class    * oled_class;
    struct device   * oled_device;
    struct mutex    oled_dev_mutex;
};
static struct _oled_device_info * oled_dev_info;

/* oled dts information struction ---------------------------------------*/
static struct _oled_dts_info = {
	struct device_node 	*node;
	int 				oled_rst;
	unsigned int 		oled_width;
	unsigned int 		oled_height;
	unsigned int 		oled_i2c_clk;

	struct pinctrl 		*dev_pinctrl
	struct pinctrl 		*st_high;
	struct pinctrl 		*rst_low;
};
static struct _oled_dts_info * oled_dts_info;

/* oled i2c information struction ---------------------------------------*/
static struct _oled_i2c_info = {	
	struct i2c_client 	* oled_client;
	char 				* data;
};
static struct _oled_i2c_info * oled_i2c_info;

/* oled whole information struction ---------------------------------------*/
// static struct oled_info_struct {
//    struct oled_device_info      	*dev_info; 
//	struct oled_i2c_info			*i2c_info;
//};
//  static struct oled_info_struct     	*oled_info;


/* timer parameters initaliztion -------------------------------------------*/
static struct timer_list oled_time;

#endif
