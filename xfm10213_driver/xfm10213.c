
#include "./xfm10213.h"

static int upgrade_finished_mark;
static int xfm10213_update_flag;

static DECLARE_WAIT_QUEUE_HEAD(xfm10213_upgrade_work_head);

static void xfm10213_init_worker(struct work_struct *work);
static DECLARE_WORK(xfm10213_init_work, xfm10213_init_worker);


static const struct of_device_id xfm10213_match_table[] = {
	{ .compatible = "xfm10213"},
	{},
};
MODULE_DEVICE_TABLE(of, xfm10213_match_table);

static irqreturn_t xfm10213_irq_handler(int irq, void *data)
{
	struct xfm10213_info *info = data;
	struct i2c_client *client = info->client;
	usr_msg("moved in function: %s", __func__);
	upgrade_finished_mark = 1;
	disable_irq(info->irq_num);
	schedule_work(xfm10213_irq_init);
	return IRQ_HANDLED;
}

static int xfm10213_irq_request(struct xfm10213_info *info)
{
	struct fsa9480_platform_data *pdata = usbsw->pdata;
	struct i2c_client *client = usbsw->client;
	int ret;

	usr_msg("moved in function: %s", __func__);
	// using IRQF_ONESHOT to keep thread_handler full exit before accept next interrupt.
	ret = request_threaded_irq(client->irq, NULL, xfm10213_irq_handler, IRQF_TRIGGER_RISING | IRQF_ONESHOT, "fsa9480 micro USB", usbsw);
	if (ret) {
		usr_msg("failed to request IRQ");
		return ret;
	}
	return 0;
}


static int xfm10213_get_dts_info(struct i2c_client * client, struct xfm10213_info * info)
{
	int ret;
	enum of_gpio_flags flags;
	char temp_str[20] = 0;
	
	usr_msg("moved in function: %s", __func__);
	
info->node = client->dev.of_node;
	if(!info->node) {
		usr_msg("cannot find device node");
		return -ENODEV;
	}
	ret = of_get_named_gpio_flags(
info->node, "xfm10213-irq-gpio", 0, &flags);
	if(ret < 0) {
		usr_msg("error: get xfm10213-irq-gpio");
		return -EIO;
	}
	ret = of_property_read_string(info->node, "xfm10213-irq-trigger-level", &temp_str);
	if(ret < 0) {
		usr_msg("error: get xfm10213-irq-trigger-level");
		return -EIO;
	}
	if((!temp_str) || (0 == strncmp("high", temp_str, 4))
		info->irq_trigger_level = 1;
	else
		info->irq_trigger_level = 0;
	
	ret = of_property_read_u8(info->node, "reg", &info->addr);
	if(ret < 0) {
		usr_msg("error: get xfm10213-i2c-reg");
		return -EIO;
	}
	info->irq_num = gpio_to_irq(info->irq_pin);

	usr_msg("requested irq_pin = %d, irq_num = %d, setted irq-trigger-level = %s",
					info->irq_pin, info->irq_num, info->irq_trigger_level ? "HIGH" : "LOW");
	
}

static ssize_t xfm10213_update_set(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	usr_msg("moved in function: %s", __func__);
	return 0;
}

static ssize_t xfm10213_wrok_mode_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	usr_msg("moved in function: %s", __func__);
	return 0;
}
static ssize_t xfm10213_wrok_mode_set(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	usr_msg("moved in function: %s", __func__);
	return 0;
}
static ssize_t xfm10213_mic_channel_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	usr_msg("moved in function: %s", __func__);
	return 0;
}
static ssize_t xfm10213_mic_channel_set(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	usr_msg("moved in function: %s", __func__);
	return 0;
}
static ssize_t xfm10213_mic_gain_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	usr_msg("moved in function: %s", __func__);
	return 0;
}
static ssize_t xfm10213_mic_gain_set(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	usr_msg("moved in function: %s", __func__);
	return 0;
}
static ssize_t xfm10213_uart_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	usr_msg("moved in function: %s", __func__);
	return 0;
}
static ssize_t xfm10213_uart_set(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	usr_msg("moved in function: %s", __func__);
	return 0;
}
static ssize_t xfm10213_out_gain_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	usr_msg("moved in function: %s", __func__);
	return 0;
}
static ssize_t xfm10213_out_gain_set(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	usr_msg("moved in function: %s", __func__);
	return 0;
}
static ssize_t xfm10213_i2s_mode_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	usr_msg("moved in function: %s", __func__);
	return 0;
}
static ssize_t xfm10213_i2s_mode_set(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	usr_msg("moved in function: %s", __func__);
	return 0;
}

