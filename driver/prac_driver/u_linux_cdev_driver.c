#include <linux/cdev.h>
#include <linux/error.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>

#define MAJOR_NUMBER 	257
#define MEM_SIZE 		100

static int td_major = MAJOR_NUMBER;

struct file_operations fops = {
    .open = fs_open,
    .write = fs_write,
    .release = fs_close,
    .read = fs_read,
    .unlocked_ioctl = fs_ioctl,
}
struct _device_info_ {
    struct cdev cdev;
    struct file_operations fops;
    dev_t dev_no;
	
	unsigned char mem[MEM_SIZE]; 
};
struct _device_info *device_info;


static void device_setup_cdev (struct _device_info_ *dev, int index)
{

	dev->cdev.owner = THIS_MODULE;
	dev->cdev.name = "test_cdev_module";
	int err = cdev_add(&dev->cdev, dev->dev_no, 1)
	if(err)
		printk("ERROR : %s\n", __func__);
}

static int __init test_device_init(void)
{
	int ret;
	dev_t devno = MKDEV(td_major, 0);
	if(td_major)
		ret = register_chrdev_region(devno, 1, "test_device");
	else {
		ret = alloc_chrdev_region(&devno, 0, 1, "test_device");
	}
	if(ret < 0)
		return ret;

	//---------------------------------------------------------------
	device_info = kzalloc(sizeof(struct _device_info_), GPL_KERNEL);
	if(!device_info) {
		ret = -ENOMEM;
		goto err_no_memory;
	}
	device_info->dev_no = devno;
	device_setup_cdev(device_info, 0);
	return 0;

	/** error deal protocal ------------------------------------*/
	err_no_memory:
		unregister_chrdev_region(dev_no, 1);
		return ret;
    
}



int fs_open (struct inode *, struct file *)
{

}
int fs_close (struct inode *, struct file *)
{

}
ssize_t fs_read (struct file *, char __user *, size_t, loff_t *)
{

}
ssize_t fs_write (struct file *, const char __user *, size_t, loff_t *)
{

}
long fs_ioctl (struct file *, unsigned int, unsigned long)
{

}



//EXPORT_SYMBOL(device_init);
EXPORT_SYMBOL_GPL(test_device_init);

static void __exit test_device_exit(void)
{
}

EXPORT_SYMBOL_GPL(test_device_exit);








moudle_init(device_init);
moudle_exit(device_exit);


MOUDLE_LICENSE("GPL v2");
MOUDLE_ALIASE("basic driver");
MOUDLE_AUTHOR();









