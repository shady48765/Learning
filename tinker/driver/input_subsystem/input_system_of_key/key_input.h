#ifndef __KEY_INPUT_H
#define __KEY_INPUT_H

#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/major.h>
// for char device
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/major.h>
#include <linux/device.h>
// for printk
#include <linux/printk.h>

// for mdelay
#include <linux/delay.h>

// for dts
#include <linux/of_gpio.h>                  /* For of_gpio* functions */
#include <linux/of_device.h>
#include <linux/time.h>	            // for get system current time
// for proc_create
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>          /* For platform devices */

#include <linux/mutex.h>

// for input subsystem
#include <linux/input.h>
// for waitqueue
#include <linux/workqueue.h>
// for timer
#include <linux/jiffies.h>
#include <linux/timer.h>	// for timer_list, jiffy timer, standard timer
#include <linux.time.h>		// for system time API
// for interrupt
#include <linux/interrupt.h>
// for attribute S_IRUSR
#include <uapi/linux/stat.h>


#define TAG                   		" <KEY> "
#define USR_MSG_LEVEL               KERN_WARNING
#define USR_ERR_LEVEL               KERN_ERR
#define usr_msg(fmt, args...)       printk(USR_MSG_LEVEL TAG fmt"\n",##args)
#define err_msg(fmt, args...)       printk(USR_ERR_LEVEL TAG fmt"(function : %s), [line : %d]\n", ##args, __func__, __LINE__)


#define KEY_DEV_NAME                    "USR_KEY"
#define VERSION							"version-0.1"

/* start define struct -----------------------------------*/
struct dts_information {
	struct pinctrl 			* node;
	struct pinctrl_state 	* state;
	struct mutex			pin_lock;
	struct device_node 		* dev_node;
	unsigned int 					irq_no;
};

struct timer_information {
	struct timer_list 	tim;
	struct timeval		val;
};

struct device_information {
	struct cdev 	* dev;
	dev_t			devt;
	unsigned int 	major;
	
};

struct key_information {
	struct dts_information 		* dts;
	struct timer_information 	* tim;
	struct device_information 	* device;

	struct mutex 				lock;
	struct input_dev 			* input_dev;
};

/* end define struct -----------------------------------*/

#endif

