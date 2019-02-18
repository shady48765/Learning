#ifndef __SIMPLE_PWM_H
#define __SIMPLE_PWM_H

#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/errno.h>
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
#include <linux/err.h>
#include <linux/pwm.h>
#include <linux/of.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>          /* For platform devices */
#include <linux/printk.h>
#include <linux/mutex.h>
#include <linux/time.h>

#define TIMER_TAG                   " <PWM> "
#define USR_MSG_LEVEL               KERN_ERR
#define USR_ERR_LEVEL               KERN_ERR
#define usr_msg(fmt, args...)       printk(USR_MSG_LEVEL TIMER_TAG ""fmt"\n",##args)
#define err_msg(fmt, args...)       printk(USR_ERR_LEVEL TIMER_TAG " (function : %s), [line : %d] "fmt"\n",__func__, __LINE__, ##args)



/* function define start ---------------------------*/

/*  function define end ---------------------------*/

#define USR_PWM_DRV_NAME                "pwm_demo"
#define USED_HRS_TIMER					1
#define FOO_PROC_NAME                   0
#define TRANDITIONAL_WAY				0


static struct   timer_list  foo_time;
static struct   timeval     old_tmval;

struct dts_info {
	unsigned int freq;
	unsigned int plus_width;
	unsigned int status;
};

struct pwm_dev {
    unsigned int    pwm_major_num;
    dev_t           pwm_dev_num;
    struct cdev     pwm_cdev;
    struct class    *pwm_class;
    struct device   *pwm_device;
	struct mutex    lock;
	struct dts_info info;
};

struct pwm_timer {
    
}

#endif
