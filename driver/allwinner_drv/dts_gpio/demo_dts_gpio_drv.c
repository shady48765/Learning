#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>      /* For platform devices */
#include <linux/interrupt.h>            /* For IRQ */
#include <linux/gpio.h>                 /* For Legacy integer based GPIO */
#include <linux/of_gpio.h>              /* For of_gpio* functions */
#include <linux/of.h>                   /* For DT*/
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/of_device.h>

/*--- global param declartion start ----------------*/
/*--- macro defined start --------------------------*/
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

static int laser_misc_open (struct inode * node, struct file * files);
static ssize_t laser_misc_write (struct file *files, const char __user * buff, 
                                size_t, loff_t * lofft);
static long laser_misc_ioctl (struct file *files, unsigned int cmd, unsigned long args);
static int laser_misc_release (struct inode * node, struct file * files);


/*--- global param declartion end ----------------*/
#if 0
struct pinctrl *pinctrl0;
struct pinctrl_state *pins_default;
struct pinctrl_state *laser;
#endif

static int laser_misc_open (struct inode * node, struct file * files)
{
    usr_msg("gpio port open.");
    nonseekable_open(node, files);
    return 0;
}
static ssize_t laser_misc_write (struct file *filp, const char __user * buff, 
                                size_t size, loff_t * lofft)
{
    usr_msg("gpio port open.");
    
    mdelay(500);
    //struct _gpio_info * gpio_info = container_of(filp->private_data, struct _gpio_info, gpio_misc_dev);
    

    
    return 0;
}
static long laser_misc_ioctl (struct file *files, unsigned int cmd, unsigned long args)
{
	return 0;
}
static int laser_misc_release (struct inode * node, struct file * files)
{
    
	return 0;
}

#include <linux/pinctrl/consumer.h>
#include <linux/miscdevice.h>
#include <linux/pinctrl/pinctrl.h>

struct laser_pin_info {
struct pinctrl *pin;
struct pinctrl_state *status;
};
struct laser_pin_info *pin_info;
    
static const struct file_operations laser_fops = {
    .owner = THIS_MODULE,
    .open = laser_misc_open,
    .write = laser_misc_write,
    .release = laser_misc_release,
    .unlocked_ioctl = laser_misc_ioctl,
};
struct miscdevice laser_miscdev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &laser_fops,
};

static int gpio_pd_probe(struct platform_device * pd_dev)
{
    int ret;
        
    ret = misc_register(&laser_miscdev);
    if(!ret) {
        printk(KERN_ERR "error : misc_register failed.\n");
        return ret;
    }

    struct device *dev = &pd_dev->dev;
        
    /* Allocate a state holder named "laser_pin_info" etc */
    pin_info = kmalloc(sizeof(struct laser_pin_info), GFP_KERNEL);
    if(!pin_info) {
        err_msg("error : malloc pin_info");
        ret = -ENOMEM;
        goto err_kmalloc;
    }

    pin_info->pin = devm_pinctrl_get(dev);
    if (IS_ERR(pin_info->pin)) {
        err_msg("error : cannot find device pin");
        return PTR_ERR(pin_info->pin);
    }

    pin_info->status = pinctrl_lookup_state(pin_info->pin, PINCTRL_STATE_DEFAULT);
    if (IS_ERR(pin_info->status)) {
        printk(KERN_ERR "error : cat not find pin %s status\n", PINCTRL_STATE_DEFAULT);
        return PTR_ERR(pin_info->status);
    }

    ret = pinctrl_select_state(pin_info->pin, pin_info->status);
    if (ret < 0) {
            printk(KERN_ERR "error : cat not set pin status.\n");
            return ret;
    }

    

err_kmalloc:
    misc_deregister(&laser_miscdev);
    return ret;
}


static int gpio_pd_remove(struct platform_device * pd_dev)
{
    //struct _gpio_info * gpio_info = platform_get_drvdata(pd_dev);
    misc_deregister(&laser_miscdev);
    //kfree(gpio_info);
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
        .of_match_table = of_match_ptr(laser_pin_match_table),
    },
    .probe = gpio_pd_probe,
    .remove = gpio_pd_remove,
};

module_platform_driver(gpio_pd_drv);

MODULE_AUTHOR("V");
MODULE_DESCRIPTION("device tree gpio driver test");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:" DEV_DRV_NAME);