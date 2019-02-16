/***** pwm dts defined in file rk3288.dtsi ***********************
	pwm2: pwm@ff680020 {
		compatible = "rockchip,rk3288-pwm";
		reg = <0x0 0xff680020 0x0 0x10>;
		#pwm-cells = <3>;
		pinctrl-names = "active";
		pinctrl-0 = <&pwm2_pin>;
		clocks = <&cru PCLK_PWM>;
		clock-names = "pwm";
		status = "disabled";
	};

	pwm3: pwm@ff680030 {
		compatible = "rockchip,rk3288-pwm";
		reg = <0x0 0xff680030 0x0 0x10>;
		#pwm-cells = <2>;
		pinctrl-names = "active";
		pinctrl-0 = <&pwm3_pin>;
		clocks = <&cru PCLK_PWM>;
		clock-names = "pwm";
		status = "disabled";
	};
*******************************************************************************/
#include "simple_pwm.h"
#include <linux/time.h>	// for get system current time
#include <linux/proc_fs.h>
#include <linux/interrupt.h>



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
#if USED_HRS_TIMER
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

static int usr_pwm_drv_create_file(void)
{
	driver_create_file(struct device_driver * drv, const struct driver_attribute * attr);
}

static of_device_id usr_pwm_id[] = {
	{.compatible = "pwm_module"},
};
MODULE_DEVICE_TABLE(pwm_drv, usr_pwm_id)

static const struct platform_device_id usr_pwm_dev_id[] = {
    {"usr_pwm", 0},
    {/*keep this*/}
};
MODULE_DEVICE_TABLE(usr_pwm, oled_device_id);

struct platform_driver usr_pwm_drv = {
	.driver {
		.owner = THIS_MODULE,
		.name = USR_PWM_DRV_NAME,
		.of_match_table = of_match_ptr(usr_pwm_id), 
	},
	.id_table = usr_pwm_dev_id,
	.probe = usr_pwm_probe,
	.remove = usr_pwm_remove,
	.suspend = usr_pwm_suspend,
	.resume = usr_pwm_resum,
};





static int __init usr_pwm_init(void)
{
    int ret;
	usr_msg("pwm driver init start");
	struct pwm_dev *  usr_pwm_info;
	usr_pwm_info = kmalloc(sizeof(struct pwm_dev), GFP_KERNEL);
	if(!usr_pwm_info) {
		err_msg(" err : kmalloc");
		return -ENOMEM;
	}
	memset(usr_pwm_info, 0, sizeof(struct pwm_dev));
    mutex_init(&usr_pwm_info->lock);

	




	
    
    
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

static void __exit usr_pwm_exit(void)
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

module_init(usr_pwm_init);
module_exit(usr_pwm_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("quan");
MODULE_DESCRIPTION("kernel jiffy timer example.");
MODULE_ALIAS("usr_HZ");
