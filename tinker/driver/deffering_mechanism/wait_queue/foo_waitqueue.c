#include "foo_waitqueue.h"
#include <linux/time.h>	// for get system current time
#include <linux/workqueue.h>
#include <linux/wait.h>

#define FOO_WAIT_QUEUE          1
#define FOO_HRT_TIMER           0



/*-------------- wait queue create start ------------------*/
#if FOO_WAIT_QUEUE

static DECLARE_WAIT_QUEUE_HEAD(foo_queue_head);
// static struct wait_queue_head_t     foo_queue_head;     // not recommended definition
static struct work_struct           foo_queue_work;
static int condition = 1;

static void foo_queue_recall_function(struct work_struct *work)
{
    static int foo_waitqueue_repeat_schedule = 1;
    
    usr_msg("get in waitqueue recall fuction, current condition = %d", condition);
    condition += 1;
    wake_up_interruptible(&foo_queue_head);
    usr_msg("queue has been woke-up, current condition = %d", condition);

    // reschedule 10 times
    if(1 == foo_waitqueue_repeat_schedule && counter < 10) {
        msleep(1000);
        schedule_work(&foo_queue_work);
    }
   else {
       // normally program will not reached here
       // after 10 times schedule_work, program will be suspend
       usr_msg("skip wake_up_interruptible, condition = %d", condition);
   }
}

void foo_waitqueue_init(void)
{
    init_waitqueue_head(&foo_queue_head);
    INIT_WORK(&foo_queue_work, foo_queue_recall_function);
    schedule_work(&foo_queue_work);
    wait_event_interruptible(foo_queue_head, condition != 0);
    usr_msg("waitqueue_created, init codition =  %d", condition);
}

#endif  //end #if FOO_WAIT_QUEUE
/*-------------- wait queue create end ------------------*/
#if FOO_HRT_TIMER
void foo_timer_callback(unsigned long arg)
{
    struct timeval tm_val;
    static int loop_counter = 0;
    
    // get current time and compare with before
    usr_msg("schedule time arrived, jiffies = %lu", jiffies);
    char * strs = (char *) arg;
    usr_msg("got transferred args is : %s", strs);
    // get current time
    do_gettimeofday(&tm_val);
    // calculator interval_time
    usr_msg("interval time is %ld sencond", tm_val.tv_sec - old_tmval.tv_sec);
    usr_msg("interval time is %ld usencond", tm_val.tv_usec - old_tmval.tv_usec);
    // get current time and compare with before end
    old_tmval = tm_val;         // reload old time value
    
    //test timer counter set from 1s to 10 second
    usr_msg("loop %d, set timer counter to %d", loop_counter, loop_counter);
    foo_time.expires = jiffies + loop_counter * HZ;             // reset counter to loop_counter sencod
    if(30 == loop_counter)
        loop_counter = 1;
    loop_counter++;
    add_timer(&foo_time);                    //recount
}


int foo_timer_init(void)
{
    int retval = 0;
    mutex_lock(&foo_mutex);
    init_timer(&foo_time);                                      // init kernel timer
    do_gettimeofday(&old_tmval);                                // get current time
    foo_time.function = foo_timer_callback;                     // set call back function
    foo_time.data = (unsigned long) "---> transfer param";      // set call back function transfer parameter
    foo_time.expires = jiffies + 1 * HZ;                        // set counter timer to 1 sencod
    add_timer(&foo_time);                                       // add timer to kernel list
    mutex_unlock(&foo_mutex);           

    return retval;
}
#endif  //end #if FOO_HRT_TIMER
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
    
    
    
    
    
    // timer, if only init, can only run once foo_timer_callback
    // every timer run foo_timer_callback, timer need to bu update!
#if FOO_HRT_TIMER
    ret = foo_timer_init();
    if(0 != ret) {
        err_msg("timer init failed");
    }
    usr_msg("timer init succeed, current jiffies = %ld", jiffies);
#endif
#if FOO_WAIT_QUEUE
    // waitqueue create
    foo_waitqueue_init();
#endif
    
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
    usr_msg("module exit");
    device_destroy(foo_dev_info->foo_class, foo_dev_info->foo_dev_number);
    class_destroy(foo_dev_info->foo_class);
    cdev_del(&foo_dev_info->foo_cdev);
    unregister_chrdev_region(foo_dev_info->foo_dev_number, 1);
    kfree(foo_dev_info);
    
#if FOO_HRT_TIMER
    retval = del_timer(&foo_time);
    if (retval)
        err_msg("The timer is still in use...");
    usr_msg("Timer module unloaded");
    usr_msg("timer_jiffy exit success");
#endif
}

module_init(timer_jiffy_init);
module_exit(timer_jiffy_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("quan");
MODULE_DESCRIPTION("kernel jiffy timer example.");
MODULE_ALIAS("usr_HZ");
