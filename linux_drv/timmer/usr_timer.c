
/*----------------------------------------------------------------------*
 * Data : 2018-10-02
 *
 *
 *
 *
 *
 *----------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/printk.h>
#include <linux/major.h>
#include <linux/device.h>

#include <linux/uaccess.h>
//#include <linux/timer.h>


/*--------------------- macro defined start --------------------------*/
#define usr_msg(args)                                           \
            do{                                                 \
                    printk(KERN_ERR "---> %s\n", args);         \
            }while(0)

#define err_msg(args)                                           \
            do{                                                 \
                    printk(KERN_ERR "-------> %s\n", args);     \
            }while(0)
/*--------------------- macro defined end --------------------------*/
#define DEV_MAJOR       0
/*------------ global parameter declaration start -------------------*/
// ---> function declartion
static void sencod_timer_handler(unsigned long args);
static int tim_open(struct inode *inode, struct file *filp);
static int tim_release(struct inode *inode, struct file *filp);
static ssize_t tim_read(struct file * flip, char __user * buff, 
						size_t counter, loff_t * fops);						\
static ssize_t tim_write(struct file * flip, const char __user * buff,		\
                         size_t counter, loff_t * fops);

// <--- function declartion
static const char * device_name = "usr_timer_dev";
static const char * class_name = "usr_class_timer";

struct file_operations fops = {
    .owner          = THIS_MODULE,
	.open           = tim_open,
	.read           = tim_read,
	.write          = tim_write,
	.release        = tim_release,
};
// ---> set container start 
struct _timer_dev {
	unsigned int dev_major;
	dev_t evt;

	struct cdev 		dev;
	struct class 		* dev_class;
	struct device 		* dev_device;

    atomic_t 			counter;
    struct timer_list 	s_timer;
};
static struct _timer_dev *dev_timer;
// <--- set container end


/*------------ global parameter declaration end -------------------*/
/*------------ fops start -----------------------------------------*/

static void sencod_timer_handler(unsigned long args)
{
    printk(KERN_ERR"%s\n", __func__);
    mod_timer(&dev_timer->s_timer, jiffies + HZ);   // trigger next timer trigger
    atomic_inc(&dev_timer->counter);                // feed counter
    printk(KERN_ERR"curretn jiffies is %ld\n", jiffies);
}
/*---------------------------------------------------------------
void init_timer_key(struct timer_list *timer, unsigned int flags,
		    const char *name, struct lock_class_key *key)
----------------------------------------------------------------*/
static int tim_open(struct inode *inode, struct file *filp)
{
	printk(KERN_ERR"%s\n", __func__);
    
    usr_msg("init timer");
    init_timer(&dev_timer->s_timer);
    
    dev_timer->s_timer.function = &sencod_timer_handler;
    dev_timer->s_timer.expires = jiffies + HZ;
    add_timer(&dev_timer->s_timer);
    atomic_set(&dev_timer->counter, 0); //init counter to zero
	return 0;
}

static int tim_release(struct inode *inode, struct file *filp)
{
	printk(KERN_ERR"%s\n", __func__);

    del_timer(&dev_timer->s_timer);
	return 0;
}
/**
 * protatype : static inline int copy_to_user(void __user volatile *to, const void *from,
			       unsigned long n)
 * @param  flip    [description]
 * @param  buff    [description]
 * @param  counter [description]
 * @param  fops    [description]
 * @return         [description]
 */
static ssize_t tim_read(struct file * flip, char __user * buff, size_t counter,
                        loff_t * fops)
{
	printk(KERN_ERR"%s\n", __func__);
	return 0;
}
/*-----------------------------------------------------------------------------------
* @Description : write function for linux driver lever
*                apllication lever ---> ssize_t write(int fd, const void *buf, size_t count);
* @Param Description :
*    (struct file * flip) :
*    (const char __user * buff) :
*    (size_t count) : set counter max is four
*    (loff_t * fops) :
* @Return value :
------------------------------------------------------------------------------------*/
static ssize_t tim_write(struct file * flip, const char __user * buff,
                         size_t counter, loff_t * fops)
{
	printk(KERN_ERR"%s\n", __func__);
	return 0;

}
/*------------ fops end ------------------------------------------*/
// #define init_timer(timer)	__init_timer((timer), 0)
static int __init timer_init(void)
{
	dev_t device_devt;
	int major;
	int ret;

	major = DEV_MAJOR;

	device_devt = MKDEV(major, 0);
	if (major > 0) {
		ret = register_chrdev_region(device_devt, 1, device_name);
	} else {
		ret = alloc_chrdev_region(&device_devt, 0, 1, device_name);
	}
	if (ret < 0) {
		err_msg("register char device error");
		goto out;
	}
	dev_timer = (struct _timer_dev *)kzalloc(sizeof(struct _timer_dev), GFP_KERNEL);
	if (!dev_timer) {
		err_msg("kzmalloc error");
		ret = -ENOMEM;
		goto err_alloc;
	}

	dev_timer->dev_major = major;
	dev_timer->evt       = MKDEV(major, 0);
	cdev_init(&dev_timer->dev, &fops);
	dev_timer->dev.owner = THIS_MODULE;
	ret = cdev_add(&dev_timer->dev, dev_timer->evt, 1);
	if (ret) {
		err_msg("cdev add failed.");
		goto err_dev_add;
	}
	//------------ char device class create start -----------------
	dev_timer->dev_class = class_create(THIS_MODULE, class_name);
	if (IS_ERR(dev_timer->dev_class))
	{
		err_msg("class create error");
		ret = PTR_ERR(dev_timer->dev_class);
		goto err_class_create;
	}
	//------------ char device class create end -----------------
	//------------ char device devices create start -------------
	usr_msg("ready to create device in path /dev/.");
	dev_timer->dev_device = device_create(dev_timer->dev_class, NULL, dev_timer->evt, NULL, "%s", device_name);
	if (IS_ERR(dev_timer->dev_device))
	{
		err_msg("device create error");
		ret = PTR_ERR(dev_timer->dev_device);
		goto err_device_create;
	}
	usr_msg("module has been created.");
	//------------ char device devices create end ----------------
	//---------------- division line ----------------------
err_device_create:
	class_destroy(dev_timer->dev_class);
err_class_create:
	cdev_del(&dev_timer->dev);
err_dev_add:
	kfree(dev_timer);
err_alloc:
	unregister_chrdev_region(device_devt, 1);
out:
	return ret;
}

static void __exit timer_exit(void)
{
	usr_msg("ready to remove driver");
	device_destroy(dev_timer->dev_class, dev_timer->evt);
	class_destroy(dev_timer->dev_class);
	cdev_del(&dev_timer->dev);
	unregister_chrdev_region(dev_timer->evt, 1);
	kfree(dev_timer);
	usr_msg("driver has been removed");
}

module_init(timer_init);
module_exit(timer_exit);

//module_param(node_major, int, S_IRUGO);

MODULE_AUTHOR("QUAN");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("kernel timmer test");