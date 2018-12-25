#ifndef __FOO_PROC_H
#define __FOO_PROC_H

#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/printk.h>
#include <linux/major.h>
#include <linux/device.h>
#include <linux/timer.h>        //for timer_list, jiffy timer, standard timer
#include <linux/raid/pq.h>
#include <linux/mutex.h>

#define TIMER_TAG                   " <TIMER> "
#define USR_MSG_LEVEL               KERN_ERR
#define USR_ERR_LEVEL               KERN_ERR
#define usr_msg(fmt, args...)       printk(USR_MSG_LEVEL TIMER_TAG " (function : %s), [line : %d] "fmt"\n",__func__, __LINE__, ##args);
#define err_msg(fmt, args...)       printk(USR_ERR_LEVEL TIMER_TAG " (function : %s), [line : %d] "fmt"\n",__func__, __LINE__, ##args);

#define FOO_DEV_NAME                "my_timer"

/**--------------------- function define start ---------------------------*/
static int timer_open(struct inode *inode, struct file *filp);
static int timer_close(struct inode *inode, struct file *filp);
static ssize_t timer_write(struct file *flip, const char __user *buff,
                                size_t counter, loff_t *fops);
static ssize_t timer_read (struct file *flip, char __user *buff, 
                                size_t counter, loff_t *fops);
long timer_ioctl (struct file *flip, unsigned int cmd, unsigned long param);

int foo_device_create(void);
int foo_timer_init(void);
void foo_timer_callback(unsigned long args);
int foo_proc_create(void);

/**--------------------- function define end ---------------------------*/


struct mutex    foo_mutex;

static struct   timer_list  foo_time;
static struct   timeval     old_tmval;

static struct file_operations foo_fops = {
    .read           = timer_read,
    .write          = timer_write,
    .open           = timer_open,
    .release        = timer_close,
    .unlocked_ioctl = timer_ioctl,
};

static struct my_cdev {
    unsigned int    foo_major_number;
    dev_t           foo_dev_number;
    struct cdev     foo_cdev;
    struct class    *foo_class;
    struct device   *foo_device;
};
struct my_cdev  * foo_dev_info;

#endif
