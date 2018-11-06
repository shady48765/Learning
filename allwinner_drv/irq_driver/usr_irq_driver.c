/*----------------------------------------------------------------------*
 * Data : 2018-10-02
 * using struct cdev to register char device
 * creat class for device
 * fill read, write, ioctl, close
 *
 *
 *----------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/printk.h>
#include <linux/major.h>
#include <linux/device.h>


/*--------------------- macro defined start --------------------------*/
#define usr_msg(args)                                           \
            do{                                                 \
                    printk(KERN_ERR "---> %s\n", args);         \
            }while(0)

#define err_msg(args)                                           \
            do{                                                 \
                    printk(KERN_ERR "-------> %s\n", args);     \
            }while(0)
/*--------------------------- macro defined end ---------------------------*/
/*----------------- global parameter declaration start --------------------*/
#define CHR_IOC_MAGIC        'G'

/*----------------- global parameter declaration end ----------------------*/

/*----------------------- device fops start -------------------------------*/

static int dev_open(struct inode *inode, struct file *filp)
{
	usr_msg("device open and init");

	// driver init function code
	return 0;
}

static int dev_release(struct inode *inode, struct file *filp)
{
	usr_msg("device close and restore the device default set");
	// device restore code
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
static ssize_t dev_read(struct file * flip, char __user * buff, size_t counter,
                        loff_t * fops)
{
	usr_msg("device read");

	//copy_to_user();
	usr_msg("device read finished.");
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
static ssize_t dev_write(struct file * flip, const char __user * buff,
                         size_t counter, loff_t * fops)
{
	int value;
	int ret;
	
	usr_msg("device write");
	if (counter < 0 || counter > 4)
		return -EINVAL;
	/**
	 * prototype : copy_from_user(void *to, const void __user *from, unsigned long size)
	 * parameter to : copy user data to. void * format
	 * parameter from : copy user data from. normally is a buff
	 * parameter size : numbers of data need to be copied
	 * return value : 0 for success, > 0 for error
	 *
	 */
	ret = copy_from_user(&value, buff, counter);
	if (ret > 0)
	{
		err_msg("copy_from_user error");
		return -EFAULT;
	}
	usr_msg("start to switch copied data.");
	switch (value)
	{
	case 0:
		usr_msg("value is : 0");
		break;
	case 1:
		usr_msg("value is : 1");
		break;
	case 2:
		usr_msg("value is : 2");
		break;
	case 3:
		usr_msg("value is : 3");
		break;
	default:
		usr_msg("Out of handle range");
	}
	usr_msg("device write finished.");
	// return data has been copied
	return counter;

}
static long dev_ioctl (struct file *flip, unsigned int cmd, unsigned long param)
{
    usr_msg("device dev_ioctl");
	return 0;
}
/*----------------------- device fops end -------------------------------*/
/*---------------- device parameter declaration start -------------------*/
static const char *device_name     = "test_char_dev";
static const char *device_cls_name = "test_chrdev_cls";
static unsigned int node_major     = 0;

static const struct file_operations dev_fops = {
	.read           = dev_read,
	.write          = dev_write,
	.open           = dev_open,
	.release        = dev_release,
	.unlocked_ioctl = dev_ioctl,
};

struct _dev_info {
	struct cdev dev;
	dev_t evt;
	unsigned int dev_major;
	struct class * dev_class;
	struct device * dev_device;
};
static struct _dev_info *dev_info;


/*--------------------- device parameter declaration end -------------------*/


static int __init dev_init(void)
{

	int ret = 0;
	dev_t dev_no;

	//----------- char device creat start --------------
	usr_msg("module init start");

	dev_no = MKDEV(node_major, 0);

	if (node_major) {
		register_chrdev_region(dev_no, 1, device_name);
	} else {
		ret = alloc_chrdev_region(&dev_no, 0, 1, device_name);
		if (ret < 0) {
			err_msg("alloc_chrdev_region error");
			goto out;
		}
		node_major = MAJOR(dev_no);
	}

	dev_info = kmalloc(sizeof(struct _dev_info), GFP_KERNEL);
	if (!dev_info) {
		err_msg("kmalloc error");
		ret = -ENOMEM;
		goto err_kmalloc;

	}
	// store node_major to device info structure
	dev_info->dev_major = node_major;
	dev_info->evt = MKDEV(dev_info->dev_major, 0);

	//-------- set device info, fill struct cdev ------
	cdev_init(&dev_info->dev, &dev_fops);
	//cdev_init has memset(cdev, 0, sizeof *cdev);
	dev_info->dev.owner = THIS_MODULE;
	ret = cdev_add(&dev_info->dev, dev_no, 1);
	if (ret) {
		err_msg("cdev add error");
		ret = -ENOMEM;
		goto err_cdev_add;
	}
	usr_msg("module init successed.");
	//----------- char device creat finished ------------------
	//----------- char device class create start --------------
	/*------------------------------------------------------------------
	// add char_device_driver to class
	#define class_create(owner, name)
	--------------------------------------------------------------------*/
	usr_msg("ready to create device class.");
	dev_info->dev_class = class_create(THIS_MODULE, device_cls_name);
	if (IS_ERR(dev_info->dev_class))
	{
		err_msg("class create error");
		ret = PTR_ERR(dev_info->dev_class);
		goto err_class_create;
	}
	//----------- char device class create end ----------------
	//----------- char device devices create start --------------
	/*------------------------------------------------------------------
	struct device *device_create(struct class *class, struct device *parent,
			     dev_t devt, void *drvdata, const char *fmt, ...)
	#endif
	--->create /dev/char_device_driver point
	-----------------------------------------------------------------------*/
	usr_msg("ready to create device in path /dev/.");
	dev_info->dev_device = device_create(dev_info->dev_class, NULL, dev_no, NULL, "%s", device_name);
	if (IS_ERR(dev_info->dev_device))
	{
		err_msg("device create error");
		ret = PTR_ERR(dev_info->dev_device);
		goto err_device_create;
	}
	usr_msg("module has been created.");
	//----------- char device devices create end ----------------

    return 0;

//---------------- division line ----------------------
err_irq:
    device_destroy(dev_info->dev_class, dev_info->evt);
err_device_create:
	class_destroy(dev_info->dev_class);
err_class_create:
	cdev_del(&dev_info->dev);
err_cdev_add:
	kfree(dev_info);
err_kmalloc:
	unregister_chrdev_region(dev_no, 1);
out:
	return ret;

}

static void __exit dev_exit(void)
{
	usr_msg("ready to remove driver");
	device_destroy(dev_info->dev_class, dev_info->evt);
	class_destroy(dev_info->dev_class);
	cdev_del(&dev_info->dev);
	unregister_chrdev_region(dev_info->evt, 1);
	kfree(dev_info);
	usr_msg("driver has been removed");
}
module_init(dev_init);
module_exit(dev_exit);

module_param(node_major, int, S_IRUGO);

MODULE_AUTHOR("QUAN");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("test char device driver");


#if 0
[Tue Nov  6 09:21:01 2018] full_func_chrdev: exports duplicate symbol dev_open (owned by kernel)
-------------------------------------------------------------------------------------------------
EXPORT_SYMBOL(dev_write);
EXPORT_SYMBOL(dev_open);
EXPORT_SYMBOL(dev_read);
EXPORT_SYMBOL(dev_release);
EXPORT_SYMBOL(dev_init);
EXPORT_SYMBOL(dev_exit);
#endif

/*----------------------- ctrl_cdev_ioctl -------------------------------*/
// static long ctrl_cdev_ioctl(struct file *file, unsigned int cmd,
// 			    unsigned long arg)
// {
// int err = 0;
// void __user *argp = (void __user *)arg;

// if (!capable(CAP_SYS_RESOURCE))
// 	return -EPERM;

// switch (cmd) {
// /* Attach an MTD device command */
// case UBI_IOCATT:
// {
// 	struct ubi_attach_req req;
// 	struct mtd_info *mtd;

// 	dbg_gen("attach MTD device");
// 	err = copy_from_user(&req, argp, sizeof(struct ubi_attach_req));
// 	if (err) {
// 		err = -EFAULT;
// 		break;
// 	}

// 	if (req.mtd_num < 0 ||
// 	    (req.ubi_num < 0 && req.ubi_num != UBI_DEV_NUM_AUTO)) {
// 		err = -EINVAL;
// 		break;
// 	}

// 	mtd = get_mtd_device(NULL, req.mtd_num);
// 	if (IS_ERR(mtd)) {
// 		err = PTR_ERR(mtd);
// 		break;
// 	}

// 	/*
// 	 * Note, further request verification is done by
// 	 * 'ubi_attach_mtd_dev()'.
// 	 */
// 	mutex_lock(&ubi_devices_mutex);
// 	err = ubi_attach_mtd_dev(mtd, req.ubi_num, req.vid_hdr_offset,
// 				 req.max_beb_per1024);
// 	mutex_unlock(&ubi_devices_mutex);
// 	if (err < 0)
// 		put_mtd_device(mtd);
// 	else
// 		/* @err contains UBI device number */
// 		err = put_user(err, (__user int32_t *)argp);

// 	break;
// }

// /* Detach an MTD device command */
// case UBI_IOCDET:
// {
// 	int ubi_num;

// 	dbg_gen("detach MTD device");
// 	err = get_user(ubi_num, (__user int32_t *)argp);
// 	if (err) {
// 		err = -EFAULT;
// 		break;
// 	}

// 	mutex_lock(&ubi_devices_mutex);
// 	err = ubi_detach_mtd_dev(ubi_num, 0);
// 	mutex_unlock(&ubi_devices_mutex);
// 	break;
// }

// default:
// 	err = -ENOTTY;
// 	break;
// }

// return err;
//
// }
