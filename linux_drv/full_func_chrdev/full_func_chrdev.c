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
#define CHR_IOC_MAGIC        'G'

/*------------ global parameter declaration start -------------------*/
/*----------------------- ctrl_cdev_ioctl -------------------------------*/
static long ctrl_cdev_ioctl(struct file *file, unsigned int cmd,
			    unsigned long arg)
{
	int err = 0;
	void __user *argp = (void __user *)arg;

	if (!capable(CAP_SYS_RESOURCE))
		return -EPERM;

	switch (cmd) {
	/* Attach an MTD device command */
	case UBI_IOCATT:
	{
		struct ubi_attach_req req;
		struct mtd_info *mtd;

		dbg_gen("attach MTD device");
		err = copy_from_user(&req, argp, sizeof(struct ubi_attach_req));
		if (err) {
			err = -EFAULT;
			break;
		}

		if (req.mtd_num < 0 ||
		    (req.ubi_num < 0 && req.ubi_num != UBI_DEV_NUM_AUTO)) {
			err = -EINVAL;
			break;
		}

		mtd = get_mtd_device(NULL, req.mtd_num);
		if (IS_ERR(mtd)) {
			err = PTR_ERR(mtd);
			break;
		}

		/*
		 * Note, further request verification is done by
		 * 'ubi_attach_mtd_dev()'.
		 */
		mutex_lock(&ubi_devices_mutex);
		err = ubi_attach_mtd_dev(mtd, req.ubi_num, req.vid_hdr_offset,
					 req.max_beb_per1024);
		mutex_unlock(&ubi_devices_mutex);
		if (err < 0)
			put_mtd_device(mtd);
		else
			/* @err contains UBI device number */
			err = put_user(err, (__user int32_t *)argp);

		break;
	}

	/* Detach an MTD device command */
	case UBI_IOCDET:
	{
		int ubi_num;

		dbg_gen("detach MTD device");
		err = get_user(ubi_num, (__user int32_t *)argp);
		if (err) {
			err = -EFAULT;
			break;
		}

		mutex_lock(&ubi_devices_mutex);
		err = ubi_detach_mtd_dev(ubi_num, 0);
		mutex_unlock(&ubi_devices_mutex);
		break;
	}

	default:
		err = -ENOTTY;
		break;
	}

	return err;
}
/*----------------------- ctrl_cdev_ioctl -------------------------------*/

static int dev_open(struct inode *inode, struct file *filp)
{
    usr_msg("device open and init");

    // init function code
    return 0;
}

static int dev_release(struct inode *inode, struct file *filp)
{
    usr_msg("device close and restore the device default set");
    // device restore code
    return 0;
}

static ssize_t dev_read(struct file * flip, char __user * buff, size_t counter, loff_t * fops)
{
    usr_msg("device read");
    
    return 0;
}
/*-----------------------------------------------------------------------------------
* @Description : write function for linux driver lever
*                apllication lever ---> ssize_t write(int fd, const void *buf, size_t count);
* @Param Description :
*    (struct file * flip) : 
*    (const char __user * buff) :
*    (size_t count) :
*    (loff_t * fops) :
* @Return value : 
------------------------------------------------------------------------------------*/
// 
// count
static ssize_t dev_write(struct file * flip, const char __user * buff, size_t counter, loff_t * fops)
{
	int ret;

    usr_msg("device write");






    
	if(count<0 || count != 4 )
		return -EINVAL;

	int value;
	//参数1--目的地
	//参数2--源头
	//参数3--拷贝的个数
	//返回值--返回拷贝不成功的数据个数，0表示成功，大于0表示出错
	ret = copy_from_user(&value, buf, count);
	if(ret > 0)
	{
		printk(KERN_ERR"copy_from_user error\n");
		return -EFAULT;
	}

	if(value)
	{
		//亮灯
		*gpc0data |= 0x3<<3;
	}else
	{
		//灭灯
		*gpc0data &= ~(0x3<<3);
	}

	//返回已经拷贝成功的数据个数
	return count;
    return 0;

}
static long dev_ioctl (struct file *, unsigned int, unsigned long);
/*------------ device parameter declaration start -------------------*/
static const char *device_name = "test_char_dev";
static unsigned int dev_major = 0;


static const struct file_operations dev_fops = {
    .read    = dev_read,
    .write   = dev_write,
    .open    = dev_open,
    .release = dev_release,
    .unlocked_ioctl   = dev_ioctl,
};

struct _dev_info {
    struct cdev dev;
};
struct _dev_info *dev_info;


/*------------ device parameter declaration end -------------------*/


static int __init dev_init(void)
{

    int ret = 0;

    usr_msg("module init start");
    dev_t dev_no = MKDEV(dev_major, 0);

    if (dev_major) {
        register_chrdev_region(dev_no, 1, device_name);
    } else {
        ret = alloc_chrdev_region(&dev_no, 0, 1, device_name);
        if (ret < 0) {
            err_msg("alloc_chrdev_region error");
            goto out;
        }
        dev_major = MAJOR(dev_no);
    }

    dev_info = kmalloc(sizeof(struct _dev_info), GFP_KERNEL);
    //dev_info = kmalloc(sizeof(struct _dev_info), GFP_ATOMIC);
    if (!dev_info) {
        err_msg("kmalloc error");
        ret = -ENOMEM;
        goto err_kmalloc;

    }

    //-------- set device info, fill struct cdev ------
    cdev_init(&dev_info->dev, &dev_fops);
    //cdev_init has memset(cdev, 0, sizeof *cdev);
    dev_info->dev.owner = THIS_MODULE;
    ret = cdev_add(&dev_info->dev, dev_no, 1);
    if (ret) {
        err_msg("cdev add error");
        goto err_cdev_add;
    }
    usr_msg("module init successed.");
//----------- char device creat finished --------------
    return 0;
//---------------- division line ----------------------
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
    cdev_del(&dev_info->dev);
    kfree(dev_info);
    unregister_chrdev_region(MKDEV(dev_major, 0), 1);
}
module_init(dev_init);
module_exit(dev_exit);

module_param(dev_major, int, S_IRUGO);

MODULE_AUTHOR("QUAN");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("test char device driver");

