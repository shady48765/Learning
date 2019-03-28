#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/pm.h>
#include <linux/slab.h>
#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/gpio.h>
#include <linux/of_platform.h>
#include <linux/of_gpio.h>
#include <linux/spinlock.h>
#include <linux/io.h>
#include <uapi/linux/input.h>
#include <linux/workqueue.h>
#include <linux/wait.h>
#include <linux/delay.h>


// define in /include/uapi/linux/input.h : #define #define KEY_USR_CUSTOMER_DEF    249 

#define KEY_VAL				KEY_BACK
#define KEY_NAME			"usr_home_button"
#define TAG					" <KEY> "
#define SET_LEVEL			KERN_INFO
#define usr_msg(fmt, args...)			printk(SET_LEVEL TAG fmt"\n", ##args)

typedef enum {
	low = 0,
	high = !low
}gpio_status;





static struct of_device_id key_dts_table[] = {
	{ .compatible = "usr-home-button", },
	{ },
};
MODULE_DEVICE_TABLE(of, key_dts_table);

/*--------------------------------------------------------------------------------------------------------------*/
#if 1

#define MS_TO_NS(x) (x * 1000000)      // ms to ns

struct usr_keys_button {
	int 						touch_home_btn;
	int 						touch_ic_rst;
	unsigned int 				irq;
	struct input_dev 			*inputdev;
	struct mutex 				lock;
};
struct usr_keys_button * btn;

struct timer_strcuture {
	ktime_t				tim_period;
	struct 	mutex		tim_mutex;
	struct	hrtimer 	timer;
	struct mutex		tim_lock;
};

static struct timer_strcuture tim;

static enum hrtimer_restart foo_hrtimer_callback(struct hrtimer * arg)
{
	static unsigned int loop_ticks;
	int key_val;
	ktime_t now;

	loop_ticks++;
    now = arg->base->get_time();

	key_val = gpio_get_value(btn->touch_home_btn);
	if(key_val) {
		usr_msg("get key_val = %d", key_val);
		input_report_key(btn->inputdev, KEY_VAL, 1);
	} else {
		usr_msg("get key_val = %d", key_val);
		input_report_key(btn->inputdev, KEY_VAL, 0);
	}
	input_sync(btn->inputdev);

	if(0 == (loop_ticks % 5)) {
		usr_msg("gpio touch_ic_rst set 1");
		gpio_set_value(btn->touch_ic_rst, 1);
	}
	if(loop_ticks > 65535)
		loop_ticks = 0;
	
    hrtimer_forward(arg, now, tim.tim_period);
	
    return HRTIMER_RESTART;
}


static void hrt_timer_init(s64 seconds, unsigned long mseconds)
{
    tim.tim_period = ktime_set(seconds, MS_TO_NS(mseconds));    
    hrtimer_init(&tim.timer, CLOCK_REALTIME, HRTIMER_MODE_REL);
    tim.timer.function = foo_hrtimer_callback;
    hrtimer_start(&tim.timer, tim.tim_period, HRTIMER_MODE_REL);
	mutex_init(&tim.tim_lock);
}

static void chip_cp2610_init(struct usr_keys_button * btn)
{
	gpio_set_value(btn->touch_ic_rst, 1);
	mdelay(200);
}

