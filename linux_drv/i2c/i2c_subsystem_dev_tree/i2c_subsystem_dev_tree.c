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
/*--------------------- macro defined end --------------------------*/
/*------------ global parameter declaration start -------------------*/
#define USR_PLT_DRV          "usr_platform_drv"

#define CHR_IOC_MAGIC        'G'

/*------------ global parameter declaration end -------------------*/
static int usr_open(struct inode *inode, struct file *filp);
static int usr_release(struct inode *inode, struct file *filp);
static ssize_t usr_read(struct file * flip, char __user * buff, 
						size_t counter, loff_t * fops);
static ssize_t usr_write(struct file * flip, const char __user * buff,
                         size_t counter, loff_t * fops);
static long usr_ioctl (struct file *flip, unsigned int cmd, 
						unsigned long param);
/*----------------------- ctrl_cdev_ioctl -------------------------------*/

static int usr_open(struct inode *inode, struct file *filp)
{
	usr_msg("device open and init");

	// driver init function code
	return 0;
}

static int usr_release(struct inode *inode, struct file *filp)
{
	usr_msg("device close and restore the device default set");
	// device restore code
	return 0;
}
/**
 * [usr_read description]
 * @param  flip    [description]
 * @param  buff    [description]
 * @param  counter [description]
 * @param  fops    [description]
 * @return         [description]
 */
static ssize_t usr_read(struct file * flip, char __user * buff, 
						size_t counter, loff_t * fops)
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
static ssize_t usr_write(struct file * flip, const char __user * buff,
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
static long usr_ioctl (struct file *flip, unsigned int cmd, 
						unsigned long param)
{
    usr_msg("device usr_ioctl");
	return 0;
}

/*------------ device parameter declaration start -------------------*/
static const char *device_name     = "test_char_dev";
static const char *device_cls_name = "test_chrdev_cls";
static unsigned int node_major     = 0;

static const struct file_operations dev_fops = {
	.read           = usr_read,
	.write          = usr_write,
	.open           = usr_open,
	.release        = usr_release,
	.unlocked_ioctl = usr_ioctl,
};

struct _dev_info {
	struct cdev dev;
	dev_t evt;
	unsigned int dev_major;
	struct class * dev_class;
	struct device * dev_device;
};
static struct _dev_info *dev_info;


/*------------ device parameter declaration end -------------------*/



static int usr_probe (struct platform_device * pdev)
{
    
}

static int usr_remove (struct platform_device * pdev)
{
    
}
/*--- common platform driver using device tree allocation --------*/
static const of_device_id platform_device_match_table[] = {
    {.compatible = "usr_driver_i2c",},
    {},
};

static struct platform_driver usr_platform_drv = {
    .probe = usr_probe,
    .remove = usr_remove,
    .driver {
        .owner = THIS_MODULE,
        .name = USR_PLT_DRV,
        .of_match_table = platform_device_match_table,
    },
};

MODULE_DEVICE_TABLE(of, platform_device_match_table);

module_platform_driver(usr_platform_drv);

module_init(usr_i2c_init);
module_exit(ues_i2c_exit);

//module_param(node_major, int, S_IRUGO);

MODULE_AUTHOR("QUAN");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("test i2c device driver");

