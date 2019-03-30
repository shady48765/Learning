
#include "delayqueue.h"


// dynamically hrtimer ticks, for millisecond
static unsigned int ticks;
static unsigned int loop_counter;
static struct delayed_work foo_work;
struct timer_info * timer_info;

#if HRS_TIMER
static enum hrtimer_restart foo_hrtimer_callback(struct hrtimer * arg)
{
	ktime_t now;
	struct timer_info * tim_info = container_of(arg, struct timer_info, tim);
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
	schedule_delayed_work(&foo_work, msecs_to_jiffies(1000));
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
	tim_info->tim_ticks = micro_seconds;
    hrtimer_start(&tim_info->tim, tim_info->tim_period, HRTIMER_MODE_REL);
    mutex_unlock(&tim_info->tim_lock);
}

#endif /** #if HRS_TIMER */



static void work_handler(struct work_struct *work)
{
    static unsigned int work_handler_tick = 0;
    work_handler_tick++;
	usr_msg("------> work handler, work_handler_tick = %d", work_handler_tick);
}


static int __init foo_delayqueue_init(void)
{
    
    usr_msg("ready to init device.");
	show_HZ();
    
	timer_info = kmalloc(sizeof(struct timer_info), GFP_KERNEL);
	if(IS_ERR(timer_info)) {
		err_msg("error : devm_kmalloc foo_device_info");
		return -ENOMEM;
	}
    
	mutex_init(&timer_info->tim_lock);
#if HRS_TIMER
	usr_msg("default ticks set to 2 seconds");
	foo_timer_init(timer_info, 2000);
#endif
	INIT_DELAYED_WORK(&foo_work, work_handler);

	return 0;
}

static void __exit foo_delayqueue_exit(void)
{

	int ret = 0;
	
	usr_msg("removed driver: %s", FOO_DEV_NAME);
#if HRS_TIMER
	do {
		ret = hrtimer_try_to_cancel(&timer_info->tim);
		if(ret < 0) {
			err_msg("timer : timer is in callback function");
		} else if(ret == 1) {
			err_msg("timer : timer is in actviting");
		} else {
			err_msg("timer : timer ready to cancel");
		}
	} while(ret);
#endif /** #if HRS_TIMER */
    usr_msg("destory delayed work sync");
    cancel_delayed_work_sync(&foo_work);
	kfree(timer_info);
	timer_info = NULL;

}

module_init(foo_delayqueue_init);
module_exit(foo_delayqueue_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("V");
MODULE_DESCRIPTION("delay work queue demo.");

