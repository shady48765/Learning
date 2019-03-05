/**
 * @Decription: High time timer
 * @Declaration: deceted aliyun server HZ = 250
 *              1. jiffies increase 250 per second
 *              2. high resolution timer can refer the pmic.c file
*/


#include "my_timer_hrts.h"
#include <linux/time.h>	            // for get system current time
// for hr-timer
#include <linux/hrtimer.h>
#include <linux/sched.h>
#include <linux/ktime.h>

#define MS_TO_NS(x) (x * 1E6L)      // ms to ns

static ktime_t  tim_period;


static enum hrtimer_restart foo_hrtimer_callback(struct hrtimer * arg)
{
    ktime_t now = arg->base->get_time();
    usr_msg("timer running at jiffies=%ld\n", jiffies);
    hrtimer_forward(arg, now, tim_period);
    return HRTIMER_RESTART;
}


int foo_timer_init(void)
{
    int retval = 0;

    mutex_lock(&foo_mutex);
    // ktime_set(const s64 secs, const unsigned long nsecs); // param1: second, param2:nanosecond
    tim_period = ktime_set(0, MS_TO_NS(1000));     // set 1second, 1000 nanosecond.
    hrtimer_init(&foo_timer, CLOCK_REALTIME, HRTIMER_MODE_REL);
    foo_timer.function = foo_hrtimer_callback;
    hrtimer_start(&foo_timer, tim_period, HRTIMER_MODE_REL);
    mutex_unlock(&foo_mutex);


    return retval;
}

static int timer_open(struct inode *inode, struct file *filp)
{
    usr_msg( "open");
    return 0;
}

static int timer_close(struct inode *inode, struct file *filp)
{
    usr_msg( "close");
    return 0;
}

static ssize_t timer_write(struct file *flip, const char __user *buff,
                         size_t counter, loff_t *fops)
{
    usr_msg( "write");
    return 0;
}

static ssize_t timer_read (struct file *flip, char __user *buff,
                            size_t counter, loff_t *fops)
{
    usr_msg( "read");
    return 0;
}
long timer_ioctl (struct file *flip, unsigned int cmd, unsigned long param)
{
    usr_msg( "ioctl");
    return 0;
}


static int __init timer_jiffy_init(void)
{
    int ret;
    mutex_init(&foo_mutex);
    usr_msg("timer_jiffy start");

    foo_dev_info = kmalloc(sizeof(struct my_cdev), GFP_KERNEL);
    if(!foo_dev_info) {
        err_msg("kzmalloc error");
        ret = -ENOMEM;
        return ret;
    }
    memset(foo_dev_info, 0, sizeof(struct my_cdev));

    ret = alloc_chrdev_region(&foo_dev_info->foo_dev_number, 0, 1, FOO_DEV_NAME);
    if(ret < 0) {
        err_msg("alloc_chrdev_region error");
        ret = -ENODEV;
        goto err_alloc_chrdev;
    }
    foo_dev_info->foo_major_number = MAJOR(foo_dev_info->foo_dev_number);
    usr_msg("timer_jiffy, device_numver = %d", foo_dev_info->foo_major_number);

    // foo_cdev cannot init when it's a point,
    // cdev_init(&foo_dev_info->foo_cdev...) cannot succese init
    cdev_init(&foo_dev_info->foo_cdev, &foo_fops);
    foo_dev_info->foo_cdev.owner = THIS_MODULE;

    ret = cdev_add(&foo_dev_info->foo_cdev, foo_dev_info->foo_dev_number, 1);
    if(ret < 0) {
        err_msg("cdev_add error");
        ret = -ENOMEM;
        goto err_cdev_add;
    }
    ret = foo_device_create();
    if(ret < 0)
        goto err_foo_device_create;

    usr_msg("timer_jiffy create success");



    // timer, if only init, can only run once foo_timer_callback
    // every timer run foo_timer_callback, timer need to bu update!
    ret = foo_timer_init();
    if(0 != ret) {
        err_msg("timer init failed");
    }
    usr_msg("timer init succeed, current jiffies = %ld", jiffies);

    return ret;

err_foo_device_create:
    cdev_del(&foo_dev_info->foo_cdev);
err_cdev_add:
    unregister_chrdev_region(foo_dev_info->foo_dev_number, 1);
err_alloc_chrdev:
    kfree(foo_dev_info);
    return ret;
}

int foo_device_create(void)
{
    int ret;
    foo_dev_info->foo_class = class_create(THIS_MODULE, FOO_DEV_NAME);
    if(IS_ERR(foo_dev_info->foo_class)) {
        err_msg("class_create error");
        ret = PTR_ERR(foo_dev_info->foo_class);
        goto err_class_create;
    }
    foo_dev_info->foo_device = device_create(foo_dev_info->foo_class, NULL, foo_dev_info->foo_dev_number, NULL, "%s", FOO_DEV_NAME);
    if(IS_ERR(foo_dev_info->foo_device)) {
        err_msg("device_create error");
        ret = PTR_ERR(foo_dev_info->foo_device);
        goto err_device_create;
    }
    usr_msg("timer_jiffy device create success");
    return 0;

err_device_create:
    class_destroy(foo_dev_info->foo_class);
err_class_create:
    return ret;


}

static void __exit timer_jiffy_exit(void)
{
    int retval;

    device_destroy(foo_dev_info->foo_class, foo_dev_info->foo_dev_number);
    class_destroy(foo_dev_info->foo_class);
    cdev_del(&foo_dev_info->foo_cdev);
    unregister_chrdev_region(foo_dev_info->foo_dev_number, 1);
    kfree(foo_dev_info);

    retval = hrtimer_cancel(&foo_timer);
    if (retval)
        err_msg("The timer is still in use...\n");
    usr_msg("Timer module unloaded\n");
    usr_msg("timer_jiffy exit success");
}

module_init(timer_jiffy_init);
module_exit(timer_jiffy_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("quan");
MODULE_DESCRIPTION("kernel jiffy timer example.");
MODULE_ALIAS("usr_HZ");
