/**
 * using gpio subsystem to control gpio
 * not contain interrupt, using gpio subsystem to request gpio and get the gpio status
 *
 *
 * dts for this file with platform tinker board s
 * 	// file : rk3288-miniarm.dts
 *   laser {
 *               compatible = "laser";
 *               label = "laser prot";
 *
 *               laser-on = <&gpio5 RK_PC3 GPIO_ACTIVE_HIGH>;
 *               laser-off = <&gpio5 RK_PC3 GPIO_ACTIVE_LOW>;
 *               status = "okay";
 *   };
 */

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/gpio.h> 					/* For Legacy integer based GPIO */
#include <linux/init.h>
#include <linux/interrupt.h> 				/* For IRQ */
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/module.h>
#include <linux/of.h> 						/* For DT*/
#include <linux/of_gpio.h> 					/* For of_gpio* functions */
// #include <linux/pinctrl/pinctrl.h>
#include <linux/platform_device.h> 			/* For platform devices */
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

/*--------------------- macro defined start --------------------------*/
#define usr_msg(args)                           \
	do                                          \
	{                                           \
		printk(KERN_ERR "-------> %s\n", args); \
	} while (0)

#define err_msg(args)                           \
	do                                          \
	{                                           \
		printk(KERN_ERR "-------> %s\n", args); \
	} while (0)
/*--------------------- macro defined end --------------------------*/
/*------------ global parameter declaration start -------------------*/
#define CHR_IOC_MAGIC 'G'

#define DRIVER_NAME				"laser_driver"
#define LASER_NAME				"laser"
#define NODE_FROM				NULL
// struct device_node	* laser_node;

typedef enum {
	on = 1,
	off = 0
}_gpio_status_enum;
_gpio_status_enum gpio_status;


static struct _laser_pin_info {
	unsigned int laser_gpio;
};
static struct _laser_pin_info laser_pin_info;

static const struct file_operations dev_fops = {
	.read           = dev_read,
	.write          = dev_write,
	.open           = dev_open,
	.release        = dev_release,
	.unlocked_ioctl = dev_ioctl,
};

struct _dev_info
{
	struct cdev dev;
	dev_t evt;
	unsigned int dev_major;
	struct class *dev_class;
	struct device *dev_device;
};
static struct _dev_info *dev_info;



static int get_dts_info(const char * compat);





static int dev_open(struct inode *inode, struct file *filp)
{
	usr_msg("device open and init");

	// gpio_request();
	// driver init function code
	return 0;
}

static int dev_release(struct inode *inode, struct file *filp)
{
	usr_msg("device close and restore the device default set");
	// device restore code
	return 0;
}

static ssize_t dev_read(struct file *flip, char __user *buff, size_t counter,
						loff_t *fops)
{
	usr_msg("device read");

	//copy_to_user();
	usr_msg("device read finished.");
	return 0;
}

static ssize_t dev_write(struct file *flip, const char __user *buff,
						 size_t counter, loff_t *fops)
{
	int value;
	int ret;

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
static long dev_ioctl(struct file *flip, unsigned int cmd, unsigned long param)
{
	usr_msg("device dev_ioctl");
	return 0;
}


/**
 * @Descripthon : laser_probe
 * 		using gpio subsystem to request gpio and get the gpio status
 */
static int laser_probe(struct platform_device * laser_pdev)
{
	int index, flag, ret;
	printk(KERN_INFO "[%s] [%d] \n", __func__, __LINE__);

	struct device_node * laser_node = laser_pdev->dev.of_node;
	if(!laser_node) {
          return -ENOENT;
    }
    laser_pin_info.laser_gpio = of_get_named_gpio(laser_node, "laser", 0);

    gpio_request(laser_pin_info, "laser_pin");
    gpio_direction_output(laser_pin_info.laser_pin);
    gpio_set_value(laser_pin_info.laser_pin, gpio_status.on);
    mdelay(1000);
    
	printk(KERN_INFO "[%s] [%d] \n", __func__, __LINE__);

	return 0;
}
static int laser_remove(struct platform_device * laser_pdev)
{
	gpio_free(laser_pin_info.laser_gpio);
	err_msg("laser_remove");
	return 0;
}

static struct of_device_id laser_table[] = {
    {
        .compatible = "laser",
    },
    {/*sentinel*/},
};
MODULE_DEVICE_TABLE(of, laser_table);


static struct platform_driver laser_driver = {
	.probe = laser_probe,
	.remove = laser_remove,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(laser_driver),
	},
};


static int __init gpio_init(void)
{
	int ret;
	printk(KERN_INFO "---------> move in function (%s)\n", __func__);
	ret = platform_driver_register(&laser_driver);
	if(ret < 0) {
		printk(KERN_ERR "---------> platform_driver_register error\n");
	}
	printk(KERN_INFO "---------> move out function (%s)\n", __func__);
	return 0;
}

static void __exit gpio_exit(void)
{
	printk(KERN_INFO "---------> move in function : %s", __func__);
	platform_driver_unregister(&laser_driver);
	printk(KERN_INFO "---------> move out function : %s", __func__);
}




module_init(gpio_init);
module_exit(gpio_exit);

module_param(node_major, int, S_IRUGO);

MODULE_AUTHOR("QUAN");
MODULE_LICENSE("GPL");
MODULE_ALIAS("laser driver");
MODULE_DESCRIPTION("gpio platform driver");
