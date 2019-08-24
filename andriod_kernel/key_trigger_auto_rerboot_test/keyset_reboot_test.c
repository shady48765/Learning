/**********************************************************************
* dts 

reboot_test: reboot-test {
	compatible = "rockchip, reboot-test";
	status = "okay";
	reboot-key = <&gpio5, RK_PC3, GPIO_ACTIVE_LOW>;
	detect_interval = <100>;		//ms
}

***********************************************************************/
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
#include <linux/of_irq.h>
#include <linux/spinlock.h>
#include <linux/io.h>
#include <uapi/linux/input.h>
#include <linux/workqueue.h>
#include <linux/wait.h>
#include <linux/delay.h>
#include <linux/sysfs.h>
#include <linux/device.h>
#include <linux/kernel.h>

static int debug_status = 1;

// #define KEY_REBOOT_TEST		254	/* Elink None Screen Test Key */
#define KEY_VAL							KEY_REBOOT_TEST 
#define TAG								" <REBOOT-KEY> "
#define SET_LEVEL						KERN_INFO
#define usr_msg(fmt, args...)			printk(SET_LEVEL TAG fmt"\n", ##args)
#define debug_msg(fmt, args...)			do {                                                \
                                            if(debug_status)                                \
                                                printk(SET_LEVEL TAG fmt"\n", ##args);      \
                                        } while(0)

#define KEY_DEV_NAME						"r101u-test-key"			 

typedef enum {
	low = 0,
	high = 1
}gpio_status;

typedef enum {
	in = 0,
	out = !in
}gpio_dir;

static int reboot_key_set_value(int gpio, gpio_dir dir, gpio_status value);
static irqreturn_t mcu_trigger_handler(int irq, void *arg);

static const struct of_device_id reboot_key_match_table[] = {
	{ .compatible = "rockchip, reboot-test", },
	{/* KEEP THIS */},
};
MODULE_DEVICE_TABLE(of, reboot_key_match_table);

/*--------------------------------------------------------------------------------------------------------------*/
struct reboot_key_info {
	int 						reboot_trigger_io;
	int 						detect_interval;
	unsigned int 				irq;
	struct input_dev 			*inputdev;
	struct mutex 				lock;
	struct workqueue_struct 	* wq;
	struct work_struct			work;
	struct mutex				tim_lock;
};

/**----------------------------------------------------------------------------------*/

static int reboot_key_set_value(int gpio, gpio_dir dir, gpio_status value)
{
	int ret;
	
	if(in == dir) {
		ret = gpio_direction_input(gpio);
	} else {
		ret = gpio_direction_output(gpio, value);
	}
	return ret;
}

static int reboot_key_chip_init(struct reboot_key_info * info)
{
	int ret;
	ret = reboot_key_set_value(info->mcu_p42_gpio, out, high);
	ret = ret || reboot_key_set_value(info->mcu_p53_gpio, in, low);	
	if(ret < 0) {
		usr_msg("gpio: [%d], gpio: [%d] set direction failed.", info->mcu_p42_gpio, info->mcu_p53_gpio);
		return ret; 
	}
	debug_msg("gpio set pass, current mcu_p53_gpio = %d", gpio_get_value(info->mcu_p53_gpio));
	return 0;
}

static void data_handler(struct work_struct * work)
{
	int val;
	struct reboot_key_info * btn = container_of(work, struct reboot_key_info, work);
	if(IS_ERR(btn)) {
		usr_msg("error: get struct usr_keys_button in function: %s", __func__);
		return ;
	}
	usr_msg("moved in function: %s", __func__);

    mutex_lock(&btn->lock);
    mdelay(1000);
    val = gpio_get_value(btn->mcu_p53_gpio);
    debug_msg("pressed, report KEY_CODE = %d, KEY_VALUE = %d", KEY_VAL, val);
    mdelay(1000);
    input_report_key(btn->inputdev, KEY_VAL, val);
    input_sync(btn->inputdev);

    mutex_unlock(&btn->lock);
}

