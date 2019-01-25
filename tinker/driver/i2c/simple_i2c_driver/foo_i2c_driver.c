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
#include <linux/time.h>	// for get system current time
#include <linux/proc_fs.h>

#include <linux/i2c.h>
#include <linux/i2c-core.h>
#include <linux/i2c-dev.h>


#define FOO_DEV_NAME		"foo_i2c_device"

#define PC_PALTFORM			1

struct _foo_device_info {
	unsigned int 	dev_major;
	dev_t			dev_no;
	struct cdev 	foo_cdev;
	struct class	*foo_class;
	struct device	*foo_device;
	struct mutex	foo_dev_lock;
};

struct _foo_i2c_info {
	struct i2c_client	*foo_client;
	char 				* data;
	struct mutex		foo_i2c_lock;
};

struct _foo_info {

};


static platform_device foo_pdev_declartion = {
	.name = FOO_DEV_NAME,
	.id = -1,
	.resource = 
};

static int __inline foo_i2c_driver_init(void)
{
#if PC_PALTFORM
	platform_device_register(struct platform_device * pdev)
#else



#endif

	
}


static void __exit foo_i2c_driver_exit(void)
{
	
}





module_init(foo_i2c_driver_init);
module_exit(foo_i2c_driver_exit);