static DEVICE_ATTR(xfm10213_update, S_IRUGO, NULL, xfm10213_update_set);
static DEVICE_ATTR(xfm10213_wrok_mode, S_IRUGO, xfm10213_wrok_mode_show, xfm10213_wrok_mode_set);
static DEVICE_ATTR(xfm10213_mic_channel, S_IRUGO, xfm10213_mic_channel_show, xfm10213_mic_channel_set);
static DEVICE_ATTR(xfm10213_mic_gain, S_IRUGO, xfm10213_mic_gain_show, xfm10213_mic_gain_set);
static DEVICE_ATTR(xfm10213_uart, S_IRUGO, xfm10213_uart_show, xfm10213_uart_set);
static DEVICE_ATTR(xfm10213_out_gain, S_IRUGO, xfm10213_out_gain_show, xfm10213_out_gain_set);
static DEVICE_ATTR(xfm10213_i2s_mode, S_IRUGO, xfm10213_i2s_mode_show, xfm10213_i2s_mode_set);

static struct attribute * xfm10213_attributes[] = {
	&dev_attr_xfm10213_update.attr,
	&dev_attr_xfm10213_wrok_mode.attr,
	&dev_attr_xfm10213_mic_channel.attr,
	&dev_attr_xfm10213_mic_gain.attr,
	&dev_attr_xfm10213_uart.attr,
	&dev_attr_xfm10213_out_gain.attr,
	&dev_attr_xfm10213_i2s_mode.attr,
	NULL,
};
static const struct attribute_group xfm10213_attribute_group = {
	.attrs = xfm10213_attributes,
};

static void xfm10213_update_protocal(struct work_struct *work)
{

    wake_up_interruptible(&xfm10213_upgrade_work_head);
    usr_msg("queue woke-up, xfm10213_update_flag = %d", xfm10213_update_flag);
    xfm10213_update_flag= 0;
    schedule();
	
    
}

void xfm10213_update_waitqueue_init(void)
{
    init_waitqueue_head(&xfm10213_upgrade_work_head);
    INIT_WORK(&xfm10213_init, xfm10213_update_protocal);
    schedule_work(&xfm10213_init);
    wait_event_interruptible(xfm10213_upgrade_work_head, xfm10213_update_flag != 0);
    usr_msg("waitqueue, xfm10213_update_flag =  %d", xfm10213_update_flag);
}
	
static int xfm10213_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret;
	// struct i2c_adapter *adapter = to_i2c_adapter(client->dev.parent);
	struct i2c_adapter * adapter;
	struct xfm10213_info * info = devm_kmalloc(&client->dev, sizeof(struct xfm10213_info), GFP_KERNEL);
	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA)) {
		usr_msg("i2c check failed.");
		return -EIO;
	}

	ret = xfm10213_get_dts_info(client, info)
	if(ret < 0) {
		usr_msg("error: get dts info");
		return -EIO;
	}
	info->client = client;
    mutex_init(&info->lock);

	xfm10213_update_waitqueue_init();
	
	sysfs_create_group(&client->dev.kobj, &xfm10213_attribute_group);
	return 0;

	return ret;
}

static int xfm10213_remove(struct i2c_client *client)
{
	struct fsa9480_usbsw *usbsw = i2c_get_clientdata(client);
	if (client->irq)
		free_irq(client->irq, usbsw);

	sysfs_remove_group(&client->dev.kobj, &fsa9480_group);
	device_init_wakeup(&client->dev, 0);
	kfree(usbsw);
	return 0;
}

static const struct i2c_device_id xfm10213_dev_id[] = {
    {"xfm10213", 0},
    {/*keep this*/}
};
MODULE_DEVICE_TABLE(i2c, xfm10213_dev_id);

static struct i2c_driver xfm10213_driver = {
	.driver = {
		.name = XFM10213_DEV_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(xfm10213_match_table);
	},
	.probe = xfm10213_probe,
	.remove = xfm10213_remove,
	.id = xfm10213_dev_id,
};

static int __init xfm10213_init(void)
{
	usr_msg("moved in function: %s", __func__);
	i2c_add_driver(&xfm10213_driver);
}

static void _exit xfm10213_exit(void)
{
	usr_msg("moved in function: %s", __func__);
	i2c_del_driver(&xfm10213_driver);
}

// module_i2c_driver(xfm10213_driver);

module_init(xfm10213_init);
module_exit(xfm10213_exit);

MODULE_AUTHOR("V");
MODULE_DESCRIPTION("XFM10213 Driver");
MODULE_LICENSE("GPL");

