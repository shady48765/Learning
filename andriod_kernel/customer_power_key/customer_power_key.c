/*****************************************************************************
 * ; key define in dts
 *  [usr-home-button]
 *  compatible = "usr-home-button"
 *  touch-home-btn = port:PL10<6><1><default><default>
 *  touch-rst-ctrl = port:PL8<1><1><default><1>
 ******************************************************************************/
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


// define in /include/uapi/linux/input.h : #define #define KEY_HOMEPAGE 

// #define KEY_VAL							KEY_POWER	// 116
// #define KEY_VAL				    		KEY_HOMEPAGE		// android home key
#define KEY_NAME						"customer_power_keys"
#define TAG								"<KEY>"
#define SET_LEVEL						KERN_INFO
#define usr_msg(fmt, args...)			printk(SET_LEVEL TAG fmt"\n", ##args)

typedef enum {
	low = 0,
	high = !low
}gpio_status;

static struct of_device_id key_dts_table[] = {
	{ .compatible = "rockchip,power-key", },
	{ },
};
MODULE_DEVICE_TABLE(of, key_dts_table);

/*--------------------------------------------------------------------------------------------------------------*/
#define HRT_TIMER				0

#define MS_TO_NS(x) (x * 1000000)      // ms to ns

struct keys {
	char *key_label;
	int pin_num;
	unsigned int linux_code;
	
};
struct customer_keys {
	struct keys 				* pwr_ctrl;
	struct keys 				* power;
	unsigned int 				irq;
	struct input_dev 			*inputdev;
	struct mutex 				lock;
	struct workqueue_struct 	* wq;
	struct work_struct			work;

	struct keys 				keys[2];
};

struct timer_list tim;

static int trigger_flag;

static void data_handler(struct work_struct * work)
{
	int key_val;
	struct customer_keys * btn = container_of(work, struct customer_keys, work);
	if(IS_ERR(btn)) {
		usr_msg("error: get struct usr_keys_button in function: %s", __func__);
		return ;
	}
	usr_msg("moved in %s", __func__);
	if(0 == trigger_flag) {
		usr_msg("handler button half schedule");
		return ;
	} else {
		mutex_lock(&btn->lock);
		key_val = gpio_get_value(btn->touch_home_btn);
		if(key_val) {
			mdelay(150);
			if(key_val == gpio_get_value(btn->touch_home_btn)) {
				usr_msg("release");
				input_report_key(btn->inputdev, KEY_VAL, 1);
				input_sync(btn->inputdev);
			} else
				goto out;
		} else {
			mdelay(150);
			if(key_val == gpio_get_value(btn->touch_home_btn)) {
	        	usr_msg("pressed");
	        	input_report_key(btn->inputdev, KEY_VAL, 0);
				input_sync(btn->inputdev);
			} else
				goto out;
		}
		trigger_flag = 0;
		mutex_unlock(&btn->lock);
	}

	return ;
out:
	trigger_flag = 0;
	mutex_unlock(&btn->lock);
	return ;
}

static irqreturn_t usr_key_handler(int irq, void *arg)
{
	struct customer_keys * btn = (struct customer_keys *)arg;
	if(!btn) {
		usr_msg("error: usr_keys_button in function: %s", __func__);
		return IRQ_HANDLED;
	}
	usr_msg("triggred");
    trigger_flag += 1;
	schedule_work(&btn->work);
    return IRQ_HANDLED; 
}

