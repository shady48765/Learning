
#include "delayqueue.h"

static struct delayed_work foo_work;

/**
 * @brief 
 *      delay workqueue recall function
 * 
 * @param work 
 * @Note 
 *      if not reschedule, delay workqueue only do once
 */
static void work_handler(struct work_struct *work)
{
    static unsigned int handler_tick = 0;
    handler_tick++;
	usr_msg("------> work handler, repeat in 2 seconds, tick counter =  %d", handler_tick);
    // recall delay workqueue, if only do once, not need to recall
    schedule_delayed_work(&foo_work, msecs_to_jiffies(2000));
}


static int __init foo_delayqueue_init(void)
{
   
    usr_msg("ready to init device.");
	show_HZ();

	INIT_DELAYED_WORK(&foo_work, work_handler);
	schedule_delayed_work(&foo_work, msecs_to_jiffies(1000));

	return 0;
}

static void __exit foo_delayqueue_exit(void)
{
	usr_msg("removed driver: %s", FOO_DEV_NAME);
    usr_msg("destory delayed work sync");
    cancel_delayed_work_sync(&foo_work);
}

module_init(foo_delayqueue_init);
module_exit(foo_delayqueue_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("V");
MODULE_DESCRIPTION("delay work queue demo.");
/**--------------------------- END LINE ----------------------------------*/