static irqreturn_t mcu_trigger_handler(int irq, void *arg)
{
	struct reboot_key_info * info = (struct reboot_key_info *)arg;
	if(!info) {
		usr_msg("error: reboot_key_info in function: %s", __func__);
		return IRQ_HANDLED;
	}
	debug_msg("triggered");
	disable_irq_nosync(irq);
	schedule_work(&info->work);
    return IRQ_HANDLED; 
}

static int usr_get_dts_info(struct reboot_key_info * info, struct device *dev)
{
	int ret;
	struct device_node *node = NULL;
	node = of_find_compatible_node(NULL, NULL, "rockchip, reboot-test");
	if (!node) {
		usr_msg("cannot find node name rockchip, reboot-test");
		return -ENODEV;
	}
	info->key_gpio_num = of_get_named_gpio(node, "reboot-key-gpios", 0);
	if(!gpio_is_valid(info->key_gpio_num)) {
		usr_msg("error: get reboot-key-gpios gpio info. ERROR code %d", info->key_gpio_num);
		return -EIO;
	}
    info->led_gpio_num = of_get_named_gpio(node, "led-gpio", 0);
    if(!gpio_is_valid(info->led_gpio_num)) {
        usr_msg("error: get info->led_gpio_num info.  ERROR code %d", info->led_gpio_num);
        return -EIO;
    }
	ret = devm_gpio_request_one(dev, info->key_gpio_num, GPIOF_IN, "reboot-key-gpios");
    if(ret < 0) {
		usr_msg("error: reboot-key-gpios request.");
		return -EIO;
	}
	ret = devm_gpio_request_one(dev, info->key_gpio_num, GPIOF_OUT_INIT_LOW, "led_gpio_num");
    if(ret < 0) {
		usr_msg("error: key_gpio_num request.");
		return -EIO;
	}

	debug_msg("requested key_gpio_num number: %d, led_gpio_num number: %d", info->key_gpio_num, info->led_gpio_num);
	ret = reboot_key_chip_init(info);
	if(ret < 0)
		return -EIO;
	
	return 0;

}


 static ssize_t led_reboot_start(struct device *dev, struct device_attribute*attr,const char *buf,size_t count)
 {
 	debug_msg("moved in function: %s", __func__);
	
 }

 static ssize_t led_reboot_running(struct device *dev, struct device_attribute*attr,const char *buf,size_t count)
 {
 
 }

 static ssize_t led_reboot_pass(struct device *dev, struct device_attribute*attr,const char *buf,size_t count)
{

}
 
static ssize_t led_reboot_failed(struct device *dev, struct device_attribute*attr,const char *buf,size_t count)
{

}

static ssize_t reboot_test_status(struct device *dev, struct device_attribute*attr, char *buf)
{

}


static DEVICE_ATTR(led_start_reboot, 			S_IWUSR | S_IRUGO, NULL, led_reboot_start);
static DEVICE_ATTR(led_runing_reboot,      		S_IWUSR | S_IRUGO, NULL, led_reboot_running);
static DEVICE_ATTR(led_test_pass_reboot,        S_IWUSR | S_IRUGO, NULL, led_reboot_pass);
static DEVICE_ATTR(led_test_failed_reboot,   	S_IWUSR | S_IRUGO, NULL, led_reboot_failed);
static DEVICE_ATTR(reboot_status,  	 			S_IWUSR | S_IRUGO, reboot_test_status, NULL);


static struct attribute *reboot_led_attributes[] = {
    &dev_attr_led_start_reboot.attr,
    &dev_attr_led_runing_reboot.attr,
    &dev_attr_led_test_pass_reboot.attr,
    &dev_attr_led_test_failed_reboot.attr,
    &dev_attr_reboot_status.attr,
    NULL
};



static int reboot_key_sysfs_create(struct platform_device * pdev)
{
	sysfs_create_group(&pdev->dev.kobj, &reboot_test_status_attr_group);
	device_create_file(struct device * dev, const struct device_attribute * attr);
}


