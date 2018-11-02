/*----------------------------------------------------------------------*
 * Data :
 * using struct cdev to register char device
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
#include <linux/device.h>
#include <linux/kmalloc.h>
#include <linux/major.h>
#include <linux/cdev.h>
#include <linux/export.h>

/*--------------------- macro defined start --------------------------*/
#define usr_msg(args)                                       \
		do{                                                 \
				printk(KERN_NOTICE "---> %s\n", args);      \
		}while(0)

#define err_msg(args)                                           \
		do{                                                     \
				printk(KERN_ERR "-------> %s\n", args);         \
		}while(0)
/*--------------------- macro defined end --------------------------*/
/*------------ global parameter declaration start -------------------*/
static struct class *dev_class; 
static struct device *dev_device;
static const char *device_name = "simple_char_dev";
static unsigned int dev_major = 0;


static const struct file_operations dev_fops = {
		.owner = THIS_MODULE,
		.read = dev_read,
		.write = dev_write,
		.open = dev_open,
		.release = dev_release,
};

static struct _dev_info {
		struct cdev dev;
};
static struct _dev_info *dev_info;


/*------------ global parameter declaration end -------------------*/

static int dev_open(struct inode *inode, struct file *filp)
{
    usr_msg("device open");
		return 0;
}

static int dev_release(struct inode *inode, struct file *filp)
{
    usr_msg("device close");

		return 0;
}

static ssize_t dev_read(struct file *filp, char __user * buf, size_t size,
				loff_t * ppos)
{
    usr_msg("device read");
		return 0;
}

static ssize_t dev_write(struct file *filp, const char __user * buf,
				size_t size, loff_t * ppos)
{
    usr_msg("device write");

}


static int __init dev_init(void)
{
    
    int ret = 0;
    dev_t dev_no = MKDEV(dev_major, 0);

    if (dev_major) {
        register_chrdev_region(dev_no, 1, device_name);
    } else {
        ret = alloc_chrdev_region(&dev_no, 0, 1);
        if (ret < 0) {
            err_msg("alloc_chrdev_region error");
            goto out;
        }
        dev_major = MAJOR(dev_no);
    }

    dev_info = kmalloc(sizeof(struct _dev_info), GFP_KERNEL);
    if (PTR_ERR(dev_info)) {
        err_msg("kmalloc error");
        ret = -ENOMEM;
        goto err_kmalloc;
        
    }
    
    //-------- set device info, fill struct cdev ------
    cdev_init(&dev_info->dev, &dev_fops);
    //cdev_init has memset(cdev, 0, sizeof *cdev);
    dev_info->dev.owner = THIS_MODULE;
    ret = cdev_add(&dev_info->dev, dev_no, 1);
    if(ret) {
        err_msg("cdev add error");
        goto err_cdev_add;
    }
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
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("test char device driver");

