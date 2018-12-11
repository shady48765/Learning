
#ifndef __GPIO_PINCTRL_H
#define __GPIO_PINCTRL_H

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/fs.h>                       // ioctl and file_operations support 
#include <linux/gpio.h>                     /* For Legacy integer based GPIO */
#include <linux/init.h>
// #include <linux/interrupt.h> /* For IRQ */
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

#include "./common.h"

#define usr_msg(fmt, arg...)                                                                    \
    do {                                                                                        \
        printk(KERN_ERR "-------> [info] :(%s) [%d] " fmt "\n", __func__, __LINE__, ##arg);     \
    } while (0)

#define err_msg(fmt, arg...)                                                                    \
    do {                                                                                        \
        printk(KERN_ERR "-------> [error] :(%s) [%d] " fmt "\n", __func__, __LINE__, ##arg);    \
    } while (0)

#define CHECK(type expr)           \
        do {                  \
            int ret = (expr); \
            ASSERT(0 == ret); \
        }while(0)
        
#define DRIVER_NAME         "laser_driver"
#define LASER_NAME          "laser"
#define LASER_CLS_NAME      "laser_class"
#define NODE_FROM           NULL


typedef enum {
    on  = 1,
    off = 0
} gpio_status_enum;
gpio_status_enum   gpio_status;

typedef struct {
    struct pinctrl          * laser_pinctrl;
    struct pinctrl_state    * laser_state_on;
    struct pinctrl_state    * laser_state_off
}pinctrl_info;
pinctrl_info * pin_info;

struct device_info {
    unsigned int    dev_major;
    struct cdev     dev;
    dev_t           dev_no;
    struct class    *dev_class;
    struct device   *dev_device;
    struct mutex    laser_mutex;
};
static struct device_info *dev_info;





// struct device_node	* laser_node;
static int get_dts_info(const char *compat);
static int dev_open(struct inode *inode, struct file *filp);
static int dev_release(struct inode *inode, struct file *filp);
static ssize_t dev_write(struct file *flip, const char __user *buff,
                    size_t counter, loff_t *fops);
static long laser_ioctl(struct file *flip, unsigned int cmd,
                    unsigned long param);
static int get_dts_info(struct device * dev);


#endif