#ifndef __DEV_SYSFS_DEMO_H
#define __DEV_SYSFS_DEMO_H


#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/errno.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/printk.h>
#include <linux/major.h>
#include <linux/device.h>
#include <linux/printk.h>
#include <linux/mutex.h>
#include <linux/kern_levels.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/workqueue.h>


#define TAG                   		" <SYSFS_DEVICE> "
#define USR_MSG_LEVEL               KERN_WARNING
#define USR_ERR_LEVEL               KERN_ERR
#define usr_msg(fmt, args...)       printk(USR_MSG_LEVEL TAG fmt"\n", ##args)
#define err_msg(fmt, args...)       printk(USR_ERR_LEVEL TAG fmt"(function: %s)[line: %d]\n",__func__, __LINE__, ##args)



/**--------------------- function define start ---------------------------*/
// int foo_device_create(void);
// int foo_timer_init(void);
// void foo_timer_callback(unsigned long args);
/**--------------------- function define end ---------------------------*/


#endif
