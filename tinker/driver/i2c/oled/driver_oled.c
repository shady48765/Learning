#include "driver_oled.h"

#define OLED_WORKQUEUE                   1
#define OLED_TASKLET                     0
#define OLED_STANDARD_TIMER              0
#define OLED_PROC_FILE                   0


#define OLED_DRIVER_VERSION         "0.1"

// #define dev_info(dev, fmt, arg...) _dev_info(dev, fmt, ##arg)
#include <linux/device.h>




struct of_device_id *oled_device_id[] = {
    { compatible = "oled, ssd1306" },
    {/*keep this*/},
};
// MODULE_DEVICE_TABLE(of, of_match_ptr(oled_device_id));

































/*-------------- workqueue start -------------------------------------------*/

#if OLED_WORKQUEUE
// #define DECLARE_WORK(n, f)			\
//          struct work_struct n = __WORK_INITIALIZER(n, f)

static struct work_struct           oled_work;
static struct workqueue_struct      oled_workqueue;

static void oled_workqueue_recall(struct work_struct *work)
{
    static int counter = 0;
    static int repeate_workqueue_recall = 1;

    usr_msg("counter = %d", counter);
    counter++;

    if(1 == repeate_workqueue_recall && counter < 10) {
        msleep(1000);
        schedule_work(&oled_work);
    } else {
        schedule();                         // give up cpu using authority
        usr_msg("workqueue scheduled");     // will approach here once
    }
}
int oled_workqueue_init(void)
{
    INIT_WORK(&oled_work, oled_workqueue_recall);
    schedule_work(&oled_work);               // or queue_work(system_wq, work);
    usr_msg("workqueue init success");
}

void oled_workqueue_remove(void)
{
    cancel_work_sync(&oled_work);
    destroy_work_on_stack(&oled_work);
    usr_msg("workqueue removed");
}

#endif // end #if FOO_TASKLET

/*-------------- workqueue end -------------------------------------------*/
/*-------------- tasklet start -------------------------------------------*/
// reschedule tasklet in tasklet recall function, system will be slowed down
#if OLED_TASKLET
static unsigned char foo_tasklet_data[] = "tasklet data, unsigned char format";
void oled_tasklet_recall(unsigned long data);
// #define DECLARE_TASKLET(name, func, data) \
//          struct tasklet_struct name = { NULL, 0, ATOMIC_INIT(0), func, data }
// name ---> struct tasklet_struct
// data ---> unsigned long
DECLARE_TASKLET(oled_tasklet, oled_tasklet_recall, (unsigned long)oled_tasklet_data);

void oled_tasklet_recall(unsigned long data)
{
    static int counter = 0;
    static int oled_tasklet_repeat_schedule = 1;

    usr_msg("transfered data is : %s", (unsigned char *)data);
    counter++;
    usr_msg("oled_tasklet_recall loop counter = %d", counter);

    // reschedule 10 times
    if(1 == oled_tasklet_repeat_schedule && counter < 10) {
        msleep(1000);
        //mdelay(1000);
        tasklet_schedule(&oled_tasklet);
    }
}

void oled_tasklet_init(void)
{
    tasklet_schedule(&oled_tasklet);     // start schedule tasklet
    usr_msg("tasklet_scheduled");
}
void oled_tasklet_exit(void)
{
    tasklet_disable_nosync(&oled_tasklet);       // system will be stucked if delete this
    tasklet_kill(&oled_tasklet);
    usr_msg("tasklet_killed");
}

#endif // end #if oled_TASKLET
/*-------------- tasklet end -------------------------------------------*/
/*-------------- proc file create start --------------------------------*/
#if OLED_PROC_FILE
#define OLED_PROC_NAME   "oled_proc"

struct proc_dir_entry   *oled_proc_dir;

int oled_proc_create(void)
{
  oled_proc_dir = proc_create(OLED_PROC_NAME, 0666, NULL, &oled_fops);
  if (IS_ERR(oled_proc_dir)) {
        return -ENOMEM;
  }
    return 0;
}
#endif  // end #if FOO_PROC_FILE


/*-------------- proc file create end ------------------*/
/*-------------- standard timer start ------------------------------------*/
#if OLED_STANDARD_TIMER
void oled_timer_callback(unsigned long arg)
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
    oled_time.expires = jiffies + loop_counter * HZ;             // reset counter to loop_counter sencod
    if(30 == loop_counter)
        loop_counter = 1;
    loop_counter++;
    add_timer(&oled_time);                    //recount
}

