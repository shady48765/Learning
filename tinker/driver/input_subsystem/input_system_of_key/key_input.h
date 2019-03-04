#ifndef __KEY_INPUT_H
#define __KEY_INPUT_H

#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/printk.h>
#include <linux/major.h>
#include <linux/device.h>
#include <linux/timer.h>        //for timer_list, jiffy timer, standard timer
#include <linux/version.h>
#include <linux/delay.h>
#include <linux/major.h>
#include <linux/module.h>
#include <linux/of_gpio.h>                  /* For of_gpio* functions */
#include <linux/of_device.h>
#include <linux/time.h>	            // for get system current time
#include <linux/proc_fs.h>
#include <linux/device.h>

#include <linux/of.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>          /* For platform devices */
#include <linux/printk.h>
#include <linux/mutex.h>
#include <linux/time.h>
#include <linux/pwm.h>
#include <linux/clk.h>      // get clock freq

#include <linux/input.h>

#define TAG                   		" <KEY> "
#define USR_MSG_LEVEL               KERN_WARNING
#define USR_ERR_LEVEL               KERN_ERR
#define usr_msg(fmt, args...)       printk(USR_MSG_LEVEL TAG ""fmt"\n",##args)
#define err_msg(fmt, args...)       printk(USR_ERR_LEVEL TAG "(function : %s), [line : %d] "fmt"\n",__func__, __LINE__, ##args)


#define USR_KEY_NAME                    "USR_KEY"

/* start define struct -----------------------------------*/


/* end define struct -----------------------------------*/

#endif

