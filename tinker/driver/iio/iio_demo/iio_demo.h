#ifndef __IIO_DEMO_H
#define __IIO_DEMO_H

#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/printk.h>
#include <linux/major.h>
#include <linux/device.h>
#include <linux/timer.h>        //for timer_list, jiffy timer, standard timer
#include <linux/raid/pq.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>      /* For platform devices */
#include <linux/interrupt.h>            /* For IRQ */
#include <linux/of.h>                   /* For DT*/
#include <linux/mutex.h>
#include <linux/export.h>
// iio subsystem

#include <linux/iio/iio.h>    /* mandatory */
#include <linux/iio/sysfs.h>  /* mandatory since sysfs are used */
#include <linux/iio/events.h> /* For advanced users, to manage iio events */
#include <linux/iio/buffer.h>  /* mandatory to use triggered buffers */

#define TAG                    		" <IIO> "
#define MSG_LEVEL               	KERN_WARNING
#define ERR_LEVEL               	KERN_ERR
#define usr_msg(fmt, args...)       printk(MSG_LEVEL TAG fmt"\n", ##args)
#define err_msg(fmt, args...)       printk(ERR_LEVEL TAG "(function : %s), [line : %d] "fmt"\n",__func__, __LINE__, ##args)

#define FOO_DEV_NAME                "iio_demo"




#endif
