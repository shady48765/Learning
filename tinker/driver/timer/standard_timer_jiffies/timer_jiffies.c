#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/cdev.h>


#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/printk.h>
#include <linux/major.h>
#include <linux/device.h>

#define DEV_NAME            "my_timer"

static struct file_operations foo_fops = {
    .read = timer_read,
    .write = timer_write,
    .open = timer_open,
    .release = timer_close,
};

static struct my_cdev {
    unsigned int    foo_major_number;
    dev_t           foo_dev_number;
    struct cdev     *foo_cdev;
    struct class    *foo_class;
    struct device   *foo_device;
};

struct my_cdev  * foo_dev_info;

static int __init timer_jiffy_init(void)
{
    int ret;
    foo_dev_info = kzmalloc(sizeof(struct my_cdev), GFP_KERNEL);
    if(!foo_dev_info) {
        printk(KERN_ERR "kzmalloc error, (%s) [%d]\n", __func__, __LINE__);
        return = -ENOMEM;
    }
    memset(foo_dev_info, 0, sizeof(struct my_cdev));
    
    ret = alloc_chrdev_region(&foo_dev_info->foo_dev_number, 0, 1, DEV_NAME);
    if(ret < 0) {
        printk(KERN_ERR "alloc_chrdev_region error, (%s) [%d]\n", __func__, __LINE__);
        ret = -ENODEV;
        goto err_alloc_chrdev;
    }
    foo_dev_info->foo_major_number = MAJOR(foo_dev_info->foo_dev_number);
    cdev_init(foo_dev_info->foo_cdev, &foo_dev_info->foo_fops);
    foo_dev_info->foo_cdev->owner = THIS_MODULE;
    ret = cdev_add(foo_dev_info->foo_cdev, foo_dev_info->foo_dev_number, 1);
    if(ret < 0) {
        printk(KERN_ERR "cdev_add error, (%s) [%d]\n", __func__, __LINE__);
        ret = -ENOMEM;
        goto err_cdev_add;
    }
    
    
    
err_cdev_add:
    unregister_chrdev_region(foo_dev_info->foo_dev_number, 1);
err_alloc_chrdev:
    kfree(foo_dev_info);
    return ret;
}

static void __exit timer_jiffy_exit(void)
{
    
}

module_init(timer_jiffy_init);
module_exit(timer_jiffy_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("quan");
MODULE_DESCRIPTION("kernel jiffy timer example.");
MODULE_ALIAS("usr_HZ");