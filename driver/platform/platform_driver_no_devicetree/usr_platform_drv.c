/*----------------------------------------------------------------------*
 * Data : 2018-10-07
 * Function: register a char device as a platform device
 * 
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
#include <linux/device.h>
#include <linux/platform_device.h>


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
/*------------ global parameter declaration start -------------------------*/
/*------------ function declaration start --------------------*/
static int dev_open(struct inode *inode, struct file *filp);
static int dev_release(struct inode *inode, struct file *filp);
static ssize_t dev_read(struct file * flip, char __user * buff, 
                        size_t counter, loff_t * fops);
static ssize_t dev_write(struct file * flip, const char __user * buff,
                         size_t counter, loff_t * fops);
static long dev_ioctl (struct file *flip, unsigned int cmd, unsigned long data);
static int pd_probe(struct platform_device *pdev);
static int pd_remove(struct platform_device * pdev);

/*------------ function declaration end ----------------------*/
/*------------ global struct and param declaration start -----------*/
#define CHR_IOC_MAGIC        'G'

#define DEV_NAME         "test_char_dev"
#define DEV_CLS_NAME     "test_chrdev_cls"
#define PLT_NAME         "test_chrdev_platform"
 
static unsigned int node_major     = 0;
// --- platform ---
/**
struct platform_driver {
	int (*probe)(struct platform_device *);
	int (*remove)(struct platform_device *);
	void (*shutdown)(struct platform_device *);
	int (*suspend)(struct platform_device *, pm_message_t state);
	int (*resume)(struct platform_device *);
	struct device_driver driver;
	const struct platform_device_id *id_table;
	bool prevent_deferred_probe;
};
struct platform_device {
	const char	*name;
	int		id;
	bool		id_auto;
	struct device	dev;
	u32		num_resources;
	struct resource	*resource;

	const struct platform_device_id	*id_entry;
};
**/
// struct device_driver {
// 	const char		*name;
// 	struct bus_type		*bus;

// 	struct module		*owner;
// 	const char		*mod_name;	/* used for built-in modules */

// 	bool suppress_bind_attrs;	/* disables bind/unbind via sysfs */
// 	enum probe_type probe_type;

// 	const struct of_device_id	*of_match_table;
// 	const struct acpi_device_id	*acpi_match_table;

// 	int (*probe) (struct device *dev);
// 	int (*remove) (struct device *dev);
// 	void (*shutdown) (struct device *dev);
// 	int (*suspend) (struct device *dev, pm_message_t state);
// 	int (*resume) (struct device *dev);
// 	const struct attribute_group **groups;

// 	const struct dev_pm_ops *pm;

// 	struct driver_private *p;
// };
static const struct platform_device_id pd_driver_id [] = {
{.name = "test_plt_drv",            .driver_data = 0xff01},
{.name = "test_platform_drv",       .driver_data = 0xff02},
{.name = "no_devtree_platform",    .driver_data = 0xff03},
};

struct platform_driver pd_driver = {
    .driver = {
        .owner = THIS_MODULE,
        .name  = PLT_NAME,
     },
    .probe = pd_probe,
    .remove = pd_remove,
    .id_table = pd_driver_id,
    
};

// --- device ---
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


/*------------ global struct and param declaration end -------*/

/*------------ global parameter declaration end --------------------------*/

/*------------ file operations start -------------------------------*/

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
/**
 * protatype : static inline int copy_to_user(void __user volatile *to, const void *from,
			       unsigned long n)
 * @param  flip    [description]
 * @param  buff    [description]
 * @param  counter [description]
 * @param  fops    [description]
 * @return         [description]
 */
static ssize_t dev_read(struct file * flip, char __user * buff, 
                            size_t counter, loff_t * fops)
{
	usr_msg("device read");

	//copy_to_user();
	usr_msg("device read finished.");
	return 0;
}
/*-----------------------------------------------------------------------------------
* @Description : write function for linux driver lever
*                apllication lever ---> ssize_t write(int fd, const void *buf, size_t count);
* @Param Description :
*    (struct file * flip) :
*    (const char __user * buff) :
*    (size_t count) : set counter max is four
*    (loff_t * fops) :
* @Return value :
------------------------------------------------------------------------------------*/
static ssize_t dev_write(struct file * flip, const char __user * buff,
                         size_t counter, loff_t * fops)
{
	int value;
	int ret;
	
	usr_msg("device write");
	if (counter < 0 || counter > 4)
		return -EINVAL;
	/**
	 * prototype : copy_from_user(void *to, const void __user *from, unsigned long size)
	 * parameter to : copy user data to. void * format
	 * parameter from : copy user data from. normally is a buff
	 * parameter size : numbers of data need to be copied
	 * return value : 0 for success, > 0 for error
	 *
	 */
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
static long dev_ioctl (struct file *flip, unsigned int cmd, unsigned long data)
{
    usr_msg("device dev_ioctl");
	return 0;
}

static int pd_probe(struct platform_device *pdev)
{
	int ret = 0;
	dev_t dev_no;

	//----------- char device creat start --------------
	usr_msg("module init start");

	dev_no = MKDEV(node_major, 0);

	if (node_major) {
		register_chrdev_region(dev_no, 1, DEV_NAME);
	} else {
		ret = alloc_chrdev_region(&dev_no, 0, 1, DEV_NAME);
		if (ret < 0) {
			err_msg("alloc_chrdev_region error");
			goto out;
		}
		node_major = MAJOR(dev_no);
	}

	dev_info = kmalloc(sizeof(struct _dev_info), GFP_KERNEL);
	//dev_info = kmalloc(sizeof(struct _dev_info), GFP_ATOMIC);
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
	//----------- char device creat finished ------------------
	//----------- char device class create start --------------
	/*------------------------------------------------------------------------------------------
	// add char_device_driver to class
	#define class_create(owner, name)
	-------------------------------------------------------------------------------------------*/
	usr_msg("ready to create device class.");
	dev_info->dev_class = class_create(THIS_MODULE, DEV_CLS_NAME);
	if (IS_ERR(dev_info->dev_class))
	{
		err_msg("class create error");
		ret = PTR_ERR(dev_info->dev_class);
		goto err_class_create;
	}
	//----------- char device class create end ----------------
	//----------- char device devices create start --------------
	/*------------------------------------------------------------------------------------------
	struct device *device_create(struct class *class, struct device *parent,
			     dev_t devt, void *drvdata, const char *fmt, ...)

	--->create /dev/char_device_driver point
	-------------------------------------------------------------------------------------------*/
	usr_msg("ready to create device in path /dev/.");
	dev_info->dev_device = device_create(dev_info->dev_class, NULL, dev_no, NULL, "%s", DEV_NAME);
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

static int pd_remove(struct platform_device * pdev)
{
	usr_msg("ready to remove driver");
	device_destroy(dev_info->dev_class, dev_info->evt);
	class_destroy(dev_info->dev_class);
	cdev_del(&dev_info->dev);
	unregister_chrdev_region(dev_info->evt, 1);
	kfree(dev_info);
	usr_msg("driver has been removed");
    return 0;
}

static int __init platform_drv_init(void)
{
	int ret;
	usr_msg("platform_driver_register");
    ret = platform_driver_register(&pd_driver);
    return ret;
}
static void __exit platform_drv_exit(void)
{
	usr_msg("platform_driver_unregister");
    platform_driver_unregister(&pd_driver);
}
module_init(platform_drv_init);
module_exit(platform_drv_exit);

module_param(node_major, int, S_IRUGO);

MODULE_AUTHOR("QUAN");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("platform char device test");

//module_platform_driver(pd_driver);