static int usr_get_dts_info(struct usr_keys_button * btn, struct device *dev)
{
	int ret;
	struct device_node *node;
	
	node = of_find_node_by_name(NULL, "usr-home-button"); 
	if (!node) {
		usr_msg("cannot find node name usr-home-button");
		return -ENODEV;
	}
	btn->touch_home_btn = of_get_named_gpio(node, "touch-home-btn", 0);
	if(!gpio_is_valid(btn->touch_home_btn)) {
		usr_msg("error: get touch_home_btn gpio info. ERROR code %d", btn->touch_home_btn);
		return -EIO;
	}
    btn->touch_ic_rst = of_get_named_gpio(node, "touch-rst-ctrl", 0);
    if(!gpio_is_valid(btn->touch_ic_rst)) {
        usr_msg("error: get touch_ic_rst gpio info.  ERROR code %d", btn->touch_ic_rst);
        return -EIO;
    }
	ret = devm_gpio_request_one(dev, btn->touch_home_btn, GPIOF_OUT_INIT_HIGH, "touch_home_btn");
	if(ret < 0) {
		usr_msg("error: gpio touch_home_btn request.");
		return -EIO;
	}
	ret = devm_gpio_request_one(dev, btn->touch_ic_rst, GPIOF_IN, "touch_rst_ctrl");
    if(ret < 0) {
		usr_msg("error: touch_ic_rst request.");
		ret = -EIO;
		goto out1;
	}
	usr_msg("requested home_btn number: %d, home_ctrl_rst number: %d", btn->touch_home_btn, btn->touch_ic_rst);

	return 0;

out1:
	devm_gpio_free(dev, btn->touch_home_btn);
	return ret;
}

static int key_probe(struct platform_device *pdev)
{
	int ret;
#if 0
	struct usr_keys_button * button = devm_kmalloc(&pdev->dev, sizeof(struct usr_keys_button), GFP_KERNEL);
	if(IS_ERR(button)) {
		usr_msg("error: devm_kmalloc");
		return -ENOMEM;
	}
#endif

	btn = devm_kmalloc(&pdev->dev, sizeof(struct usr_keys_button), GFP_KERNEL);
	if(IS_ERR(btn)) {
		usr_msg("error: devm_kmalloc");
		return -ENOMEM;
	}

	mutex_init(&btn->lock);

	platform_set_drvdata(pdev, btn);
	dev_set_drvdata(&pdev->dev, btn);
	
	btn->inputdev = input_allocate_device();
	if(IS_ERR(btn->inputdev)){
		usr_msg("error: input_allocate_device failed!");
		return -ENODEV;
	}

	btn->inputdev->name 		= KEY_NAME;				
	btn->inputdev->phys 		= "input3";				
	btn->inputdev->id.bustype 	= BUS_HOST;		
	btn->inputdev->id.vendor 	= 0x0001;
	btn->inputdev->id.product 	= 0x0001;
	btn->inputdev->id.version 	= 0x0100;

	set_bit(EV_KEY, btn->inputdev->evbit);		
	set_bit(KEY_VAL, btn->inputdev->keybit); 	

	ret = usr_get_dts_info(btn, &pdev->dev);
	if(ret < 0) {
		usr_msg("error: get gpio dts info");
		ret = -ENODEV;
		goto error1;
	}
	chip_cp2610_init(btn);
	ret = input_register_device(btn->inputdev);
	if(ret){
		usr_msg("error: input register device failed!");
		goto error1;
	}
	hrt_timer_init(0, 100);	// set ticks to 100ms

	// dev_set_drvdata(&pdev->dev, (void *)btn);
	return 0;  
	
error1:
   input_free_device(btn->inputdev);
   kfree(btn);
   return ret;

}

static int key_remove(struct platform_device *pdev)
{
	// struct usr_keys_button *btn = platform_get_drvdata(pdev);
	input_unregister_device(btn->inputdev);
	
	while(hrtimer_try_to_cancel(&tim.timer));
	// while(hrtimer_cancel(&tim.timer));
	kfree(btn);
	return 0;
}

static int key_suspend (struct device *dev)
{
	// struct usr_keys_button *button = dev_get_drvdata(dev);
	gpio_set_value(btn->touch_ic_rst, 0);
	mdelay(500);
	return 0;
}

static int key_resume (struct device *dev)
{
	// struct usr_keys_button *button = dev_get_drvdata(dev);
	gpio_set_value(btn->touch_ic_rst, 1);
	mdelay(500);
	return 0;

}