static int keys_get_dts_info(struct customer_keys * btn, struct platform_device *pdev)
{
	int ret, index;
	unsigned int flags;
	struct device_node *child_node;
	struct device_node *node = pdev->dev.of_node;
	if(!node) {
		usr_msg("error: get dev.of_node");
		return -ENODEV;
	}
	index = 0;
	for_each_child_of_node(node, child_node) {
		btn->keys[index].key_label = of_get_property(child_node, "label", NULL);
		if(!strncmp("power", btn->keys[index].key_label, 5) {
			if (of_property_read_u32(child_node, "linux,code", &btn->keys[index].linux_code)) {
				usr_msg("error:  cannot read power-key linux,code");
				return = -EINVAL;
			}
			btn->keys[index].pin_num =  of_get_named_gpio(child_node, "pwr-key", 0);
		    if(!gpio_is_valid(btn->keys[index].pin_num)) {
        		usr_msg("error: get pwr-key gpio info.  ERROR code %d", btn->keys[index].pin_num);
        		return -EIO;
    		}
			ret = devm_gpio_request_one(dev, btn->keys[index].pin_num, GPIOF_OUT_INIT_HIGH, "pwr-key");
			if(ret < 0) {
				usr_msg("error: gpio pwr-key request.");
				return -EIO;
			}
			gpio_direction_input(btn->keys[index].pin_num);
			// btn->power = btn->keys[index].pin_num;
			btn->power = &btn->keys[index];
		} else {
			// ctrl pin don't have linux,code
			btn->keys[index].pin_num =  of_get_named_gpio(child_node, "pwr-ctrl-key", 0);
		    if(!gpio_is_valid(btn->keys[index].pin_num)) {
        		usr_msg("error: get pwr-ctrl-key gpio info.  ERROR code %d", btn->keys[index].pin_num);
        		return -EIO;
    		}
			ret = devm_gpio_request_one(dev, btn->keys[index].pin_num, GPIOF_OUT_INIT_HIGH, "pwr-ctrl-key");
			if(ret < 0) {
				usr_msg("error: gpio pwr-ctrl-key request.");
				return -EIO;
			}
			gpio_direction_output(btn->keys[index].pin_num);
			// btn->pwr_ctrl = btn->keys[index].pin_num;
			btn->pwr_ctrl = &btn->keys[index];
		}
		index++;
	}
	usr_msg("requested pwr-key number: %d, pwr-ctrl-key number: %d", btn->power->pin_num , btn->pwr_ctrl->pin_num);

	return 0;
}

static int key_probe(struct platform_device *pdev)
{
	int ret;
	ret = of_get_child_count(pdev->dev.of_node);
	if(ret < 0) {
		usr_msg("error: no keys");
		return ret;
	}
	
	struct customer_keys * btn = devm_kmalloc(&pdev->dev, sizeof(struct customer_keys), GFP_KERNEL);
	if(IS_ERR(btn)) {
		usr_msg("error: devm_kmalloc");
		return -ENOMEM;
	}
	mutex_init(&btn->lock);

	platform_set_drvdata(pdev, btn);
	dev_set_drvdata(&pdev->dev, btn);
	
	btn->inputdev = devm_input_allocate_device(&pdev->dev);
	if(IS_ERR(btn->inputdev)){
		usr_msg("error: input_allocate_device failed!");
		return -ENODEV;
	}

	btn->inputdev->name 		= KEY_NAME;				
	btn->inputdev->phys 		= KEY_NAME;	
	btn->inputdev->dev.parent 	= &pdev->dev;
	btn->inputdev->id.bustype 	= BUS_HOST;		
    btn->inputdev->id.vendor 	= 0x0001;
    btn->inputdev->id.product 	= 0x0001;
    btn->inputdev->id.version 	= 0x0100;
    btn->inputdev->evbit[0] = BIT_MASK(EV_KEY);

	set_bit(btn->power->linux_code, btn->inputdev->keybit);
	input_set_capability(btn->inputdev, EV_KEY, btn->power->linux_code);
	
	ret = input_register_device(btn->inputdev);
	if(ret){
		usr_msg("error: input register device failed!");
		goto error1;
	}
	
	ret = keys_get_dts_info(btn, pdev);
	if(ret < 0) {
		ret = -ENODEV;
		usr_msg("error: get gpio dts info");
		goto error2;
	}

	btn->wq = create_singlethread_workqueue("key_data_handler");
	if (!btn->wq) {
		usr_msg("error: can not create workqueue");
		goto error2;
	}
	flush_workqueue(btn->wq);
	INIT_WORK(&btn->work, data_handler);

	btn->irq = gpio_to_irq(btn->power->pin_num);
	ret = request_irq(btn->irq, usr_key_handler, IRQF_TRIGGER_RISING, "powr-key-irq", (void *)btn);
	if(ret < 0) {
		usr_msg("error: request irq");
		goto error3;
	}
	usr_msg("requested irq number = %d", btn->irq);
	platform_set_drvdata(pdev, (void *) btn);
	dev_set_drvdata(&pdev->dev, (void *) btn);
	return 0;

error3:
	destroy_workqueue(btn->wq);
error2:
	input_put_device(btn->inputdev);
error1:
   input_free_device(btn->inputdev);
   return ret;
}

static int key_remove(struct platform_device *pdev)
{
	struct usr_keys_button *btn = platform_get_drvdata(pdev);
	
	cancel_work_sync(&btn->work);
	flush_workqueue(btn->wq);
	destroy_workqueue(btn->wq);
	input_unregister_device(btn->inputdev);
	
	return 0;
}


#ifdef CONFIG_PM	
static int key_suspend(struct device *dev)
{
	struct usr_keys_button *btn = dev_get_drvdata(dev);
	usr_msg("ready to suspend");
	disable_irq_nosync(btn->irq);
	cancel_work_sync(&btn->work);
	flush_workqueue(btn->wq);
	usr_msg("key suspended.");
	return 0;
}

static int key_resume(struct device *dev)
{
	struct usr_keys_button *btn = dev_get_drvdata(dev);
	usr_msg("ready to resume");
	enable_irq(btn->irq);
	usr_msg("key resumed operation.");
	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(key_ops, key_suspend, key_resume);

static struct platform_driver keys_pdrv = {
	.probe		= key_probe,
	.remove		= key_remove,
	.driver		= {
		.name	= KEY_NAME,
		.owner	= THIS_MODULE,
		.pm	= &key_ops,
		.of_match_table = of_match_ptr(key_dts_table),
	},
};

module_platform_driver(keys_pdrv);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("V");
MODULE_DESCRIPTION("customer gpio key define");
MODULE_ALIAS("customer-keys");
