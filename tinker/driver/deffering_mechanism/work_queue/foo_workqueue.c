#include "foo_workqueue.h"
#include <linux/time.h>	// for get system current time
#if FOO_PROC_FILE
#include <linux/proc_fs.h>
#endif// end #FOO_PROC_FILE
#if FOO_TASKLET
    #include <linux/interrupt.h>
#endif // end #FOO_TASKLET
#if FOO_WORKQUEUE
    #include <linux/workqueue.h>
#endif


#define FOO_WORKQUEUE                   1
#define FOO_TASKLET                     0
#define FOO_STANDARD_TIMER              0
#define FOO_PROC_FILE                   0
/*-------------- workqueue start -------------------------------------------*/

#if FOO_WORKQUEUE
// #define DECLARE_WORK(n, f)						        \
//          struct work_struct n = __WORK_INITIALIZER(n, f)

static struct work_struct           foo_work;
static struct workqueue_struct      foo_workqueue;

static void foo_workqueue_recall(struct work_struct *work)
{
    static int counter = 0;
    static int repeate_workqueue_recall = 1;
    
    usr_msg("counter = %d", counter);
    counter++;

    if(1 == repeate_workqueue_recall && counter < 10) {
        msleep(1000);
        schedule_work(&foo_work);
    } else {
        
        schedule();
        // will not approach here
        usr_msg("workqueue scheduled");
    }
}

int foo_workqueue_init(void)
{
    INIT_WORK(&foo_work, foo_workqueue_recall);
    schedule_work(&foo_work);   // or queue_work(system_wq, work);
    usr_msg("workqueue init success");
}

void foo_workqueue_remove(void)
{
    cancel_work_sync(&foo_work);
    destroy_work_on_stack(&foo_work);
    usr_msg("workqueue removed");
}

#endif // end #if FOO_TASKLET

/*-------------- workqueue end -------------------------------------------*/
/*-------------- tasklet start -------------------------------------------*/
// reschedule tasklet in tasklet recall function, system will be slowed down
#if FOO_TASKLET
static unsigned char foo_tasklet_data[] = "tasklet data, unsigned char format";
void foo_tasklet_recall(unsigned long data);
// #define DECLARE_TASKLET(name, func, data) \
//          struct tasklet_struct name = { NULL, 0, ATOMIC_INIT(0), func, data }
// name ---> struct tasklet_struct
// data ---> unsigned long
DECLARE_TASKLET(foo_tasklet, foo_tasklet_recall, (unsigned long)foo_tasklet_data);

void foo_tasklet_recall(unsigned long data)
{
    static int counter = 0;
    static int foo_tasklet_repeat_schedule = 1;
    
    usr_msg("transfered data is : %s", (unsigned char *)data);
    counter++;
    usr_msg("foo_tasklet_recall loop counter = %d", counter);
    
    // reschedule 10 times
    if(1 == foo_tasklet_repeat_schedule && counter < 10) {
        msleep(1000);
        tasklet_schedule(&foo_tasklet);
    }
}

void foo_tasklet_init(void)
{
    // start schedule tasklet
    tasklet_schedule(&foo_tasklet);
    usr_msg("tasklet_scheduled");
}
void foo_tasklet_exit(void)
{
    tasklet_disable_nosync(&foo_tasklet);       // system will be stucked if delete this
    tasklet_kill(&foo_tasklet);
    usr_msg("tasklet_killed");
}

#endif // end #if FOO_TASKLET
/*-------------- tasklet end -------------------------------------------*/
/*-------------- proc file create start --------------------------------*/
#if FOO_PROC_FILE
#define FOO_PROC_NAME   "foo_proc"

struct proc_dir_entry   *foo_proc_dir;

int foo_proc_create(void)
{
    foo_proc_dir = proc_create(FOO_PROC_NAME, 0666, NULL, &foo_fops);
    if(IS_ERR(foo_proc_dir)){
        return -ENOMEM;
    }
    return 0;
}
#endif  // end #if FOO_PROC_FILE


/*-------------- proc file create end ------------------*/
/*-------------- standard timer start ------------------------------------*/
#if FOO_STANDARD_TIMER
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
#endif // end #if FOO_STANDARD_TIMER
/*-------------- standard timer end ------------------------------------*/
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

int foo_chrdev_register(void)
{
    int ret;
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
    
    return ret;
err_cdev_add:
    unregister_chrdev_region(foo_dev_info->foo_dev_number, 1);
err_alloc_chrdev:
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
    usr_msg("foo_device create success");
    return 0;
    
err_device_create:
    class_destroy(foo_dev_info->foo_class);
err_class_create:
    return ret;
}

static int __init foo_device_init(void)
{
    int ret;
    mutex_init(&foo_mutex);
    usr_msg("foo_device create start");
    
    foo_dev_info = kmalloc(sizeof(struct my_cdev), GFP_KERNEL);
    if(!foo_dev_info) {
        err_msg("kzmalloc error");
        ret = -ENOMEM;
        return ret;
    }
    memset(foo_dev_info, 0, sizeof(struct my_cdev));
    
    ret = foo_chrdev_register();
    if(ret < 0)
        goto err_chrdev_register;
    ret = foo_device_create();
    if(ret < 0)
        goto err_foo_device_create;
    usr_msg("foo_device create success");
    
    
#if FOO_STANDARD_TIMER
    // timer, if only init, can only run once foo_timer_callback
    // every timer run foo_timer_callback, timer need to bu update!
    ret = foo_timer_init();
    if(0 != ret) {
        err_msg("timer init failed");
    }
    usr_msg("timer init succeed, HZ = %d, current jiffies = %ld", HZ, jiffies);
#endif // end FOO_STANDARD_TIMER

#if FOO_PROC_FILE
    // proc file create
    foo_proc_create();
    usr_msg("proc file created");
#endif // end #if FOO_PROC_FILE

#if FOO_TASKLET
    foo_tasklet_init();
#endif  // end #if FOO_TASKLET

#if FOO_WORKQUEUE
    foo_workqueue_init();
#endif
    return ret;

err_foo_device_create:
    cdev_del(&foo_dev_info->foo_cdev);
    unregister_chrdev_region(foo_dev_info->foo_dev_number, 1);
err_chrdev_register:
    kfree(foo_dev_info);
    return ret;
}

static void __exit foo_device_exit(void)
{
    int retval;
    usr_msg("module uninstall protocol");
    device_destroy(foo_dev_info->foo_class, foo_dev_info->foo_dev_number);
    class_destroy(foo_dev_info->foo_class);
    cdev_del(&foo_dev_info->foo_cdev);
    unregister_chrdev_region(foo_dev_info->foo_dev_number, 1);
    kfree(foo_dev_info);
#if FOO_STANDARD_TIMER
    retval = del_timer(&foo_time);
    if (retval)
        err_msg("The timer is still in use...\n");
    usr_msg("Timer module unloaded\n");
    usr_msg("standard timer exit success");
#endif  // end #if FOO_STANDARD_TIMER
#if FOO_TASKLET
    foo_tasklet_exit();
#endif  // end #if FOO_TASKLET

#if FOO_WORKQUEUE
    foo_workqueue_remove();
    usr_msg("foo_workqueue_removed success");
#endif
    usr_msg("foo_device module uninstall success");
}

module_init(foo_device_init);
module_exit(foo_device_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("quan");
MODULE_DESCRIPTION("kernel jiffy timer example.");
MODULE_ALIAS("usr_HZ");
