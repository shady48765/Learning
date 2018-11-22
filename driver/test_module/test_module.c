#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/printk.h>
#include <linux/device.h>
#include <linux/printk.h>

#define user_msg(args)                                \
        do{                                           \
                printk(KERN_NOTICE "%s\n", args);     \
        }while(0)


#define MODULE_MAJOR 232

static int dev_major = MODULE_MAJOR; 

static int dev_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int dev_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static ssize_t dev_read(struct file *filp, char __user * buf, size_t size,
			      loff_t * ppos)
{
#if 0

	unsigned long p = *ppos;
	unsigned int count = size;
	int ret = 0;

	struct dev_dev *dev = filp->private_data;

	if (p >= GLOBALMEM_SIZE)
		return 0;
	if (count > GLOBALMEM_SIZE - p)
		count = GLOBALMEM_SIZE - p;

	if (copy_to_user(buf, dev->mem + p, count)) {
		ret = -EFAULT;
	} else {
		*ppos += count;
		ret = count;

		printk("read %u bytes(s) from %lu\n", count, p);
	}

	return ret;
#endif
    return 0;
}

static ssize_t dev_write(struct file *filp, const char __user * buf,
			       size_t size, loff_t * ppos)
{
#if 0

	unsigned long p = *ppos;
	unsigned int count = size;
	int ret = 0;

	struct dev_dev *dev = filp->private_data;

	if (p >= GLOBALMEM_SIZE)
		return 0;
	if (count > GLOBALMEM_SIZE - p)
		count = GLOBALMEM_SIZE - p;

	if (copy_from_user(dev->mem + p, buf, count))
		ret = -EFAULT;
	else {
		*ppos += count;
		ret = count;

		printk("written %u bytes(s) from %lu\n", count, p);
	}

	return ret;
#endif
    return 0;

}





#if 0
static void dev_setup_cdev(struct dev_dev *dev, int index)
{
	int err, devno = MKDEV(dev_major, index);

	cdev_init(&dev->cdev, &dev_fops);
	dev->cdev.owner = THIS_MODULE;
	err = cdev_add(&dev->cdev, devno, 1);
	if (err)
		printk(KERN_NOTICE "Error %d adding dev%d", err, index);
    
}
#endif

/*------------ global parameter declaration start -------------------*/
static struct class *dev_class; 
static struct device *dev_device;
static const char *device_name = "test_module";
    
struct _dev_info {
    struct cdev cdev;
};
struct _dev_info *dev_info;

static const struct file_operations dev_fops = {
    .owner = THIS_MODULE,
    .read = dev_read,
    .write = dev_write,
    .open = dev_open,
    .release = dev_release,
};

/*------------ global parameter declaration end -------------------*/

static int __init dev_init(void)
{
	int ret;

    
    user_msg("init module");
    ret = register_chrdev(dev_major, device_name, &dev_fops);
    if (ret < 0) {
        printk(KERN_ERR "register device error <%s>\n", __func__);
        goto err_register_dev;
    }
    dev_info = kmalloc(sizeof(struct _dev_info), GFP_KERNEL);
    if(!dev_info) {
    	printk(KERN_ERR "kmalloc error <%s>", __func__);
	goto err_kmalloc;
    }

    dev_class = class_create(dev_fops.owner, "test_module_class");
    if(IS_ERR(dev_class)) {
       printk(KERN_ERR "creat class error <%s>\n", __func__);
       goto err_class_create;
    }

    dev_device = device_create(dev_class, NULL, MKDEV(dev_major, 0), NULL, "%s_%d", device_name, 1);
    if(IS_ERR(dev_device))
    {
        printk(KERN_ERR "device_creat error <%s>\n", __func__);
        goto err_device_creat;
    }
    user_msg("module init success");
    return 0;
    
    err_device_creat:
        class_destroy(dev_class);
    err_class_create:
        unregister_chrdev(dev_major, device_name);
	err_kmalloc:	
        kfree(dev_info);
    err_register_dev:
        return ret;
    
}

static void __exit dev_exit(void)
{
    user_msg("module exit process");

    device_destroy(dev_class, MKDEV(dev_major, 0));
    class_destroy(dev_class);
    unregister_chrdev(dev_major, 0);
}
module_init(dev_init);
module_exit(dev_exit);

module_param(dev_major, int, S_IRUGO);

MODULE_AUTHOR("QUAN");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("test char device driver");

