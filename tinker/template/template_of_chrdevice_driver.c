#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/errno.h>

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/printk.h>
#include <linux/major.h>
#include <linux/device.h>

#define TIMER_TAG                   " <TIMER> "
#define USR_MSG_LEVEL               KERN_ERR
#define USR_ERR_LEVEL               KERN_ERR
#define usr_msg(fmt, args...)       printk(USR_MSG_LEVEL TIMER_TAG " (function : %s), [line : %d] "fmt"\n",__func__, __LINE__, ##args);
#define err_msg(fmt, args...)       printk(USR_ERR_LEVEL TIMER_TAG " (function : %s), [line : %d] "fmt"\n",__func__, __LINE__, ##args);

#define FOO_DEV_NAME                "my_timer"



int foo_device_create(void);
int foo_timer_init(void);

int foo_timer_init(void)
{
    
}

static int timer_open(struct inode *inode, struct file *filp)
{
    return 0;
}

static int timer_close(struct inode *inode, struct file *filp)
{
    return 0;
}

static ssize_t timer_write(struct file *flip, const char __user *buff,
                         size_t counter, loff_t *fops)
{
    return 0;
}

static ssize_t timer_read (struct file *flip, char __user *buff, 
                            size_t counter, loff_t *fops)
{
    return 0;
}

static struct file_operations foo_fops = {
    .read = timer_read,
    .write = timer_write,
    .open = timer_open,
    .release = timer_close,
};

static struct my_cdev {
    unsigned int    foo_major_number;
    dev_t           foo_dev_number;
    struct cdev     foo_cdev;
    struct class    *foo_class;
    struct device   *foo_device;
};

struct my_cdev  * foo_dev_info;

static int __init timer_jiffy_init(void)
{
    int ret;
    usr_msg("timer_jiffy start");
    foo_dev_info = kmalloc(sizeof(struct my_cdev), GFP_KERNEL);
    if(!foo_dev_info) {
        err_msg("kzmalloc error");
        ret = -ENOMEM;
        return ret;
    }
    memset(foo_dev_info, 0, sizeof(struct my_cdev));
    
    ret = alloc_chrdev_region(&foo_dev_info->foo_dev_number, 0, 1, FOO_DEV_NAME);
    if(ret < 0) {
        err_msg("alloc_chrdev_region error");
        ret = -ENODEV;
        goto err_alloc_chrdev;
    }
    foo_dev_info->foo_major_number = MAJOR(foo_dev_info->foo_dev_number);
    usr_msg("timer_jiffy, device_numver = %d", foo_dev_info->foo_major_number);

    // foo_cdev cannot init when it's a point, 
    // cdev_init(&foo_dev_info->foo_cdev...) cannot succese init
    cdev_init(&foo_dev_info->foo_cdev, &foo_fops);
    foo_dev_info->foo_cdev.owner = THIS_MODULE;
    
    ret = cdev_add(&foo_dev_info->foo_cdev, foo_dev_info->foo_dev_number, 1);
    if(ret < 0) {
        err_msg("cdev_add error");
        ret = -ENOMEM;
        goto err_cdev_add;
    }
    ret = foo_device_create();
    if(ret < 0)
        goto err_foo_device_create;
    
    usr_msg("timer_jiffy create success");
    return ret;

err_foo_device_create:
    cdev_del(&foo_dev_info->foo_cdev);
err_cdev_add:
    unregister_chrdev_region(foo_dev_info->foo_dev_number, 1);
err_alloc_chrdev:
    kfree(foo_dev_info);
    return ret;
}

int foo_device_create(void)
{
    int ret;
    foo_dev_info->foo_class = class_create(THIS_MODULE, FOO_DEV_NAME);
    if(IS_ERR(foo_dev_info->foo_class)) {
        err_msg("class_create error");
        ret = PTR_ERR(foo_dev_info->foo_class);
        goto err_class_create;
    }
    foo_dev_info->foo_device = device_create(foo_dev_info->foo_class, NULL, foo_dev_info->foo_dev_number, NULL, "%s", FOO_DEV_NAME);
    if(IS_ERR(foo_dev_info->foo_device)) {
        err_msg("device_create error");
        ret = PTR_ERR(foo_dev_info->foo_device);
        goto err_device_create;
    }
    usr_msg("timer_jiffy device create success");
    return 0;
    
err_device_create:
    class_destroy(foo_dev_info->foo_class);
err_class_create:
    return ret;

    
}

static void __exit timer_jiffy_exit(void)
{
    device_destroy(foo_dev_info->foo_class, foo_dev_info->foo_dev_number);
    class_destroy(foo_dev_info->foo_class);
    cdev_del(&foo_dev_info->foo_cdev);
    unregister_chrdev_region(foo_dev_info->foo_dev_number, 1);
    kfree(foo_dev_info);
    usr_msg("timer_jiffy exit success");
}

module_init(timer_jiffy_init);
module_exit(timer_jiffy_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("quan");
MODULE_DESCRIPTION("kernel jiffy timer example.");
MODULE_ALIAS("usr_HZ");