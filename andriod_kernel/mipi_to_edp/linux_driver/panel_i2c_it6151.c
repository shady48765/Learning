#include <drm/drmP.h>
#include <drm/drm_panel.h>

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
#include <linux/i2c.h>

#include "it6151_global.h"


#define DEVICE_NAME "IT6151"

static struct i2c_client *it6151_mipirx = NULL;
static struct i2c_client *it6151_edp = NULL;


static DEFINE_MUTEX(it6151_i2c_access);

typedef enum {
	low = 0,
	high = 1
}gpio_status_enum;


static struct i2c_client *it6151_mipirx = NULL;
static struct i2c_client *it6151_edp = NULL;


static void it6151_power_on(struct it6151_info * info)
{
	gpio_set_value(info->gpio_stdby_pwr, high);
	mdelay(20);
}
static void it6151_power_off(struct it6151_info * info)
{
	gpio_set_value(info->gpio_stdby_pwr, low);
	mdelay(20);
}

static void it6151_chip_init(struct it6151_info * info)
{
	it6151_power_on(info);
	
}

static int usr_get_dts_info(struct it6151_info * info, struct i2c_client * client)
{
	int ret;
		
	struct device_node *node = NULL;
	
	node = of_find_compatible_node(NULL, NULL, "it6151,chip_i2c");
	
	if (!node) {
		usr_msg("cannot find node name rockchip, it6151,chip_i2c");
		return -ENODEV;
	}
	info->gpio_init = of_get_named_gpio(node, "it6151-init-gpio", 0);
	if(!gpio_is_valid(info->gpio_init)) {
		usr_msg("error: get it6151-init-gpio gpio info.");
		return -EIO;
	}
    info->gpio_stdby_pwr = of_get_named_gpio(node, "it6151-stdby-pwr-gpio", 0);
    if(!gpio_is_valid(info->gpio_stdby_pwr)) {
        usr_msg("error: get gpio_stdby_pwr gpio info");
        return -EIO;
    }
	ret = devm_gpio_request_one(&client->dev , info->gpio_init, GPIOF_IN, "it6151-init");
	if(ret < 0) {
		usr_msg("error: it6151-init request.");
		return -EIO;
	}
	ret = devm_gpio_request_one(&client->dev, info->gpio_stdby_pwr, GPIOF_OUT_INIT_HIGH, "it6151-standby");
    if(ret < 0) {
		usr_msg("error: it6151-standby request.");
		ret = -EIO;
		goto out1;
	}
	info->irq_num = gpio_to_irq(info->gpio_init);
	usr_msg("requested gpio_init: %d, gpio_standby: %d, irq_num = %d", 
						info->gpio_init, info->gpio_stdby_pwr, info->irq_num);
	if(ret < 0)
		goto out1;
	
	return 0;
	
out1:
	return ret;

}


int it6151_i2c_read_byte(uint8_t dev_addr, u8 addr, uint8_t *returnData)
{
	struct i2c_msg msg[2];
	char     readData = 0;
	int      ret=0;

	mutex_lock(&it6151_i2c_access);

	if(dev_addr == IT6151_MIPIRX_SLAVE_ADDR_WRITE)
	{		

		msg[0].addr = dev_addr







	
		it6151_mipirx->ext_flag=((it6151_mipirx->ext_flag) & I2C_MASK_FLAG ) | I2C_WR_FLAG | I2C_DIRECTION_FLAG;
		
		cmd_buf[0] = addr;
		ret = i2c_master_send(it6151_mipirx, &cmd_buf[0], (1<<8 | 1));
		
		if (ret < 0)
		{
			it6151_mipirx->ext_flag=0;
		
			mutex_unlock(&it6151_i2c_access);
		
			return 0;
		}
		
		readData = cmd_buf[0];
		*returnData = readData;
		
		it6151_mipirx->ext_flag=0;
	}
	else if(dev_addr == IT6151_EDP_SLAVE_ADDR_WRITE)
	{
		it6151_edp->flags = ;
		cmd_buf[0] = addr;
		ret = i2c_master_send(it6151_edp, &cmd_buf[0], (1<<8 | 1));

		if (ret < 0)
		{
			it6151_edp->ext_flag=0;

			mutex_unlock(&it6151_i2c_access);

			return 0;
		}

		readData = cmd_buf[0];
		*returnData = readData;

		it6151_edp->ext_flag=0;
	}
	else
		usr_msg("[it6151_i2c_read_byte]error:  no this dev_addr! \n");
	
	mutex_unlock(&it6151_i2c_access);

	return 1;
}

