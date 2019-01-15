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

#include "common_cmd.h"

#define    OLED_DEV_NAME         "OLED_SSD1306"



typedef enum {
    off = 0,
    on
}status;
    
    
static int oled_open(struct inode *inode, struct file *filp);
static int oled_close(struct inode *inode, struct file *filp);
static ssize_t oled_write(struct file *flip, const char __user *buff,
                                    size_t counter, loff_t *fops);
long oled_ioctl (struct file *flip, unsigned int cmd, unsigned long param);


struct file_operations fops = {
    .open = oled_open,
    .release = oled_close,
    .write = oled_write,
    .unlocked_ioctl = oled_ioctl,
};

static struct oled_device {
    unsigned int    oled_major_number;
    dev_t           oled_dev_number;
    struct cdev     oled_cdev;
    struct class    *oled_class;
    struct device   *oled_device;
};

struct oled_info_struct {
    struct cdev             oled_dev;
    struct file_operations  oled_fops;
    struct oled_device      oled_dev;
};
struct oled_info_struct     *oled_info;



#endif