int oled_timer_init(void)
{
    int retval = 0;
    mutex_lock(&oled_mutex);
    init_timer(&oled_time);                                      // init kernel timer
    do_gettimeofday(&old_tmval);                                // get current time
    oled_time.function = oled_timer_callback;                     // set call back function
    oled_time.data = (unsigned long) "---> transfer param";      // set call back function transfer parameter
    oled_time.expires = jiffies + 1 * HZ;                        // set counter timer to 1 sencod
    add_timer(&oled_time);                                       // add timer to kernel list
    mutex_unlock(&oled_mutex);

    return retval;
}
#endif // end #if FOO_STANDARD_TIMER
/*-------------- standard timer end ------------------------------------*/
static int oled_open(struct inode *inode, struct file *filp)
{
    usr_msg( "open");
    return 0;
}

static int oled_close(struct inode *inode, struct file *filp)
{
    usr_msg( "close");
    return 0;
}

static ssize_t oled_write(struct file *flip, const char __user *buff,
                         size_t counter, loff_t *fops)
{
    usr_msg( "write");
    return 0;
}

static ssize_t oled_read (struct file *flip, char __user *buff,
                            size_t counter, loff_t *fops)
{
    usr_msg( "read");
    return 0;
}
static long oled_ioctl (struct file *flip, unsigned int cmd, unsigned long param)
{
    usr_msg( "ioctl");
    return 0;
}
/*------------------------------- driver create -------------------------------*/
int oled_chrdev_register(void)
{
    int ret;
    ret = alloc_chrdev_region(&oled_info->oled_device.oled_dev_t, 0, 1, OLED_DEV_NAME);
    if(ret < 0) {
        err_msg("alloc_chrdev_region error");
        ret = -ENODEV;
        goto err_alloc_chrdev;
    }
    oled_info->oled_device.oled_major_number = MAJOR(oled_info->oled_device.oled_dev_t);
    usr_msg("oled driver, device major numver = %d", oled_info->oled_device.oled_major_number);

    cdev_init(&oled_info->oled_device.oled_cdev, &oled_info.oled_fops);
    oled_info->oled_device.oled_cdev.owner = THIS_MODULE;

    ret = cdev_add(&oled_info->oled_device.oled_cdev, oled_info->oled_device.oled_dev_t, 1);
    if(ret < 0) {
        err_msg("cdev_add error");
        ret = -ENOMEM;
        goto err_cdev_add;
    }

    return ret;
err_cdev_add:
    unregister_chrdev_region(oled_info->oled_device.oled_dev_t, 1);
err_alloc_chrdev:
    return ret;
}

int oled_device_create(void)
{
    int ret;
    oled_info->oled_device->oled_class = class_create(THIS_MODULE, OLED_DEV_NAME);
    if(IS_ERR(oled_info->oled_device->oled_class)) {
        err_msg("class_create error");
        ret = PTR_ERR(oled_info->oled_device->oled_class);
        goto err_class_create;
    }
    oled_info->oled_device->oled_device = device_create(oled_info->oled_device->oled_class, NULL,
                                            oled_info->oled_device.oled_dev_t, NULL, "%s", OLED_DEV_NAME);
    if(IS_ERR(oled_info->oled_device->oled_device)) {
        err_msg("device_create error");
        ret = PTR_ERR(oled_info->oled_device->oled_device);
        goto err_device_create;
    }
    usr_msg("device_create success");
    return 0;

err_device_create:
    class_destroy(oled_info->oled_device->oled_class);
err_class_create:
    return ret;
}

static int __init oled_init(void)
{
    int ret;

    usr_msg("foo_device create start");

    oled_info = kmalloc(sizeof(struct oled_info_struct), GFP_KERNEL);
    if(!oled_info) {
        err_msg("kzmalloc error");
        ret = -ENOMEM;
        return ret;
    }

    memset(oled_info, 0, sizeof(struct oled_info_struct));

    ret = oled_chrdev_register();
    if(ret < 0) {
        err_msg("oled_chrdev_register error \n");
        goto err_chrdev_register;
    }
    ret = oled_device_create();
    if(ret < 0) {
        err_msg("oled_device_create error \n");
        goto err_foo_device_create;
    }
    usr_msg("foo_device create success");

    mutex_init(&oled_info->oled_device.oled_mutex);

    // #if FOO_STANDARD_TIMER
    //     // timer, if only init, can only run once foo_timer_callback
    //     // every timer run foo_timer_callback, timer need to bu update!
    //     ret = foo_timer_init();
    //     if(0 != ret) {
    //         err_msg("timer init failed");
    //     }
    //     usr_msg("timer init succeed, HZ = %d, current jiffies = %ld", HZ, jiffies);
    // #endif // end FOO_STANDARD_TIMER

    // #if FOO_PROC_FILE
    //     // proc file create
    //     foo_proc_create();
    //     usr_msg("proc file created");
    // #endif // end #if FOO_PROC_FILE

    // #if FOO_TASKLET
    //     foo_tasklet_init();
    // #endif  // end #if FOO_TASKLET

    // #if FOO_WORKQUEUE
    //     foo_workqueue_init();
    // #endif




    return ret;

err_foo_device_create:
    cdev_del(&foo_dev_info->foo_cdev);
    unregister_chrdev_region(foo_dev_info->foo_dev_number, 1);
err_chrdev_register:
    kfree(foo_dev_info);
    return ret;
}

