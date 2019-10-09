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

#define IT6151_EDP_SLAVE_ADDR_WRITE						(0x5C << 0)
#define	IT6151_MIPIRX_SLAVE_ADDR_WRITE 					(0x6C << 0)
#define DP_I2C_ADDR 	(0x5C << 0)
#define MIPI_I2C_ADDR 	(0x6C << 0)
#define REGFLAG_DELAY 	(0xAB)


#define I2C_WRITE_FLAG			0
#define I2C_READ_FLAG			1


static struct i2c_client *it6151_mipirx = NULL;
static struct i2c_client *it6151_edp = NULL;


struct mutex it6151_i2c_access;


typedef enum {
	low = 0,
	high = 1
}gpio_status_enum;


static const struct i2c_device_id it6151_id_table[] = {
	{.name = "it6151_mipirx", 	.driver_data = 0,},
	{.name = "it6151_edp", 		.driver_data = 0,},
};

MODULE_DEVICE_TABLE(of, it6151_id_table);

#if 0
static DECLARE_WAIT_QUEUE_HEAD(it6151_init_queue_head);
struct it6151_init_queue {
	struct work_struct	 		it6151_init_wq_struct;
	struct it6151_info			*it6151_info_in_workstruct;
};

static void it6151_init_wq_recall(struct work_struct *work)
{
	struct it6151_init_queue * queue_info = container_of(work, struct it6151_init_queue, it6151_init_wq_struct);
	if(!queue_info) {
		usr_msg("get struct it6151_init_wq_recall in function %s failed", __func__);
	}
	if(2 != it6151_i2c_init_flag)
		schedule();
    usr_msg("get in it6151_init_wq_recall, it6151_i2c_init_flag = %d", it6151_i2c_init_flag);
	it6151_chip_init();
	it6151_i2c_init_flag = 0;
}

static void it6151_create_chip_init_wq(struct it6151_info * info)
{
	struct it6151_init_queue * queue_info = kmalloc(sizeof(struct it6151_init_queue), GFP_KERNEL);
	queue_info->it6151_info_in_workstruct = info;
	init_waitqueue_head(&it6151_init_queue_head);
	INIT_WORK(&queue_info->it6151_init_wq_struct, it6151_init_wq_recall);
	// it6151_init_wq_struct.data = (atomic_long_t)info;
	schedule_work(&queue_info->it6151_init_wq_struct);
	wait_event_interruptible(it6151_init_queue_head, it6151_i2c_init_flag == 2);
	usr_msg("waitqueue_created, init codition =  %d", it6151_i2c_init_flag);
}

#endif
void it6151_power_on(struct it6151_info * info)
{
	gpio_set_value(info->gpio_stdby_pwr, high);
	mdelay(20);
}

// static void it6151_power_off(struct it6151_info * info)
// {
// 	gpio_set_value(info->gpio_stdby_pwr, low);
// 	mdelay(20);
// }

#if 0
void it6151_chip_init(void)
{
	unsigned char RevID;
	usr_msg("moved in function: %s");
	it6151_i2c_write_byte(DP_I2C_ADDR, 0x15, (it6151_i2c_read_byte(DP_I2C_ADDR, 0x15,&RevID)) | 0x80 );
	it6151_i2c_write_byte(DP_I2C_ADDR, 0x22, (it6151_i2c_read_byte(DP_I2C_ADDR, 0x22,&RevID)) | 0x10 ); 
	mdelay(10);
}
#endif

