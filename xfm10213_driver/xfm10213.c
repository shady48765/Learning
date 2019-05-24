/**********************************************************************************
    xfm10213@3e {
		compatible = "xfm10213";
		status = "okay";
		reg = <0x3e>;
        xfm10213-irq-gpio = <&gpio5 RK_PB2 GPIO_ACTIVE_HIGH>;
        xfm10213-irq-trigger-level = "high";
	};
***********************************************************************************/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/platform_data/fsa9480.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/pm_runtime.h>
#include <linux/of.h>


#define XFM10213_DEV_NAME				"xfm10213_device"

#define TAG								" <XFM10213> "
#define SET_LEVEL						KERN_INFO
#define usr_msg(fmt, args...)			printk(SET_LEVEL TAG fmt"\n", ##args)

struct xfm10213_info {
	int irq_pin;
	int irq_num;
	int irq_trigger_level;
	
	unsigned char addr;
	mutex lock;
	
	struct i2c_client * client;
	struct device_node * node;
}


static const struct of_device_id xfm10213_match_table[] = {
	{ .compatible = "xfm10213"},
	{},
};
MODULE_DEVICE_TABLE(of, xfm10213_match_table);


static int fsa9480_write_reg(struct i2c_client *client,
		int reg, int value)
{
	int ret;

	ret = i2c_smbus_write_byte_data(client, reg, value);

	if (ret < 0)
		dev_err(&client->dev, "%s: err %d\n", __func__, ret);

	return ret;
}

static int fsa9480_read_reg(struct i2c_client *client, int reg)
{
	int ret;

	ret = i2c_smbus_read_byte_data(client, reg);

	if (ret < 0)
		dev_err(&client->dev, "%s: err %d\n", __func__, ret);

	return ret;
}

static int fsa9480_read_irq(struct i2c_client *client, int *value)
{
	int ret;

	ret = i2c_smbus_read_i2c_block_data(client,
			FSA9480_REG_INT1, 2, (u8 *)value);
	*value &= 0xffff;

	if (ret < 0)
		dev_err(&client->dev, "%s: err %d\n", __func__, ret);

	return ret;
}

/**----------------------------------------------------------------------------*/
static ssize_t fsa9480_get_switch(char *buf)
{
	struct fsa9480_usbsw *usbsw = chip;
	struct i2c_client *client = usbsw->client;
	unsigned int value;

	value = fsa9480_read_reg(client, FSA9480_REG_MANSW1);

	if (value == SW_VAUDIO)
		return sprintf(buf, "VAUDIO\n");
	else if (value == SW_UART)
		return sprintf(buf, "UART\n");
	else if (value == SW_AUDIO)
		return sprintf(buf, "AUDIO\n");
	else if (value == SW_DHOST)
		return sprintf(buf, "DHOST\n");
	else if (value == SW_AUTO)
		return sprintf(buf, "AUTO\n");
	else
		return sprintf(buf, "%x", value);
}

static ssize_t fsa9480_show_device(struct device *dev,
				   struct device_attribute *attr,
				   char *buf)
{
	struct fsa9480_usbsw *usbsw = dev_get_drvdata(dev);
	struct i2c_client *client = usbsw->client;
	int dev1, dev2;

	dev1 = fsa9480_read_reg(client, FSA9480_REG_DEV_T1);
	dev2 = fsa9480_read_reg(client, FSA9480_REG_DEV_T2);

	if (!dev1 && !dev2)
		return sprintf(buf, "NONE\n");

	/* USB */
	if (dev1 & DEV_T1_USB_MASK || dev2 & DEV_T2_USB_MASK)
		return sprintf(buf, "USB\n");

	/* UART */
	if (dev1 & DEV_T1_UART_MASK || dev2 & DEV_T2_UART_MASK)
		return sprintf(buf, "UART\n");

	/* CHARGER */
	if (dev1 & DEV_T1_CHARGER_MASK)
		return sprintf(buf, "CHARGER\n");

	/* JIG */
	if (dev2 & DEV_T2_JIG_MASK)
		return sprintf(buf, "JIG\n");

	return sprintf(buf, "UNKNOWN\n");
}

static ssize_t fsa9480_show_manualsw(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return fsa9480_get_switch(buf);

}

static ssize_t fsa9480_set_manualsw(struct device *dev,
				    struct device_attribute *attr,
				    const char *buf, size_t count)
{
	fsa9480_set_switch(buf);

	return count;
}

static DEVICE_ATTR(device, S_IRUGO, fsa9480_show_device, NULL);
static DEVICE_ATTR(switch, S_IRUGO | S_IWUSR,
		fsa9480_show_manualsw, fsa9480_set_manualsw);

static struct attribute *fsa9480_attributes[] = {
	&dev_attr_device.attr,
	&dev_attr_switch.attr,
	NULL
};

