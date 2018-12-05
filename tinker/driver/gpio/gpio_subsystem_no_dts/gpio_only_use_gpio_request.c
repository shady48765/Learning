/**
 * 
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/printk.h>
#include <linux/major.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/delay.h>

// #define GPIO_IRQ

static unsigned int laser_pin = 171;

static const char *device_name     = "laser_name";
static const char *device_cls_name = "laser_cls";
static unsigned int node_major     = 0;

/*--------------------- macro defined start --------------------------*/
#define usr_msg(args)                                           \
            do{                                                 \
                    printk(KERN_ERR "---> %s\n", args);         \
            }while(0)

#define err_msg(args)                                           \
            do{                                                 \
                    printk(KERN_ERR "-------> %s\n", args);     \
            }while(0)

#define CHR_IOC_MAGIC        'G'

typdef enum {
    off = 0,
    on = 1
}_enmu_gpio_status;
static _enmu_gpio_status gpio_pin_status;

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
/*------------ global parameter declaration end -------------------*/
static int dev_open(struct inode *inode, struct file *filp) 
{
    int ret;

    usr_msg("device open");
    if (!gpio_is_valid(laser_pin)) {
    prinkt(KERN_ERR "--------> [laser pin is not valid] (%s)\n", __func__);
    return -ENODEV;
    }
    ret = gpio_request(laser_pin, "laser");
    if(ret = 0) {
        prinkt(KERN_ERR "--------> [laser pin request failed] (%s)\n", __func__);
    }
    gpio_direction_output(laser_pin);
    gpio_set_value(laser_pin, gpio_pin_status.on);


    return 0;
}

static int dev_release(struct inode *inode, struct file *filp)
{
    usr_msg("device close");
    gpio_free(laser_pin);

    return 0;
}

static ssize_t dev_write(struct file * flip, const char __user * buff,
                         size_t counter, loff_t * fops)
{
    int value;
    int ret;
    int index;
    
    usr_msg("device write");
    if (counter < 0 || counter > 4)
        return -EINVAL;
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
        gpio_set_value(laser_pin, gpio_pin_status.on);
        break;
    case 1:
        usr_msg("value is : 1");
        gpio_set_value(laser_pin, gpio_pin_status.off);
        break;
    case 2:
        usr_msg("value is : 2");
        for(index = 0; index < 3; index++) {
            gpio_set_value(laser_pin, gpio_pin_status.on);
            mdelay(2000);
            gpio_set_value(laser_pin, gpio_pin_status.off);
            mdelay(2000);
        }
        break;
    default:
        usr_msg("Out of handle range");
    }
    usr_msg("device write finished.");
    // return data has been copied
    return counter;

}


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
    gpio_free(laser_pin);
    usr_msg("driver has been removed");
}
module_init(dev_init);
module_exit(dev_exit);

module_param(node_major, int, S_IRUGO);

MODULE_AUTHOR("QUAN");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("test char device driver");