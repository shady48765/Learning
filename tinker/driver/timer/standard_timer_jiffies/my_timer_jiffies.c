#include "my_timer_jiffies.h"
#include <linux/time.h>	// for get system current time



/**
 * @Description:
 *          timer counter recall function.
 * @Attention: 
 *          need to reload/init timer counter every tick cycle
 * 
 */
void foo_timer_callback(unsigned long arg)
{
    struct timeval tm_val;
    static int loop_counter = 0;
    
    usr_msg("schedule time arrived, jiffies = %lu", jiffies);
    char * strs = (char *) arg;
    usr_msg("got transferred args is : %s", strs);
    // get current time
    do_gettimeofday(&tm_val);
    // calculator interval_time
    usr_msg("interval time is %ld sencond", tm_val.tv_sec - old_tmval.tv_sec);
    usr_msg("interval time is %ld usencond", tm_val.tv_usec - old_tmval.tv_usec);

    old_tmval = tm_val;
    
#if 1
    //test timer counter set from 1s to 10 second
    usr_msg("loop %d, set timer counter to %d", loop_counter, loop_counter);
    foo_time.expires = jiffies + loop_counter * HZ;    // reset counter to loop_counter sencod
    if(10 == loop_counter)
        loop_counter = 1;
    loop_counter++;
#else
    foo_time.expires = jiffies + 2 * HZ;    // reset counter to 2 sencod
#endif
    add_timer(&foo_time);                    //recount
}

/**
 * @Description:
 *          init timer
 *          set timer, init count
 * ------------------------------------------------------------
 *          j = jiffies;            // read the current value 
 *          stamp_1 = j + HZ;       // 1 second in the future
 *          stamp_half = j + HZ/2;  // half a second 
 *          stamp_n = j + n * HZ    / 1000; // n milliseconds 
 * 
 */
int foo_timer_init(void)
{
    int retval = 0;
    mutex_lock(&foo_mutex);
    /** linux kernel version > 4.15 init_timer can not use */
    init_timer(&foo_time);              // init kernel timer
    do_gettimeofday(&old_tmval);        // get current time
    foo_time.function = foo_timer_callback;             // set call back function
    foo_time.data = (unsigned long) "---> transfer param";   // set call back function transfer parameter
    foo_time.expires = jiffies + 1 * HZ;                // set counter timer to 1 sencod
    add_timer(&foo_time);               // add timer to kernel list
    mutex_unlock(&foo_mutex);           
   #if 0     
    setup_timer(&my_timer, foo_timer_callback, 0);
    usr_msg( "Setup timer to fire in 500ms (%ld)", jiffies); 
    retval = mod_timer(&my_timer, jiffies + msecs_to_jiffies(500));
    mutex_unlock(&foo_mutex);
    if(retval) {
        err_msg("set mod_timer failed, current jiffies = %ld\n", jiffies);
        return -EINVAL;
    }
    #endif
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

    retval = del_timer_sync(&foo_time);
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
