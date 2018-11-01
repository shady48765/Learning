/*
 * a simple char device driver: test_module without mutex
 *
 * Copyright (C) 2014 Barry Song  (baohua@kernel.org)
 *
 * Licensed under GPLv2 or later.
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/printk.h>

#define GLOBALMEM_SIZE	0x1000
#define MEM_CLEAR 0x1
#define GLOBALMEM_MAJOR 230

static int test_module_major = GLOBALMEM_MAJOR;
module_param(test_module_major, int, S_IRUGO);

struct test_module_dev {
	struct cdev cdev;
	unsigned char mem[GLOBALMEM_SIZE];
};

struct test_module_dev *test_module_devp;

static int test_module_open(struct inode *inode, struct file *filp)
{
	filp->private_data = test_module_devp;
	return 0;
}

static int test_module_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static long test_module_ioctl(struct file *filp, unsigned int cmd,
			    unsigned long arg)
{
	struct test_module_dev *dev = filp->private_data;

	switch (cmd) {
	case MEM_CLEAR:
		memset(dev->mem, 0, GLOBALMEM_SIZE);
		printk(  "test_module is set to zero\n");
		break;

	default:
		return -EINVAL;
	}

	return 0;
}

static ssize_t test_module_read(struct file *filp, char __user * buf, size_t size,
			      loff_t * ppos)
{
	unsigned long p = *ppos;
	unsigned int count = size;
	int ret = 0;
	struct test_module_dev *dev = filp->private_data;

	if (p >= GLOBALMEM_SIZE)
		return 0;
	if (count > GLOBALMEM_SIZE - p)
		count = GLOBALMEM_SIZE - p;

	if (copy_to_user(buf, dev->mem + p, count)) {
		ret = -EFAULT;
	} else {
		*ppos += count;
		ret = count;

		printk(  "read %u bytes(s) from %lu\n", count, p);
	}

	return ret;
}

static ssize_t test_module_write(struct file *filp, const char __user * buf,
			       size_t size, loff_t * ppos)
{
	unsigned long p = *ppos;
	unsigned int count = size;
	int ret = 0;
	struct test_module_dev *dev = filp->private_data;

	if (p >= GLOBALMEM_SIZE)
		return 0;
	if (count > GLOBALMEM_SIZE - p)
		count = GLOBALMEM_SIZE - p;

	if (copy_from_user(dev->mem + p, buf, count))
		ret = -EFAULT;
	else {
		*ppos += count;
		ret = count;

		printk(  "written %u bytes(s) from %lu\n", count, p);
	}

	return ret;
}

static loff_t test_module_llseek(struct file *filp, loff_t offset, int orig)
{
	loff_t ret = 0;
	switch (orig) {
	case 0:
		if (offset < 0) {
			ret = -EINVAL;
			break;
		}
		if ((unsigned int)offset > GLOBALMEM_SIZE) {
			ret = -EINVAL;
			break;
		}
		filp->f_pos = (unsigned int)offset;
		ret = filp->f_pos;
		break;
	case 1:
		if ((filp->f_pos + offset) > GLOBALMEM_SIZE) {
			ret = -EINVAL;
			break;
		}
		if ((filp->f_pos + offset) < 0) {
			ret = -EINVAL;
			break;
		}
		filp->f_pos += offset;
		ret = filp->f_pos;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	return ret;
}

static const struct file_operations test_module_fops = {
	.owner = THIS_MODULE,
	.llseek = test_module_llseek,
	.read = test_module_read,
	.write = test_module_write,
	.unlocked_ioctl = test_module_ioctl,
	.open = test_module_open,
	.release = test_module_release,
};

static void test_module_setup_cdev(struct test_module_dev *dev, int index)
{
	int err, devno = MKDEV(test_module_major, index);

	cdev_init(&dev->cdev, &test_module_fops);
	dev->cdev.owner = THIS_MODULE;
	err = cdev_add(&dev->cdev, devno, 1);
	if (err)
		printk(KERN_NOTICE "Error %d adding test_module%d", err, index);
}

static int __init test_module_init(void)
{
	int ret;
	dev_t devno = MKDEV(test_module_major, 0);
	printk("---> test module init process\n");
	if (test_module_major)
		ret = register_chrdev_region(devno, 1, "test_module");
	else {
		ret = alloc_chrdev_region(&devno, 0, 1, "test_module");
		test_module_major = MAJOR(devno);
	}
	if (ret < 0)
		return ret;

	test_module_devp = kzalloc(sizeof(struct test_module_dev), GFP_KERNEL);
	if (!test_module_devp) {
		ret = -ENOMEM;
		goto fail_malloc;
	}

	test_module_setup_cdev(test_module_devp, 0);
	printk("---> test module init process exit\n");
	return 0;

 fail_malloc:
	unregister_chrdev_region(devno, 1);
	printk(  "---> init process finished\n");
	return ret;
}

static void __exit test_module_exit(void)
{
	printk(  "test module exit\n");
	cdev_del(&test_module_devp->cdev);
	kfree(test_module_devp);
	unregister_chrdev_region(MKDEV(test_module_major, 0), 1);
}
module_init(test_module_init);
module_exit(test_module_exit);

MODULE_AUTHOR("QUAN");
MODULE_LICENSE("GPL v2");
