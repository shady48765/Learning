
#include "sysfs_demo.h"
#include <linux/time.h>			// for get system current time
// for hr-timer
#include <linux/hrtimer.h>
#include <linux/sched.h>
#include <linux/ktime.h>


#define FOO_DEV_NAME                "foo_sysfs"

#ifndef MS_TO_NS
	#define MS_TO_NS(x) 	(x * (1000000UL))      // ms to ns
#endif	/** end of #ifndef MS_TO_NS */

#define show_jiffies()		usr_msg("system jiffies = %lu", jiffies)

struct timer_info {
	struct mutex		tim_lock;
	struct hrtimer   	tim;
	struct timeval      old_timval;
	unsigned int 		tim_ticks;
	ktime_t 			tim_period;
};

static unsigned int ticks;

#if 0
static struct foo_demo_info {
	struct timer_info 	*tim_info;
};
#endif

static inline struct timer_info *to_timer(struct hrtimer *tim)
{
	return container_of(tim, struct timer_info, tim);
}

static enum hrtimer_restart foo_hrtimer_callback(struct hrtimer * arg)
{
	static unsigned loop_counter;
	ktime_t now;
	usr_msg("take struct frome container");
	struct timer_info * tim_info = to_timer(arg);
	if(IS_ERR(tim_info)) {
		err_msg("error : get struct timer_info address faild");
	}
	usr_msg("take struct frome container success");
    now = arg->base->get_time();
    hrtimer_forward(arg, now, tim_info->tim_period);
    loop_counter++;
    if(loop_counter >= 65535) {
    	loop_counter = 0;
    }
	usr_msg("loop_counter = %d", loop_counter);
	
    return HRTIMER_RESTART;
}

static void foo_timer_init(struct timer_info * tim_info, unsigned int micro_seconds)
{

	if(micro_seconds < 0 || micro_seconds > 60000) {
		usr_msg("Input micro seconds out of limit, change micro sencods to max value 60000");
		micro_seconds = 60000;
	}
	usr_msg("hrs_timer init");
	mutex_lock(&tim_info->tim_lock);
    // ktime_set(const s64 secs, const unsigned long nsecs); // param1: second, param2:nanosecond
    err_msg("-------------- 1 ----------------");
    tim_info->tim_period = ktime_set(0, MS_TO_NS(micro_seconds));     	
    err_msg("-------------- 2 ----------------");
    hrtimer_init(&tim_info->tim, CLOCK_REALTIME, HRTIMER_MODE_REL);
    err_msg("-------------- 3 ----------------");
    tim_info->tim.function = foo_hrtimer_callback;
    err_msg("-------------- 4 ----------------");
    hrtimer_start(&tim_info->tim, tim_info->tim_period, HRTIMER_MODE_REL);
    err_msg("-------------- 5 ----------------");
    mutex_unlock(&tim_info->tim_lock);
    err_msg("-------------- 6 ----------------");

}

static ssize_t foo_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	
	usr_msg("show : %s", __func__);
//int mod_timer(struct timer_list *timer, unsigned long expires);----修改到期定时器
	return 0; 
}

static ssize_t foo_close(struct device *dev, struct device_attribute *attr, 
								const char *buf, size_t count)
{
	
	usr_msg("close : %s", __func__);

	return 0;
}

struct device_attribute  dev_attr = {
	.attr = {
		.name = FOO_DEV_NAME,
		.mode=  0664,
	},
	.show = foo_show,
	.store = foo_close,
};

static int foo_create_sysfs(struct device * dev, struct device_attribute * attr)
{
	int ret;
	ret = device_create_file(dev, attr);
	if(ret < 0) {
		err_msg("error : create sysfs");
	}
	return ret;
}

static void foo_remove_sysfs(struct device * dev, struct device_attribute * attr)
{
	usr_msg("remove sysfs");
	device_remove_file(dev, attr);
}


static int foo_probe(struct platform_device * pdev)
{
	int err;
	struct timer_info * tim_info;

	usr_msg("driver and device matched, ready to run foo_probe function");
	tim_info = devm_kmalloc(&pdev->dev, sizeof(struct timer_info), GFP_KERNEL);
	if(IS_ERR(tim_info)) {
		err_msg("error : devm_kmalloc");
		return -ENOMEM;
	}
	mutex_init(&tim_info->tim_lock);
	foo_timer_init(tim_info, 1000);

	foo_create_sysfs(&pdev->dev, &dev_attr);

	platform_set_drvdata(pdev, tim_info);

	return err;
	
}

static int foo_remove(struct platform_device *pdev)
{
	int ret;
	struct timer_info * tim_info;

	tim_info = platform_get_drvdata(pdev);
	usr_msg("remove %s platfrom driver", FOO_DEV_NAME);
	
	foo_remove_sysfs(&pdev->dev, &dev_attr);
	do {
		ret = hrtimer_try_to_cancel(&tim_info->tim);
		if(ret < 0) {
			err_msg("timer : timer is in callback function");
		} else if(ret == 1) {
			err_msg("timer : timer is in actviting");
		} else {
			err_msg("timer : timer ready to cancel");
		}
	}while(ret);

	
	return ret;
}

static struct platform_driver foo_platform_drv = {
	.driver = {
		.name = FOO_DEV_NAME,
		.owner = THIS_MODULE,
	},
	.probe = foo_probe,
	.remove = foo_remove,
};

static struct platform_device foo_platform_dev = {
	.name = FOO_DEV_NAME,
	.id = -1,
};

static int __init foo_sysfs_init(void)
{
	int ret;
	
	// show_jiffies();
	usr_msg("start regiter platform device and driver.");
	ret = platform_device_register(&foo_platform_dev);
	if(ret < 0)
		goto out;
    
	ret = platform_driver_register(&foo_platform_drv);
	if(ret < 0)
		goto out;
	
	return ret;

out:
	return ret;
}


static void __exit foo_sysfs_exit(void)
{
	usr_msg("ready to remove driver: %s", FOO_DEV_NAME);
	platform_driver_unregister(&foo_platform_drv);
	// platform_device_unregister(&foo_platform_dev);
}

module_init(foo_sysfs_init);
module_exit(foo_sysfs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("V");
MODULE_DESCRIPTION("sysfs demo.");