static void __exit oled_exit(void)
{
    int retval;
    usr_msg("module uninstall protocol");
    device_destroy(oled_info->oled_device->oled_class, oled_info->oled_device.oled_dev_t);
    class_destroy(oled_info->oled_device->oled_class);
    cdev_del(&oled_info->oled_device.oled_cdev);
    unregister_chrdev_region(oled_info->oled_device.oled_dev_t, 1);
    kfree(oled_info);
    
#if OLED_STANDARD_TIMER
    retval = del_timer(&oled_time);
    if (retval)
        err_msg("The timer is still in use...\n");
    usr_msg("Timer module unloaded\n");
    usr_msg("standard timer exit success");
#endif  // end #if FOO_STANDARD_TIMER
#if OLED_TASKLET
    oled_tasklet_exit();
#endif  // end #if FOO_TASKLET

#if OLED_WORKQUEUE
    oled_workqueue_remove();
    usr_msg("foo_workqueue_removed success");
#endif
    usr_msg("foo_device module uninstall success");
}




















#if 0
struct bus_type oled_bus_type = {
    
};

struct notifier_block oled_notifier_block = {
    .notifier_call = 
};
#endif


struct oled_i2c_device {
    unsigned char 		* data;
    struct i2c_client   *oled_client;
    struct mutex        lock;
};


static int oled_get_dts_info(struct platform_device *pdev)
{
	int ret = 0;
	oled_dts_info = NULL;
	oled_dts_info->node = of_find_matching_node(node, oled_device_id);
	if(!oled_dts_info->node) {
		err_msg("oled_dts_info kmalloc error.");
		ret =  -ENOMEM;
		goto err_no_mem:
	} else {
		// get dts information
		of_property_read_u32(oled_dts_info->node, "width", &oled_dts_info->oled_width);
		usr_msg("---> width = %d", oled_dts_info->oled_width);
		of_property_read_u32(oled_dts_info->node, "height", &oled_dts_info->oled_height);
		usr_msg("---> height = %d", oled_dts_info->oled_height);
	}
	// get dts gpio configurations
	devm_pinctrl_get(struct device * dev);
	pinctrl_lookup_state(struct pinctrl * p, const char * name);
	oled_rst = of_get_named_gpio(struct device_node * np, const char * propname, int index);

err_no_mem:
	kfree(oled_dts_info);
}



static int oled_probe(struct i2c_client * client, const struct i2c_device_id *i2c_id)
{
    usr_msg("move in");
	oled_get_dts_info();
    if(!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		err_msg("i2c check failed");
		return -ENODEV;
    }
    
    dev_info(&client->dev, "chip found, driver verison" OLED_DRIVER_VERSION "\n");

    oled_client = client;

    oled_init();
    oled_clear();
    oled_show_string(6, 1, "Bootting...", 16);

    return 0;


#if 0
    /* Keep track of adapters which will be added or removed later */
	res = bus_register_notifier(&i2c_bus_type, &i2cdev_notifier);
	if (res)
		goto out_unreg_class;
#endif
}

struct platform_driver oled_driver = {
    .driver = {
        .owner = THIS_MODULE,
        .name = OLED_DEV_NAME,
        .of_match_table = of_match_ptr(oled_device_id),
        
    },
    .probe = oled_probe,
    .remove = oled_remove,
};

module_i2c_driver(oled_driver);



MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("quan, <shusheng1991@gmail.com>");
MODULE_DESCRIPTION("OLED Driver, Contrl with SSD1306");
MODULE_VERSION(OLED_DRIVER_VERSION);