static const struct attribute_group fsa9480_group = {
	.attrs = fsa9480_attributes,
};
/**----------------------------------------------------------------------------*/
static irqreturn_t fsa9480_irq_handler(int irq, void *data)
{
	struct fsa9480_usbsw *usbsw = data;
	struct i2c_client *client = usbsw->client;
	int intr;

	/* clear interrupt */
	fsa9480_read_irq(client, &intr);

	/* device detection */
	fsa9480_detect_dev(usbsw, intr);

	return IRQ_HANDLED;
}

static int fsa9480_irq_init(struct fsa9480_usbsw *usbsw)
{
	struct fsa9480_platform_data *pdata = usbsw->pdata;
	struct i2c_client *client = usbsw->client;
	int ret;
	int intr;
	unsigned int ctrl = CON_MASK;

	/* clear interrupt */
	fsa9480_read_irq(client, &intr);

	/* unmask interrupt (attach/detach only) */
	fsa9480_write_reg(client, FSA9480_REG_INT1_MASK, 0xfc);
	fsa9480_write_reg(client, FSA9480_REG_INT2_MASK, 0x1f);

	usbsw->mansw = fsa9480_read_reg(client, FSA9480_REG_MANSW1);

	if (usbsw->mansw)
		ctrl &= ~CON_MANUAL_SW;	/* Manual Switching Mode */

	fsa9480_write_reg(client, FSA9480_REG_CTRL, ctrl);

	if (pdata && pdata->cfg_gpio)
		pdata->cfg_gpio();

	if (client->irq) {
		ret = request_threaded_irq(client->irq, NULL,
				fsa9480_irq_handler,
				IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
				"fsa9480 micro USB", usbsw);
		if (ret) {
			dev_err(&client->dev, "failed to request IRQ\n");
			return ret;
		}

		if (pdata)
			device_init_wakeup(&client->dev, pdata->wakeup);
	}

	return 0;
}
static void xfm10213_chip_init(struct i2c_client *client, struct xfm10213_info * info)
{
	
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
	ret = of_get_named_gpio_flags(info->node, "xfm10213-irq-gpio", 0, &flags);
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

static int xfm10213_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct i2c_adapter *adapter = to_i2c_adapter(client->dev.parent);
	struct fsa9480_usbsw *usbsw;
	int ret = 0;

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA)) {
		usr_msg("i2c check failed.");
		return -EIO;
	}

	usbsw = kzalloc(sizeof(struct fsa9480_usbsw), GFP_KERNEL);
	if (!usbsw) {
		dev_err(&client->dev, "failed to allocate driver data\n");
		return -ENOMEM;
	}

	usbsw->client = client;
	usbsw->pdata = client->dev.platform_data;

	chip = usbsw;

	i2c_set_clientdata(client, usbsw);

	ret = fsa9480_irq_init(usbsw);
	if (ret)
		goto fail1;

	ret = sysfs_create_group(&client->dev.kobj, &fsa9480_group);
	if (ret) {
		dev_err(&client->dev,
				"failed to create fsa9480 attribute group\n");
		goto fail2;
	}

	/* ADC Detect Time: 500ms */
	fsa9480_write_reg(client, FSA9480_REG_TIMING1, 0x6);

	if (chip->pdata->reset_cb)
		chip->pdata->reset_cb();

	/* device detection */
	fsa9480_detect_dev(usbsw, INT_ATTACH);

	pm_runtime_set_active(&client->dev);

	return 0;

fail2:
	if (client->irq)
		free_irq(client->irq, usbsw);
fail1:
	kfree(usbsw);
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

static int xfm10213_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;
	int ret;

	printk(DEBUG_INIT, "%s enter \n", __func__);
	
	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA)){
		dprintk(DEBUG_INIT, "%s i2c_check_functionality error \n", __func__);
		return -ENODEV;
	}
            
	if (twi_id == adapter->nr) {
			client->addr = i2c_address[i2c_num];
			dprintk(DEBUG_INIT,"%s:addr= 0x%x,i2c_num:%d\n",__func__,client->addr,i2c_num);
			ret = i2c_smbus_read_byte_data(client,VC8536_CHIP_ID_REG);
			dprintk(DEBUG_INIT,"Read ID value is :%#x",ret);
			strlcpy(info->type, SENSOR_NAME, I2C_NAME_SIZE);
			return 0; 
        
	} else {
			dprintk(DEBUG_INIT, "%s error adapter->nr==%d \n", __func__,adapter->nr);
			return -ENODEV;
	}
}

static struct i2c_driver xfm10213_driver = {
	.driver = {
		.name = XFM10213_DEV_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(xfm10213_match_table);
	},
	.probe = xfm10213_probe,
	.remove = xfm10213_remove,
	.detect = xfm10213_detect,
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

