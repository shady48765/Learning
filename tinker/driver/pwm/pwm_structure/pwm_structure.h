#ifndef __PWM_STRUCTURE_H
#define __PWM_STRUCTURE_H

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

#define TAG                   		" <PWM> "
#define USR_MSG_LEVEL               KERN_WARNING
#define USR_ERR_LEVEL               KERN_ERR
#define usr_msg(fmt, args...)       printk(USR_MSG_LEVEL TAG ""fmt"\n",##args)
#define err_msg(fmt, args...)       printk(USR_ERR_LEVEL TAG " (function : %s), [line : %d] "fmt"\n",__func__, __LINE__, ##args)


#define USR_PWM_DRV_NAME                "pwm_framwork_demo"
#define USED_HRS_TIMER					1
#define DEV_CREATE_FILE					1

/* start define struct -----------------------------------*/

static struct dts_info {
	unsigned int pin;

	unsigned int period;
	unsigned int freq;
	unsigned int polarity;
	unsigned int duty_cycle;
};


static struct pwm_info_container {	
	struct mutex 		lock;
	struct pwm_chip 	chip;
	struct dts_info 	dts;
	struct clk			*pwm_clk;
	struct clk 			*pll_clk;
	struct pwm_capture  *pwm_capture;
};

/* end define struct -----------------------------------*/

#endif

