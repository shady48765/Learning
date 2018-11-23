/*
 * a simple char device driver: misc_register without mutex
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

static int misc_register_major = GLOBALMEM_MAJOR;
module_param(misc_register_major, int, S_IRUGO);

struct misc_register_dev {
	struct cdev cdev;
	unsigned char mem[GLOBALMEM_SIZE];
};

struct misc_register_dev *misc_register_devp;

static int misc_register_open(struct inode *inode, struct file *filp)
{
	filp->private_data = misc_register_devp;
	return 0;
}

static int misc_register_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static long misc_register_ioctl(struct file *filp, unsigned int cmd,
			    unsigned long arg)
{
	struct misc_register_dev *dev = filp->private_data;

	switch (cmd) {
	case MEM_CLEAR:
		memset(dev->mem, 0, GLOBALMEM_SIZE);
		printk(  "misc_register is set to zero\n");
		break;

	default:
		return -EINVAL;
	}

	return 0;
}

static ssize_t misc_register_read(struct file *filp, char __user * buf, size_t size,
			      loff_t * ppos)
{
	unsigned long p = *ppos;
	unsigned int count = size;
	int ret = 0;
	struct misc_register_dev *dev = filp->private_data;

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

static ssize_t misc_register_write(struct file *filp, const char __user * buf,
			       size_t size, loff_t * ppos)
{
	unsigned long p = *ppos;
	unsigned int count = size;
	int ret = 0;
	struct misc_register_dev *dev = filp->private_data;

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

static loff_t misc_register_llseek(struct file *filp, loff_t offset, int orig)
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

static const struct file_operations misc_register_fops = {
	.owner = THIS_MODULE,
	.llseek = misc_register_llseek,
	.read = misc_register_read,
	.write = misc_register_write,
	.unlocked_ioctl = misc_register_ioctl,
	.open = misc_register_open,
	.release = misc_register_release,
};

static void misc_register_setup_cdev(struct misc_register_dev *dev, int index)
{
	int err, devno = MKDEV(misc_register_major, index);

	cdev_init(&dev->cdev, &misc_register_fops);
	dev->cdev.owner = THIS_MODULE;
	err = cdev_add(&dev->cdev, devno, 1);
	if (err)
		printk(KERN_NOTICE "Error %d adding misc_register%d", err, index);
}

static int __init misc_register_init(void)
{
}

static void __exit misc_register_exit(void)
{
}
module_init(misc_register_init);
module_exit(misc_register_exit);

MODULE_AUTHOR("QUAN");
MODULE_LICENSE("GPL v2");
