
#include "sysfs_demo.h"
#include <linux/time.h>			// for get system current time
// for hr-timer
#include <linux/hrtimer.h>
#include <linux/sched.h>
#include <linux/ktime.h>


#define FOO_DEV_NAME                "foo_sysfs"
#define DEVICE_CREATE_FILE			0
#define DIRECT_CREATE_SYSFS			1

#ifndef MS_TO_NS
	#define MS_TO_NS(x) 	(x * (1000000UL))      // ms to ns
#endif	/** end of #ifndef MS_TO_NS */

#define show_HZ()		usr_msg("system HZ = %u", HZ)

struct timer_info {
	struct mutex		tim_lock;
	struct hrtimer   	tim;
	struct timeval      old_timval;
	unsigned int 		tim_ticks;
	ktime_t 			tim_period;
};

// dynamically hrtimer ticks, for millisecond
static unsigned int ticks = 0;
static unsigned int loop_counter = 0;

/**
 * get strcut timer_info start address
 * @param  tim [description]
 * @return     strcut timer_info start address
 */
static inline struct timer_info *to_timer(struct hrtimer *tim)
{
	return container_of(tim, struct timer_info, tim);
}

static enum hrtimer_restart foo_hrtimer_callback(struct hrtimer * arg)
{
	ktime_t now;
	struct timer_info * tim_info = to_timer(arg);
	if(IS_ERR(tim_info)) {
		err_msg("error : get struct timer_info address faild");
	}
	
    now = arg->base->get_time();
	if(0 == ticks) {	// default status
		tim_info->tim_period = ktime_set(0, MS_TO_NS(tim_info->tim_ticks));
		usr_msg("tim_ticks has been setted to = %d", tim_info->tim_ticks);
	} else {	// setted status
		tim_info->tim_period = ktime_set(0, MS_TO_NS(ticks));
		usr_msg("tim_ticks has been setted to = %d", ticks);
	}
	
    hrtimer_forward(arg, now, tim_info->tim_period);
    loop_counter++;
    if(loop_counter >= 65535) {
    	loop_counter = 0;
    }
	usr_msg("loop_counter = %d", loop_counter);
	
    return HRTIMER_RESTART;
}

/**
 * @brief init hrtimer
 * 
 * @param struct of timer parameter 
 * @param delay ticks micro_seconds 
 */
static void foo_timer_init(struct timer_info * tim_info, unsigned int micro_seconds)
{

	if(micro_seconds < 0 || micro_seconds > 60000) {
		usr_msg("Input micro seconds out of limit, change micro sencods to max value 60000");
		micro_seconds = 60000;
	}
	usr_msg("hrs_timer init");
	mutex_lock(&tim_info->tim_lock);
    // ktime_set(const s64 secs, const unsigned long nsecs); // param1: second, param2:nanosecond
    tim_info->tim_period = ktime_set(0, MS_TO_NS(micro_seconds));     	
    hrtimer_init(&tim_info->tim, CLOCK_REALTIME, HRTIMER_MODE_REL);
    tim_info->tim.function = foo_hrtimer_callback;
	tim_info->tim_ticks    = micro_seconds;
    hrtimer_start(&tim_info->tim, tim_info->tim_period, HRTIMER_MODE_REL);
    mutex_unlock(&tim_info->tim_lock);
}

#if DEVICE_CREATE_FILE
static ssize_t foo_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	usr_msg("show : %s", __func__);
	return 0; 
}

static ssize_t foo_close(struct device *dev, struct device_attribute *attr, 
								const char *buf, size_t count)
{
	usr_msg("close : %s", __func__);
	return 0;
}

static struct device_attribute  dev_attr = {
	.attr = {
		.name = FOO_DEV_NAME,
		.mode = 0664,
	},
	.show  = foo_show,
	.store = foo_close,
};
#endif /** end of #if DEVICE_CREATE_FILE */


#if DIRECT_CREATE_SYSFS

static struct kobject * foo_kobject;

struct d_attr {
    struct attribute attr;
    int value; /* This is our data */
};

static struct d_attr data_show = {
    .attr.name = "read_loops",
    .attr.mode = 0644,
    .value = 0,
};

static struct d_attr data_write = {
    .attr.name = "change_ticks",
    .attr.mode = 0644,
    .value = 0,
};

