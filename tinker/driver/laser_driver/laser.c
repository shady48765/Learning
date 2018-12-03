/*----------------------------------------------------------------------*
 * Data : 2018-10-02
 * using struct cdev to register char device
 * creat class for device
 * fill read, write, ioctl, close
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
#include <linux/pinctrl/pinctrl.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>
#include <linux/err.h>


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

#define LASER_NAME			"laser"
#define NODE_FROM			NULL
struct device_node	* laser_node;



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


/*------------ global parameter declaration end -------------------*/

static int get_dts_info(const char * compat)
{
	int ret;
	ret = of_machine_is_compatible(LASER_NAME);
	if(ret < 0) {

	}

	laser_node = of_find_compatible_node(NODE_FROM, NULL, compat);
	if(!laser_node) {
		err_msg("malloc error");
		goto err_find_node;
	}


	return 0;

err_find_node:
	return ERR;

}


static int dev_open(struct inode *inode, struct file *filp)
{
	usr_msg("device open and init");

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



static int laser_probe(struct platform_device * laser_pdev)
{
	int index, flag, ret;
	struct device * dev = &laser_pdev->dev;
	struct device_node * laser_node = laser_pdev->dev.of_node;

	prinkt(KERN_ERR "-------> [move in laser probe] (%s)\n", __func__);

	laser_gpio = of_get_named_gpio_flags(device_node, LASER_NAME, 0, &flag);
	if(!gpio_is_valid(laser_gpio)) {
		prinkt(KERN_ERR "-------> gpio not valid\n");
		return -ERR;
	}
	ret = gpio_request(laser_gpio, LASER_NAME);
	if(ret != 0) {
		gpio_free(laser_gpio);
		return -EIO;
	}

	gpio _direction_output(laser_gpio, GPIO_HIGH);
	for(index = 0; index < 10; index++) {
		gpio_set_value(laser_gpio, GPIO_LOW);
		mdelay(10000);
		gpio_set_value(laser_gpio, GPIO_HIGH);
		mdelay(10000);
	}
	
	printk("[%s] [%d] \n", __func__, __LINE__);

	return 0;
}
static int laser_remove(struct platform_device * laser_pdev)
{
	gpio_free(laser_pin_info.laser_gpio);
	return 0;
}




static struct of_device_id laser_table[] = {
	{.compatible = "laser",},
	{ },
};
MODULE_DEVICE_TABLE(of, laser_table);


static struct platform_driver laser_driver = {
	.probe = laser_probe,
	.remove = laser_remove,
	.driver = {
		.name = "laser_driver",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(laser_driver),
	},
};

static int __init dev_init(void)
{
	int ret;
	printk(KERN_ERR "---------> move in function : %s", __func__);
	ret = platform_driver_register(&laser_driver);
	if(ret < 0) {
		printk(KERN_ERR "---------> function : %s", __func__);
	}
	printk(KERN_ERR "---------> move out function : %s", __func__);
	return 0;
}

static void __exit dev_exit(void)
{
	printk(KERN_ERR "---------> move in function : %s", __func__);
	platform_driver_unregister(&laser_driver);
	printk(KERN_ERR "---------> move out function : %s", __func__);
}




module_init(dev_init);
module_exit(dev_exit);

module_param(node_major, int, S_IRUGO);

MODULE_AUTHOR("QUAN");
MODULE_LICENSE("GPL");
MODULE_ALIAS("laser driver");
MODULE_DESCRIPTION("test char device driver");
