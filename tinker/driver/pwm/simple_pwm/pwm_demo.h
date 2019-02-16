#ifndef __SIMPLE_PWM_H
#define __SIMPLE_PWM_H

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
#include <linux/mutex.h>
#include <linux/delay.h>

#define TIMER_TAG                   " <PWM> "
#define USR_MSG_LEVEL               KERN_ERR
#define USR_ERR_LEVEL               KERN_ERR
#define usr_msg(fmt, args...)       printk(USR_MSG_LEVEL TIMER_TAG ""fmt"\n",##args)
#define err_msg(fmt, args...)       printk(USR_ERR_LEVEL TIMER_TAG " (function : %s), [line : %d] "fmt"\n",__func__, __LINE__, ##args)



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

#define USR_PWM_DRV_NAME                "pwm_demo"
#define USED_HRS_TIMER					1
#define TRANDITIONAL_WAY				0


static struct   timer_list  foo_time;
static struct   timeval     old_tmval;

static struct file_operations foo_fops = {
    .read           = timer_read,
    .write          = timer_write,
    .open           = timer_open,
    .release        = timer_close,
    .unlocked_ioctl = timer_ioctl,
};

struct dts_info {
	unsigned int freq;
	unsigned int plus_width;
	unsigned int status;
};

struct pwm_dev {
    unsigned int    pwm_major_num;
    dev_t           pwm_dev_num;
    struct cdev     pwm_cdev;
    struct class    *pwm_class;
    struct device   *pwm_device;
	struct mutex    lock;
	struct dts_info info;
};

#endif