/*--------------------------------------------------------------------------------------------------*/
#else
struct workqueue_struct 	*data_handler_queue;
struct usr_keys_button {
	int 						touch_home_btn;
	int 						touch_ic_rst;
	unsigned int 				irq;
	struct input_dev 			*inputdev;
	struct mutex 				lock;
	struct work_struct 			work;
	// struct work_struct 			key_resume_work;
};

static int work_trigger_flag;

static irqreturn_t usr_key_handler(int irq, void *arg);


static struct of_device_id key_dts_table[] = {
	{ .compatible = "usr-home-button", },
	{ },
};
MODULE_DEVICE_TABLE(of, key_dts_table);


static irqreturn_t usr_key_handler(int irq, void *arg)
{
	struct usr_keys_button * btn = (struct usr_keys_button *)arg;
	if(IS_ERR(btn)) {
		usr_msg("error: struct btn get err int irq");
		return IRQ_HANDLED;
	}
	usr_msg("triggred");
    work_trigger_flag += 1;
	schedule_work(&btn->work);
    return IRQ_HANDLED; 
}

static int usr_get_dts_info(struct usr_keys_button * button, struct device *dev)
{
	int ret;
	struct device_node *node;
	
	node = of_find_node_by_name(NULL, "usr-home-button"); 
	if (!node) {
		usr_msg("cannot find node name usr-home-button");
		return -ENODEV;
	}
	button->touch_home_btn = of_get_named_gpio(node, "touch-home-btn", 0);
	if(button->touch_home_btn < 0) {
		usr_msg("error: get touch-home-btn gpio info");
		return -EIO;
	}
	button->touch_ic_rst = of_get_named_gpio(node, "touch-rst-ctrl", 0);
	if(button->touch_home_btn < 0) {
		usr_msg("error: get touch-rst-ctrl gpio info");
		return -EIO;
	}

	ret = devm_gpio_request_one(dev, button->touch_home_btn, (GPIOF_IN | GPIOF_INIT_HIGH), "touch_home_btn");
	if(ret < 0) {
		usr_msg("error: gpio touch_home_btn request.");
		return -EIO;
	}
	ret = devm_gpio_request_one(dev, button->touch_ic_rst, GPIOF_OUT_INIT_HIGH, "touch_rst_ctrl");
    if(ret < 0) {
		usr_msg("error: touch_rst_ctrl request.");
		ret = -EIO;
		goto out1;
	}
	gpio_set_value(button->touch_ic_rst, 1);
	mdelay(200);
	usr_msg("requested home_btn number: %d, home_ctrl_rst number: %d", button->touch_home_btn, button->touch_ic_rst);

	button->irq = gpio_to_irq(button->touch_home_btn);
	
	ret = request_irq(button->irq, usr_key_handler, IRQF_TRIGGER_FALLING, 
									"usr_home_btn_irq", (void *)button);
	if(ret < 0) {
		usr_msg("error: request irq");
		goto out2;
	}
	return ret;

out2:
	devm_gpio_free(dev, button->touch_ic_rst);
out1:
	devm_gpio_free(dev, button->touch_home_btn);
	return ret;	
}

void chip_cp2610_init(struct usr_keys_button * btn)
{
	gpio_set_value(btn->touch_ic_rst, 0);
	mdelay(100);
	gpio_set_value(btn->touch_ic_rst, 1);
	mdelay(200);
}

static void data_handler(struct work_struct * work)
{
	int key_val;
	struct usr_keys_button * btn = container_of(work, struct usr_keys_button, work);
	if(IS_ERR(btn)) {
		usr_msg("error : get struct usr_keys_button in funciton: %s", __func__);
		
		work_trigger_flag = 0;
	
		return ;
	}
	if(0 == work_trigger_flag) {
		usr_msg("handler button half schedule");
		schedule();
	} else {
		gpio_set_value(btn->touch_ic_rst, 1);
		mdelay(50);
		// mutex_lock(&btn->lock);
		usr_msg("handler dealing button half");
	    key_val = gpio_get_value(btn->touch_home_btn);

	    usr_msg("key_val = %d", key_val);
	    if(0 == key_val) {
	        usr_msg("pressed");
	        input_report_key(btn->inputdev, KEY_VAL, 0);
	        input_sync(btn->inputdev);

	    } else if(1 == key_val) {
	        usr_msg("release");
	        input_report_key(btn->inputdev, KEY_VAL, 1);
	        input_sync(btn->inputdev);
	    }
	}
	work_trigger_flag = 0;
}

