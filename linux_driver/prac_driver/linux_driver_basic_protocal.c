/**
 * 最基本注册流程
 * 通用流程
 * 自动获得设备号和设备节点
 * 1. moudle_init
 * 2. register_chrdev() or int alloc_chrdev_region();
 * 3. class_creat(THIS_MODULE, "class_name")
 * 4. device_creat()
 * 5. finish struct file_operations
 * register_chrdev()为经典方法，新一点的内核用 static void scull_setup_cdev(struct scull_dev, int index)
 */
#include <linux/init.h>
#include <linux/moudle.h>
#include <linux/fs.h>
#include <linux/device.h>

const struct file_operations device_fops = {
	.open = device_open,
	.read = device_read,
	.write = device_write,
	.release = device_close,
};
static unsigned int dev_major = 0;
static struct class * dev_class;
static int __init device_init(void)
{
	printk("---> module init, %s \n", __FUNCTION__);
	int ret;
	// 1. 向系统申请主设备号, 自动申请
	dev_major = register_chrdev(0, "char_device", &device_fops);
	if (dev_major < 0)
	{
		printk("---> register_chrdev error\n");
		return -1;
	}
	printf("---> register_chrdev success, dev_major = %d\n", dev_major);

	// 2. 将设备归类, THIS_MODULE 表示模块为当前模块创建的
	dev_class = class_creat(THIS_MODULE, "device_class");
	// 3. 在当前类中创建设备文件， 位于 /dev目录下
	/*------------------------------------------------
	* 参数1: 表示设备文件在哪个类别
	* 参数2: 表示当前设备文件的父对象，一般为NULL
	* 参数3： 设备文件的编号
	* 参数4: 设备的私有数据，一般为NULL
	* 参数5: 设备节点的名字
	-------------------------------------------------*/
	// 只使用了主设备号
	device_creat(device_class, NULL, MKDEV(dev_major, 0), NULL, "device%d", 0);
	printk("---> device creat finished\n");
}

static void __exit device_exit(void)
{
	printk("---> moudle delete\n");

	// 参数1:设备类别
	// 参数2： 设备号
	device_destory(device_class, MKDEV(dev_major, 0));
	class_destory(dev_class);

	// 释放主设备号
	unregister_chrdev(dev_major, "char_device");
}

moudle_init(device_init);
moudle_exit(device_exit);
MOUDLE_LICENSE("GPL");

// compare to application layer open(fd, O_RW)
int device_open(struct inode *node, struct file * filp)
{
	printk("---> %s\n", __FUNCTION__);
	return 0;
}
// compare to application layer read(fd, buff, size)
ssize_t device_read(struct file * filp, char __user * buff, size_t count, loff_t * fops)
{
	printk("---> %s\n", __FUNCTION__);
	return 0;
}
// compare to application layer write(fd, buff, size)
ssize_t device_write(struct file * filp, const char __user *buff, size_t count, loff_t *fops)
{
	printk("---> %s\n", __FUNCTION__);
	return 0;
}
int device_close(struct inode * node, struct file * filp)
{
	printk("---> %s\n", __FUNCTION__);
	return 0;
}


/*------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------*/
/***********************************************************************************
 * 基本LED驱动
 * ioctl, allocate device , register_chardev_region
 * 
 ************************************************************************************/

#include <linux/init.h>
#include <linux/moudle.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/ioctl.h>
#include <linux/error.h>

const struct file_operations device_fops = {
	.open = device_open,
	.read = device_read,
	.write = device_write,
	.release = device_close,
};
static unsigned int dev_major = 0;
static struct class * dev_class;
static struct device * dev_ret;

static int __init device_init(void)
{
	int result;
	dev_t dev = MKDEV(dev_major, 0);

	printk("---> module init, %s \n", __FUNCTION__);

	// 1. 向系统申请设备号，dev_major有数值就直接注册
	if(dev_major) {
		result = register_chardev_region(dev, 1, "device1");
	}
	else {
		result = alloc_chrdev_region(&dev, 0, 1, "device1");
		dev_major = MAJOR(dev);
	}
	if(result < 0)
	{
		return result;
	}

}

















static void __exit device_exit(void)
{
	printk("---> moudle delete\n");

	// 参数1:设备类别
	// 参数2： 设备号
	device_destory(device_class, MKDEV(dev_major, 0));
	class_destory(dev_class);

	// 释放主设备号
	unregister_chrdev(dev_major, "char_device");
}

moudle_init(device_init);
moudle_exit(device_exit);
MOUDLE_LICENSE("GPL");

// compare to application layer open(fd, O_RW)
int device_open(struct inode * node, struct file * filp)
{
	printk("---> %s\n", __FUNCTION__);
	return 0;
}
// compare to application layer read(fd, buff, size)
ssize_t device_read(struct file * filp, char __user * buff, size_t count, loff_t * fops)
{
	printk("---> %s\n", __FUNCTION__);
	return 0;
}
// compare to application layer write(fd, buff, size)
ssize_t device_write(struct file * filp, const char __user * buff, size_t count, loff_t *fops)
{
	printk("---> %s\n", __FUNCTION__);
	return 0;
}
int device_close(struct inode * node, struct file * filp)
{
	printk("---> %s\n", __FUNCTION__);
	return 0;
}

/*------------------------------------------------------------------------------------------*/

/***********************************************************************************
 * platform 演示
 ***********************************************************************************/