static struct attribute * attrs[] = {
    &data_show.attr,
    &data_write.attr,
    NULL
};
static ssize_t show(struct kobject *kobj, struct attribute *attr, char *buf)
{
    struct d_attr *da = container_of(attr, struct d_attr, attr);
    usr_msg( "show called (%s), loop_counter = %d\n", da->attr.name, loop_counter);
	da->value = loop_counter;
    return scnprintf(buf, PAGE_SIZE, "%s: %d\n", da->attr.name, da->value);
}

static ssize_t store(struct kobject *kobj, struct attribute *attr, const char *buf, size_t len)
{
    struct d_attr *da = container_of(attr, struct d_attr, attr);
	
    sscanf(buf, "%d", &da->value);
	if(da->value <= 0 || da->value >= 65535)
		return sizeof(int);
	ticks = da->value;
    usr_msg("timer ticks has been setted to %d\n", da->value);
    return sizeof(int);
}

static struct sysfs_ops ops = {
    .show  = show,
    .store = store,
};

static struct kobj_type k_type = {
    .sysfs_ops     = &ops,
    .default_attrs = attrs,
};
#endif /** end of #if DIRECT_CREATE_SYSFS */

/**
 * @brief create sysfs
 * 
 * @param platform device if using device_create_file
 * @return 0 for success, !0 for failed 
 */
static int foo_create_sysfs(struct device * dev)
{
	int ret;
	
#if DIRECT_CREATE_SYSFS
	foo_kobject = kzalloc(sizeof(struct kobject), GFP_KERNEL);
	if(IS_ERR(foo_kobject)) {
		err_msg("error : foo_kobject kzalloc");
		return -ENOMEM;
	}
	// kobject_init(struct kobject * kobj, struct kobj_type * ktype)
	kobject_init(foo_kobject, &k_type);
     // parent is NULL, create folder under /sys/ direction
    ret = kobject_add(foo_kobject, NULL, "%s", FOO_DEV_NAME);
	if(0 != ret ) {
		ret = -1;
		err_msg("error: kobject_add() failed\n");
		kobject_put(foo_kobject);
		foo_kobject = NULL;
		goto out;
	}
#endif	/** end of #if DIRECT_CREATE_SYSFS */
	
#if DEVICE_CREATE_FILE
	ret = device_create_file(dev, attr);
	if(ret < 0) {
		err_msg("error : create sysfs");
        goto out;
	}
#endif /** end of #if DEVICE_CREATE_FILE */

	return ret;
out:
	return ret;
}

/**
 * @brief delete created sysfs, when driver removed
 * 
 * @param dev 
 */
static void foo_remove_sysfs(struct device * dev)
{
	usr_msg("remove sysfs");
#if DEVICE_CREATE_FILE
	device_remove_file(dev, attr);
#endif /** end of #if DEVICE_CREATE_FILE */

#if DIRECT_CREATE_SYSFS
	kobject_put(foo_kobject);
	foo_kobject = NULL;
#endif	/** end of #if DIRECT_CREATE_SYSFS */
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
	usr_msg("default ticks set to 2 seconds");
	foo_timer_init(tim_info, 2000);
	err = foo_create_sysfs(&pdev->dev);
	platform_set_drvdata(pdev, tim_info);

	return err;
}

static int foo_remove(struct platform_device *pdev)
{
	int ret;
	struct timer_info * tim_info;

	tim_info = platform_get_drvdata(pdev);
	usr_msg("remove %s platfrom driver", FOO_DEV_NAME);
	
	foo_remove_sysfs(&pdev->dev);
	do {
		ret = hrtimer_try_to_cancel(&tim_info->tim);
		if(ret < 0) {
			err_msg("timer : timer is in callback function");
		} else if(ret == 1) {
			err_msg("timer : timer is in actviting");
		} else {
			err_msg("timer : timer ready to cancel");
		}
	} while(ret);
	
	return ret;
}

static struct platform_driver foo_platform_drv = {
	.driver = {
		.name  = FOO_DEV_NAME,
		.owner = THIS_MODULE,
	},
	.probe  = foo_probe,
	.remove = foo_remove,
};

static int __init foo_sysfs_init(void)
{
	int ret;
	
	show_HZ();
	ret = platform_driver_register(&foo_platform_drv);
	if(ret < 0)
		err_msg("error : platform device register");
	return ret;
}


static void __exit foo_sysfs_exit(void)
{
	platform_driver_unregister(&foo_platform_drv);
	usr_msg("removed driver: %s", FOO_DEV_NAME);
}

module_init(foo_sysfs_init);
module_exit(foo_sysfs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("V");
MODULE_DESCRIPTION("sysfs demo.");

