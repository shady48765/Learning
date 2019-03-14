#ifndef __DELAYQUEUE_H
#define __DELAYQUEUE_H


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
#include <linux/time.h>			// for get system current time
// for hr-timer
#include <linux/hrtimer.h>
#include <linux/sched.h>
#include <linux/ktime.h>


#define TAG                   		" <DELAY_QUEUE> "
#define USR_MSG_LEVEL               KERN_WARNING
#define USR_ERR_LEVEL               KERN_ERR
#define usr_msg(fmt, args...)       printk(USR_MSG_LEVEL TAG fmt"\n", ##args)
#define err_msg(fmt, args...)       printk(USR_ERR_LEVEL TAG fmt"(function: %s)[line: %d]\n",__func__, __LINE__, ##args)

#ifndef MS_TO_NS
	#define MS_TO_NS(x) 			(x * (1000000UL))      // ms to ns
#endif	/** end of #ifndef MS_TO_NS */

#define show_HZ()		usr_msg("system HZ = %u", HZ)


#define FOO_DEV_NAME                "simple_delaywork"

#endif
/**--------------------------- END LINE ----------------------------------*/