static int key_probe(struct platform_device *pdev)
{
	int ret;

	struct usr_keys_button * button = devm_kmalloc(&pdev->dev, sizeof(struct usr_keys_button), GFP_KERNEL);
	if(IS_ERR(button)) {
		usr_msg("error: devm_kmalloc");
		return -ENOMEM;
	}

	mutex_init(&button->lock);
	
    button->inputdev = input_allocate_device();
    if(IS_ERR(button->inputdev)){
        usr_msg("error: input_allocate_device failed!");
        return -ENODEV;
    }

    button->inputdev->name = KEY_NAME;  			
    button->inputdev->phys = "input3"; 				
    button->inputdev->id.bustype = BUS_HOST;  		
    button->inputdev->id.vendor = 0x0001;
    button->inputdev->id.product = 0x0001;
    button->inputdev->id.version = 0x0100;

    set_bit(EV_KEY, button->inputdev->evbit);		
    set_bit(KEY_VAL, button->inputdev->keybit);		

	ret = usr_get_dts_info(button, &pdev->dev);
	if(ret < 0) {
		usr_msg("error: get dts info");
		ret = -ENODEV;
		goto error1;
	}
	chip_cp2610_init(button);
    ret = input_register_device(button->inputdev);
    if(ret){
        usr_msg("error: input register device failed!");
        goto error1;
    }
    work_trigger_flag = 0;

	data_handler_queue = create_singlethread_workqueue("data_handler_work");
	INIT_WORK(&button->work, data_handler);
	
#if 0
	key_resume_queue = create_singlethread_workqueue("key_resume_work");
	INIT_WORK(&button->key_resume_work, key_resume_callback);
	

    // notifier
    customer_key_notifier.notifier_call = key_notifier_callback;
    
    shaddow_btn = button;
    platform_set_drvdata(pdev, button);
    
    // queue_work(data_handler_queue, &button->work);
    // queue_work(key_resume_queue, &key_resume_work);
#endif
	return 0;  
	
error1:
   input_free_device(button->inputdev);
   return ret;
}

static int key_remove(struct platform_device *pdev)
{
	struct usr_keys_button *btn = platform_get_drvdata(pdev);
	input_unregister_device(btn->inputdev);
	flush_workqueue(data_handler_queue);
	destroy_workqueue(data_handler_queue);
	kfree(btn);
	return 0;
}

// static const struct dev_pm_ops customer_keys_pm_ops = {
//     .suspend = tca8418_keypad_suspend,
//     .resume  = tca8418_keypad_resume,
// };
#endif


static struct dev_pm_ops customer_keys_pm_ops = {
	.suspend = key_suspend,
	.resume = key_resume,
};

struct platform_device_id key_id[] = {
	{.name = "key", .driver_data = 0, },
};

static struct platform_driver keys_pdrv = {
	.probe		= key_probe,
	.remove		= key_remove,
	.driver		= {
		.name	= KEY_NAME,
		.owner	= THIS_MODULE,
		// #ifdef CONFIG_PM
		.pm	= &customer_keys_pm_ops,
		// #endif
		.of_match_table = of_match_ptr(key_dts_table),
	},
	.id_table = key_id,
};

module_platform_driver(keys_pdrv);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("V");
MODULE_DESCRIPTION("customer gpio key define");
MODULE_ALIAS("customer-keys");
