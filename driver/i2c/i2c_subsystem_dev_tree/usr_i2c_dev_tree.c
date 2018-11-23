/*-----------------------------------------------------
* Description :
* I2c adapter use description :
* Purpose :
* Author :
* Date :
* Version :
------------------------------------------------------*/
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/printk.h>
#include <linux/major.h>
#include <linux/device.h>
#include <linux/platform_device.h>

#include <drivers/i2c-core.h>
#include <linux/mod_devicetable.h>
#include </linux/i2c.h>

#define I2C_DEVICE 		"demo_i2c_dev"
#define USR_DEVICE      "demo_usr_dev"



/**
struct i2c_device_id {
	char name[I2C_NAME_SIZE];
	kernel_ulong_t driver_data;
};
**/

/**
msg.len is a u16. This means you must always be less than 216 (64k) with
your read/write buffers.
**/
struct i2c_msg {
};

struct i2c_client *usr_client;



int (*probe)(struct i2c_client *, const struct i2c_device_id *);
int (*remove)(struct i2c_client *);
/* Device detection callback for automatic device creation */
int (*detect)(struct i2c_client *, struct i2c_board_info *);
/* a ioctl like command that can be used to perform specific functions
 * with the device.
 */
int (*command)(struct i2c_client *client, unsigned int cmd, void *arg);



static int usr_i2c_probe(struct i2c_client *client, const struct i2c_device_id
                         *id)
{
	const struct of_device_id *match;
	match = of_match_device(mcp23s08_i2c_of_match, &client->dev);
	if (match) {
		/* Device tree code goes here */
	} else {
	    usr_client = kmalloc(sizeof(i2c_client), GFP_KERNEL);
		pdata = dev_get_platdata(&usr_client->dev);
		/*
		* Platform data code comes here.
		* One can use
		* pdata = dev_get_platdata(&client->dev);
		*
		* or *id*, which is a pointer on the *i2c_device_id* entry that
		originated
		* the match, in order to use *id->driver_data* to extract the
		device
		* specific data, as described in platform driver chapter.
		*/
	}

	static struct i2c_device_id i2c_match_table[] = {
		{ "demo_usr_i2c",},
		{ }
	};
	MODULE_DEVICE_TABLE(i2c, i2c_match_table);

static const struct i2c_device_id usr_i2c_id[] = {{USR_DEVICE, 0}, {}};
	static struct i2c_driver usr_i2c_driver = {
		.driver = {
			.name           = I2C_DEVICE,
			.owner          = THIS_MODULE,
			.of_match_table = i2c_match_table,
		},
		.probe    = usr_i2c_probe,
		.remove   = usr_i2c_remove,
		.id_table = usr_i2c_id,
		.detect   = usr_i2c_detect,
	};


}





struct i2c_client {
};

struct i2c_driver {
};

struct i2c_adapter {
};


module_i2c_driver();


