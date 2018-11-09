#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>

/*--------------------- macro defined start --------------------------*/
#define DEV_DRV_NAME                "demo_dts_gpio_drv"
#define DEVICE_NAME                 "demo_gpio_dev"

#define usr_msg(args)                                           \
            do{                                                 \
                    printk(KERN_ERR "---> %s\n", args);         \
            }while(0)

#define err_msg(args)                                           \
            do{                                                 \
                    printk(KERN_ERR "-------> %s\n", args);     \
            }while(0)
/*--------------------------- macro defined end ---------------------------*/

static int gpio_dev_open (struct inode * node, struct file * files);
static ssize_t gpio_dev_write (struct file *files, const char __user * buff, 
                                size_t, loff_t * lofft);
static long gpio_dev_iotcl (struct file *files, unsigned int cmd, unsigned long args);
static int gpio_dev_release (struct inode * node, struct file * files);




static const struct file_operations gpio_fops = {
    .owner = THIS_MODULE,
    .open = gpio_dev_open,
    .write = gpio_dev_write,
    .release = gpio_dev_release,
    .unlocked_ioctl = gpio_dev_iotcl,
};
#if 0
static struct miscdevice misc_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &gpio_fops,
};
#endif
struct _gpio_info {
    struct miscdevice *gpio_misc_dev;
};





static int gpio_dev_open (struct inode * node, struct file * files)
{
    usr_msg("gpio port open.");
    return 0;
}
static ssize_t gpio_dev_write (struct file *filp, const char __user * buff, 
                                size_t size, loff_t * lofft)
{
    struct _gpio_info * gpio_info = container_of(filp->private_data, struct _gpio_info, gpio_misc_dev);
    

    
    return 0;
}
static long gpio_dev_iotcl (struct file *files, unsigned int cmd, unsigned long args)
{
	return 0;
}
static int gpio_dev_release (struct inode * node, struct file * files)
{
    // !!!!!!!!!!!!!! struct gpio_chip
	return 0;
}



static int gpio_pd_probe(struct platform_device * pd_dev)
{
    int ret;
    struct _gpio_info * gpio_info;
    gpio_info = kmalloc(sizeof(struct _gpio_info), GFP_KERNEL);
    if(!gpio_info) {
        err_msg("kzmalloc error.");
        ret = -ENOMEM;
        goto err_kzmalloc;
    }
    
    gpio_info->gpio_misc_dev->name = DEVICE_NAME;
    gpio_info->gpio_misc_dev->minor = MISC_DYNAMIC_MINOR;
    gpio_info->gpio_misc_dev->fops = &gpio_fops;
    
    ret = misc_register(gpio_info->gpio_misc_dev);
    if (0 != ret) {
        err_msg("misc register error.");
        goto err_misc_register;
    }
    err_misc_register:
        misc_deregister(gpio_info->gpio_misc_dev);
        kfree(gpio_info);
    err_kzmalloc:
        return ret;
}


static int gpio_pd_remove(struct platform_device * pd_dev)
{
    struct _gpio_info * gpio_info = platform_get_drvdata(pd_dev);
    misc_deregister(gpio_info->gpio_misc_dev);
    kfree(gpio_info);
    return 0;
}







struct of_device_id laser_pin_match_table[] = {
    {.compatible = "nano, laser-pin",},
    {.compatible = "nano, laserflash-pin",},
    {},
};
MODULE_DEVICE_TABLE(of, laser_pin_match_table);

struct platform_driver gpio_pd_drv = {
    .driver = {
        .name = DEV_DRV_NAME,
        .owner = THIS_MODULE,
        .of_match_table = laser_pin_match_table,
    },
    .probe = gpio_pd_probe,
    .remove = gpio_pd_remove,
};

module_platform_driver(gpio_pd_drv);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("QUAN");
MODULE_DESCRIPTION("gpio pin dts test");