static int reboot_key_probe(struct platform_device *pdev)
{
	int ret;

	struct reboot_key_info * info = devm_kmalloc(&pdev->dev, sizeof(struct reboot_key_info), GFP_KERNEL);
	if(IS_ERR(info)) {
		usr_msg("error: devm_kmalloc");
		return -ENOMEM;
	}
	debug_msg("moved in function: %s", __func__);
	mutex_init(&info->lock);

	platform_set_drvdata(pdev, info);
	dev_set_drvdata(&pdev->dev, info);
	

	info->inputdev = devm_input_allocate_device(&pdev->dev);
	if(IS_ERR(info->inputdev)){
		usr_msg("error: input_allocate_device failed!");
		return -ENODEV;
	}

	info->inputdev->name 		= KEY_DEV_NAME;				
	info->inputdev->phys 		= KEY_DEV_NAME;	
	info->inputdev->dev.parent 	= &pdev->dev;
	info->inputdev->id.bustype 	= BUS_HOST;		
    info->inputdev->id.vendor 	= 0x0001;
    info->inputdev->id.product 	= 0x0001;
    info->inputdev->id.version 	= 0x0100;
    info->inputdev->evbit[0] = BIT_MASK(EV_KEY);

	set_bit(KEY_VAL, info->inputdev->keybit);
	input_set_capability(info->inputdev, EV_KEY, KEY_VAL);
	
	ret = input_register_device(info->inputdev);
	if(ret){
		usr_msg("error: input register device failed!");
		goto error1;
	}


	ret = usr_get_dts_info(info, &pdev->dev);
	if(ret < 0) {
		ret = -ENODEV;
		usr_msg("error: get gpio dts info");
		goto error1;
	}

	info->wq = create_singlethread_workqueue("reboot_key_data_handler");
	if (!info->wq) {
		usr_msg("error: can not create workqueue");
		goto error2;
	}
	flush_workqueue(info->wq);
	INIT_WORK(&info->work, data_handler);

	info->irq = gpio_to_irq(info->mcu_p53_gpio);
	ret = request_irq(info->irq, mcu_trigger_handler, IRQF_TRIGGER_HIGH, "reboot_key_irq", (void *)info);
	if(ret < 0) {
		usr_msg("error: request irq");
		goto error3;
	}
	debug_msg("requested irq number = %d", info->irq);
	platform_set_drvdata(pdev, (void *) info);
	dev_set_drvdata(&pdev->dev, (void *) info);
	return 0;


error3:
	destroy_workqueue(info->wq);

error2:
	input_put_device(info->inputdev);
	input_free_device(info->inputdev);
error1:

   return ret;

}

static int reboot_key_remove(struct platform_device *pdev)
{
	struct reboot_key_info *btn = platform_get_drvdata(pdev);
	
	cancel_work_sync(&btn->work);
	flush_workqueue(btn->wq);
	destroy_workqueue(btn->wq);

	input_unregister_device(btn->inputdev);
	
	return 0;
}

#if 0
#ifdef CONFIG_PM	
static int reboot_key_suspend(struct device *dev)
{
	struct reboot_key_info *btn = dev_get_drvdata(dev);
	usr_msg("ready to suspend");
	disable_irq_nosync(btn->irq);

	cancel_work_sync(&btn->work);
	flush_workqueue(btn->wq);
	gpio_set_value(btn->touch_ic_rst, 0);
	usr_msg("customer key suspended.");
	return 0;
}

static int reboot_key_resume(struct device *dev)
{
	struct reboot_key_info *btn = dev_get_drvdata(dev);
	usr_msg("ready to resume");
	chip_cp2610_init(btn);

	enable_irq(btn->irq);
	usr_msg("customer key resumed operation.");
	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(reboot_key_pm_ops, reboot_key_suspend, reboot_key_resume);
#endif

static struct platform_driver reboot_key_driver = {
	.probe		= reboot_key_probe,
	.remove		= reboot_key_remove,
	.driver		= {
		.name	= KEY_DEV_NAME,
		.owner	= THIS_MODULE,
		// .pm	= &reboot_key_pm_ops,
		.of_match_table = of_match_ptr(reboot_key_match_table),
	},
};


module_platform_driver(reboot_key_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("V");
MODULE_DESCRIPTION("reboot_key control loop");
MODULE_ALIAS("reboot_key");