int usr_get_dts_info(struct it6151_info * info, struct i2c_client * client)
{
	int ret;
		
	struct device_node *node = NULL;
	
	// node = of_find_compatible_node(NULL, NULL, "it6151_edp");
	// if (!node) {
		// usr_msg("cannot find node name rockchip, it6151_edp");
		// return -ENODEV;
	//}
	usr_msg("device node name: %s", client->dev.of_node->name);
	if(!client->dev.of_node) {
		usr_msg("device node is empty");
		return -ENODEV;
	}
	node = client->dev.of_node;
    info->gpio_stdby_pwr = of_get_named_gpio(node, "standby-pwr-gpio", 0);
    if(!gpio_is_valid(info->gpio_stdby_pwr)) {
        usr_msg("error: get standby-pwr-gpio gpio info");
        return -EIO;
    }

	info->gpio_int = of_get_named_gpio(node, "interrupt-gpio", 0);
	if(!gpio_is_valid(info->gpio_int)) {
		usr_msg("error: get interrupt-gpio gpio info.");
		return -EIO;
	}

	ret = devm_gpio_request_one(&client->dev , info->gpio_int, GPIOF_IN, "it6151-int");
	if(ret < 0) {
		usr_msg("error: it6151-int request.");
		return -EIO;
	}
	ret = devm_gpio_request_one(&client->dev, info->gpio_stdby_pwr, GPIOF_OUT_INIT_HIGH, "it6151-stby-pwr");
    if(ret < 0) {
		usr_msg("error: it6151-standby request.");
		ret = -EIO;
		goto out1;
	}
	info->irq_num = gpio_to_irq(info->gpio_int);
	usr_msg("requested gpio_int: %d, gpio_standby: %d, irq_num = %d", 
						info->gpio_int, info->gpio_stdby_pwr, info->irq_num);
	if(ret < 0)
		goto out1;
	
	return 0;
	
out1:
	return ret;

}


int it6151_i2c_read_byte(uint8_t adpt_addr, uint8_t reg_addr, uint8_t *data)
{
	struct i2c_msg msg[2];
	char data_tmp = 0;
	int ret = 0;


	mutex_lock(&it6151_i2c_access);

	if(adpt_addr == IT6151_MIPIRX_SLAVE_ADDR_WRITE) {
		msg[0].addr = adpt_addr;
		msg[0].flags = it6151_mipirx->flags & I2C_WRITE_FLAG;
		msg[0].len = 1;
		msg[0].buf = &reg_addr;
		msg[1].addr = adpt_addr;
		msg[1].flags = I2C_M_RD;
		msg[1].len = 1;
		msg[1].buf = &data_tmp;
		ret = i2c_transfer(it6151_mipirx->adapter, msg, 2);
		usr_msg("IT6151_MIPIRX: ret = %d", ret);
				
	} else if(adpt_addr == IT6151_EDP_SLAVE_ADDR_WRITE) {
		msg[0].addr = adpt_addr;
		msg[0].flags = it6151_mipirx->flags & I2C_WRITE_FLAG;
		msg[0].len = 1;
		msg[0].buf = &reg_addr;
		msg[1].addr = adpt_addr;
		msg[1].flags = I2C_M_RD;
		msg[1].len = 1;
		msg[1].buf = &data_tmp;
		ret = i2c_transfer(it6151_edp->adapter, msg, 2);
		usr_msg("IT6151_EDP:  ret = %d", ret);

	}else {
		usr_msg("[it6151_i2c_read_byte]error:  no this dev_addr! \n");
	}
	if(ret < 0) {
		usr_msg("IT6151_EDP:  ret = %d", ret);
		mutex_unlock(&it6151_i2c_access);
		return ret;
	}
	*data = data_tmp;
	mutex_unlock(&it6151_i2c_access);

	return ret;
}