int it6151_i2c_write_byte(u8 dev_addr, u8 addr, u8 writeData)
{
	char    write_data[2] = {0};
	int     ret=0;

	mutex_lock(&it6151_i2c_access);

	write_data[0] = addr;
	write_data[1] = writeData;

	if(dev_addr == IT6151_MIPIRX_SLAVE_ADDR_WRITE)
	{
		it6151_mipirx->addr = IT6151_MIPIRX_SLAVE_ADDR_WRITE;
		it6151_mipirx->ext_flag=((it6151_mipirx->ext_flag) & I2C_MASK_FLAG) | I2C_DIRECTION_FLAG;
		
		ret = i2c_master_send(it6151_mipirx, write_data, 2);
		
		if(ret < 0)
		{
			it6151_mipirx->ext_flag=0;
			mutex_unlock(&it6151_i2c_access);
			
			return 0;
		}
		
		it6151_mipirx->ext_flag=0;
	}
	else if(dev_addr == IT6151_EDP_SLAVE_ADDR_WRITE)
	{
		it6151_edp->addr = IT6151_EDP_SLAVE_ADDR_WRITE;
		it6151_edp->ext_flag=((it6151_edp->ext_flag) & I2C_MASK_FLAG) | I2C_DIRECTION_FLAG;

		ret = i2c_master_send(it6151_edp, write_data, 2);

		if (ret < 0)
		{
			it6151_edp->ext_flag=0;
			mutex_unlock(&it6151_i2c_access);
				
			return 0;
		}

		it6151_edp->ext_flag=0;
	}	
	else
	{
#ifndef BUILD_LK
		printk("[Kernel/LCM] [it6151_i2c_write_byte]error:  no this dev_addr! \n");
#else
		printf("[LK/LCM] [it6151_i2c_write_byte]error:  no this dev_addr! \n");
#endif	
	 
	}

	mutex_unlock(&it6151_i2c_access);

	return 1;
}




static int it6151_i2c_probe(struct i2c_client * client, const struct i2c_device_id * dev_id)
{
	// int ret;
	
	struct it6151_info * info = devm_kmalloc(&client->dev, sizeof(struct it6151_info), GFP_KERNEL);
	if(IS_ERR(info)) {
		usr_msg("error: devm_kmalloc");
		return -ENOMEM;
	}
	usr_msg("---> moved in function");

	info->it6151_client = client;
	i2c_set_clientdata(client, (void *)info);
	usr_get_dts_info(info, client);

	it6151_i2c_init_flag = 1;

	return 0;
}

static int it6151_i2c_remove(struct i2c_client * client)
{
	return 0;
}




static const struct i2c_device_id it6151_id_table[] = 
{
	{"it6151,chip_i2c", 0},
	{/** keep this */}
};
MODULE_DEVICE_TABLE(of, it6151_id_table);
	

static const struct of_device_id it6151_match_table[] = {
	{.compatible = "it6151,chip_i2c"},
	/** keep this */
};
MODULE_DEVICE_TABLE(i2c, it6151_match_table);


static struct i2c_driver it6151_driver = {
	.probe = it6151_i2c_probe,
	.remove = it6151_i2c_remove,
	.driver = {
		.name = "it6151",
		.of_match_table = it6151_match_table,
	},
	.id_table = it6151_id_table,
};

module_i2c_driver(it6151_driver);

MODULE_AUTHOR("V <machester@aliyun.com>");
MODULE_DESCRIPTION("DRM Driver for mipi to edp transder chip it6151");
MODULE_LICENSE("GPL");