int it6151_i2c_write_byte(uint8_t adpt_addr, uint8_t reg_addr, uint8_t data)
{
	struct i2c_msg msg;
	char msg_buff[] = {reg_addr, data};
	int ret = 0;


	mutex_lock(&it6151_i2c_access);

	if(adpt_addr == IT6151_MIPIRX_SLAVE_ADDR_WRITE) {
		msg.addr = adpt_addr;
		msg.flags = it6151_mipirx->flags & I2C_WRITE_FLAG;
		msg.len = 2;
		msg.buf = msg_buff;
		ret = i2c_transfer(it6151_mipirx->adapter, &msg, 1);
		usr_msg("IT6151_MIPIRX: ret = %d", ret);
		
	} else if(adpt_addr == IT6151_EDP_SLAVE_ADDR_WRITE) {
		msg.addr = adpt_addr;
		msg.flags = it6151_mipirx->flags & I2C_WRITE_FLAG;
		msg.len = 2;
		msg.buf = msg_buff;
		ret = i2c_transfer(it6151_edp->adapter, &msg, 1);
		usr_msg("IT6151_EDP:  ret = %d", ret);

	}else {
		usr_msg("[it6151_i2c_read_byte]error:  no this dev_addr! \n");
	}
	if(ret < 0) {
		usr_msg("IT6151_EDP:  ret = %d", ret);
		mutex_unlock(&it6151_i2c_access);
		return ret;
	}
	mutex_unlock(&it6151_i2c_access);

	return ret;
}


static int it6151_i2c_probe(struct i2c_client * client, const struct i2c_device_id * dev_id)
{
	int ret;
	
	struct it6151_info *info = kmalloc(sizeof(struct it6151_info), GFP_KERNEL);
	if(IS_ERR(info)) {
		usr_msg("error: devm_kmalloc");
		return -ENOMEM;
	}
	usr_msg("---> moved in function %s", __func__);
	mutex_init(&it6151_i2c_access);
	
	// if(i2c_match_id(&it6151_id_table[0], client)){
	if(strcmp(client->name, it6151_id_table[0].name) == 0) {
		usr_msg("--->in function: %s, client->name = %s", __func__, client->name);
		if (!(it6151_mipirx = (struct i2c_client *)kmalloc(sizeof(struct i2c_client), GFP_KERNEL))) {
			ret = -ENOMEM;
			goto exit;
		}
		memset(it6151_mipirx, 0, sizeof(struct i2c_client));

		it6151_mipirx = client;    
	} 
	//else if(i2c_match_id(&it6151_id_table[1], client)){
	else if(strcmp(client->name, it6151_id_table[1].name) == 0) {
		usr_msg("--->in function: %s, client->name = %s", __func__, client->name);
		if (!(it6151_edp = (struct i2c_client *)kmalloc(sizeof(struct i2c_client), GFP_KERNEL))) {
			ret = -ENOMEM;
			goto exit;
		}
		memset(it6151_edp, 0, sizeof(struct i2c_client));
	
		it6151_edp = client;
		
		i2c_set_clientdata(client, (void *)info);
		ret = usr_get_dts_info(info, client);
		if(ret < 0) {
			usr_msg("in function: %s, usr_get_dts_info failed", __func__);
			goto exit;
		}
		it6151_power_on(info);
		// it6151_create_chip_init_wq(info);
		// it6151_i2c_init_flag = 1;

	} else {
		usr_msg("---> moved in function %s client name match failed ret = %d", __func__, ret);
		ret = -EIO;
		goto exit;
	}
	usr_msg("---> moved out function %s", __func__);

	
	return 0;
	
exit:
	kfree(info);
	return ret;	
}

static int it6151_i2c_remove(struct i2c_client * client)
{
	usr_msg("moved in function: %s", __func__);
	return 0;
}

static const struct of_device_id it6151_match_table[] = {
	{.compatible = "it6151_mipirx"},
	{.compatible = "it6151_edp"},
	/** keep this */
};
MODULE_DEVICE_TABLE(i2c, it6151_match_table);


static struct i2c_driver it6151_driver = {
	.probe = it6151_i2c_probe,
	.remove = it6151_i2c_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = DEVICE_NAME,
		.of_match_table = of_match_ptr(it6151_match_table),
	},
	.id_table = it6151_id_table,
};

module_i2c_driver(it6151_driver);

MODULE_AUTHOR("V <machester@aliyun.com>");
MODULE_DESCRIPTION("DRM Driver for mipi to edp transder chip it6151");
MODULE_LICENSE("GPL");














