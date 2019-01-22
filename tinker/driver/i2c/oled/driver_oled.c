/* oled whole information struction ---------------------------------------*/

#include "driver_oled.h"


#define     OLED_DRIVER_VERSION         "0.1"

static int oled_open(struct inode *inode, struct file *filp);
static int oled_close(struct inode *inode, struct file *filp);
static ssize_t oled_write(struct file *flip, const char __user *buff,
                                    size_t counter, loff_t *fops);
static long oled_ioctl (struct file *flip, unsigned int cmd, 
                            unsigned long param);

static struct file_operations fops = {
    .open           = oled_open,
    .release        = oled_close,
    .write          = oled_write,
    .unlocked_ioctl = oled_ioctl,
};


#if 0
struct bus_type oled_bus_type = {

};

struct notifier_block oled_notifier_block = {
    .notifier_call =
};
#endif


int gpio_staus_set(status state)
{
	int ret = 0;
    mutex_lock(&oled_dev_info->oled_dev_lock);
    if(low == state) {
        ret = pinctrl_select_state(oled_dts_info->dev_pinctrl,oled_dts_info->rst_low);
		if(ret < 0) {
			err_msg("error : set oled_dts_info->rst_low");
			ret = -EIO;
		}
		
    } else {
        ret = pinctrl_select_state(oled_dts_info->dev_pinctrl,oled_dts_info->rst_high);
		if(ret < 0) {
			err_msg("error : set oled_dts_info->rst_high");
			ret = -EIO;
		}
    }
    mutex_unlock(&oled_dev_info->oled_dev_lock);
	return ret;
}

static int oled_i2c_probe(struct i2c_client * client, const struct i2c_device_id *i2c_id)
{
    int ret = 0;


    usr_msg("---> move in");
    if(!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		err_msg("i2c check failed");
		return -ENODEV;
    }

    dev_info(&client->dev, "chip found, driver verison" OLED_DRIVER_VERSION "\n");

	ret = struct_mallock_init();
	if(ret < 0) {
		err_msg("struct malloc error");
		return -ENOMEM;
	}

	oled_i2c_info->oled_client = client;
    mutex_init(&oled_i2c_info->oled_i2c_lock);


    // oled_init();
    // oled_clear();
    // oled_show_string(6, 1, "Bootting...", 16);

    return 0;


#if 0
    /* Keep track of adapters which will be added or removed later */
	res = bus_register_notifier(&i2c_bus_type, &i2cdev_notifier);
	if (res)
		goto out_unreg_class;
#endif
}

static int oled_i2c_remove(struct i2c_client *client)
{
    return 0;
}



static struct of_device_id oled_i2c_table[] = {
	{.compatible = "oled, ssd1306",},
	{/* keep this */}
};
// MODULE_DEVICE_TABLE(i2c, of_match_ptr(oled_i2c_table));

static struct i2c_driver oled_i2c_driver = {
    .driver = {
        .owner = THIS_MODULE,
        .name = OLED_DEV_NAME,
        .of_match_table = of_match_ptr(oled_i2c_table),
    },
    .probe = oled_i2c_probe,
    .remove = oled_i2c_remove,
};

void add_oled_i2c_driver(void)
{
    i2c_add_driver(&oled_i2c_driver);
}


static int oled_open(struct inode *inode, struct file *filp)
{
    usr_msg( "open");
    return 0;
}

static int oled_close(struct inode *inode, struct file *filp)
{
    usr_msg( "close");
    return 0;
}

static ssize_t oled_write(struct file *flip, const char __user *buff,
                         size_t counter, loff_t *fops)
{
    usr_msg( "write");
    return 0;
}

static ssize_t oled_read (struct file *flip, char __user *buff,
                            size_t counter, loff_t *fops)
{
    usr_msg( "read");
    return 0;
}
static long oled_ioctl (struct file *flip, unsigned int cmd, unsigned long param)
{
    usr_msg( "ioctl");
    return 0;
}




int struct_mallock_init(void)
{
	int ret = 0;

	usr_msg("struct malloc and initilaziton.");
	oled_dts_info = kmalloc(sizeof(struct _oled_dts_info), GFP_KERNEL);
	if(IS_ERR(oled_dts_info)) {
		err_msg("malloc struct < oled_dts_info > error");
		ret = -ENOMEM;
		return ret;
	}
	oled_dev_info = kmalloc(sizeof(struct _oled_device_info), GFP_KERNEL);
	if(IS_ERR(oled_dev_info)) {
		err_msg("malloc struct < oled_dev_info > error");
		ret = -ENOMEM;
		goto err_oled_dev_info;
	}
	oled_i2c_info = kmalloc(sizeof(struct _oled_i2c_info), GFP_KERNEL);
	if(IS_ERR(oled_i2c_info)) {
		err_msg("malloc struct < oled_i2c_info > error");
		ret = -ENOMEM;
		goto err_oled_i2c_info;
	}
    mutex_init(&oled_dev_info->oled_dev_lock);
    
	return ret;

err_oled_i2c_info:
	kfree(oled_dev_info);
err_oled_dev_info:
	kfree(oled_dts_info);

	return ret;

}

int register_oled_driver(void)
{
	int ret = 0;
	ret = alloc_chrdev_region(&oled_dev_info->oled_devno , 0, 1, OLED_DEV_NAME);
	if(ret < 0) {
		err_msg("error : alloc_chrdev_region");
		ret = -ENODEV;
		goto out;
	}
	oled_dev_info->oled_major = MAJOR(oled_dev_info->oled_devno);
	usr_msg("---> dev_info->oled_major = %d", oled_dev_info->oled_major);
#if 0
	oled_dev_info->oled_cdev = cdev_alloc();
	if(IS_ERR(oled_dev_info->oled_cdev)) {
		err_msg("error : cdev_alloc");
		goto err_cdev_alloc;
	}
#endif
	cdev_init(&oled_dev_info->oled_cdev, &fops);
	ret = cdev_add(&oled_dev_info->oled_cdev , oled_dev_info->oled_devno, 1);
	if(ret < 0) {
		err_msg("error : cdev_add.");
		goto err_cdev_add;
	}

	oled_dev_info->oled_class = class_create(THIS_MODULE, OLED_DEV_NAME);
    if (IS_ERR(oled_dev_info->oled_class)) {
        err_msg("error : class create.");
        ret = PTR_ERR(oled_dev_info->oled_class);
        goto err_cls_create;
    }
    oled_dev_info->oled_device = device_create(oled_dev_info->oled_class, NULL,
                                                oled_dev_info->oled_devno,
                                                NULL, "%s", OLED_DEV_NAME);
    if(IS_ERR(oled_dev_info->oled_device)) {
        err_msg("error : device_create.");
        ret = PTR_ERR(oled_dev_info->oled_device);
        goto err_dev_create;
    }
    return ret;

err_dev_create:
    class_destroy(oled_dev_info->oled_class);
err_cls_create:
	cdev_del(&oled_dev_info->oled_cdev);
err_cdev_add:
	unregister_chrdev_region(oled_dev_info->oled_devno, 1);
out:
	return ret;
}


static int oled_probe(struct platform_device * pdev)
{
	int ret;
    int index;

	usr_msg("---> moved in");

	ret = struct_mallock_init();
	if(ret < 0) {
		err_msg("error : malloc.");
		return ret;
	}
	ret = oled_get_dts_info(pdev);
	if(ret < 0) {
		err_msg("error : get dts information.");
		return ret;
	}
	ret = register_oled_driver();
	if(ret < 0) {
		err_msg("error : oled driver register.");
		return ret;
	}
    
    for(index = 0; index < 5; index++) {
        gpio_staus_set(high);
        mdelay(1000);
        gpio_staus_set(low);
        mdelay(1000);
    }
    add_oled_i2c_driver();

    return 0;
}

static int oled_remove(struct platform_device * pdev)
{
    i2c_del_driver(&oled_i2c_driver);
    
    device_destroy(oled_dev_info->oled_class, oled_dev_info->oled_devno);
    class_destroy(oled_dev_info->oled_class);
    cdev_del(&oled_dev_info->oled_cdev);
    unregister_chrdev(oled_dev_info->oled_major, OLED_DEV_NAME);
//    pinctrl_free(oled_dts_info->rst_high, false);
//    pinctrl_free(oled_dts_info->rst_low, false);
//    pinctrl_free(oled_dts_info->dev_pinctrl, false);
    kfree(oled_dev_info);
    kfree(oled_dts_info);
    kfree(oled_i2c_info);

	return 0;
}

static struct of_device_id oled_table[] = {
	{.compatible = "oled-pins",},
	{/* keep this */}
};
    
// MODULE_DEVICE_TABLE(oled, of_match_ptr(oled_table));

static struct platform_driver oled_driver = {
	.probe = oled_probe,
	.remove = oled_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = OLED_DEV_NAME,
		.of_match_table = of_match_ptr(oled_table),
	},
};
        
int oled_get_dts_info(struct platform_device *pdev)
{
    int ret = 0;

    mutex_lock(&oled_dev_info->oled_dev_lock);
    
    oled_dts_info->node = of_find_matching_node(NULL, oled_table);
    if(!oled_dts_info->node) {
        err_msg("oled_dts_info kmalloc error.");
        ret =  -ENODEV;
        goto err_no_dev;
    } else {
        // get dts information
        of_property_read_u32(oled_dts_info->node, "width", &oled_dts_info->oled_width);
        usr_msg("---> width = %d", oled_dts_info->oled_width);
        of_property_read_u32(oled_dts_info->node, "height", &oled_dts_info->oled_height);
        usr_msg("---> height = %d", oled_dts_info->oled_height);

        // get dts gpio configurations
        oled_dts_info->dev_pinctrl = devm_pinctrl_get(&pdev->dev);
        if(IS_ERR(oled_dts_info->dev_pinctrl)) {
            err_msg("error : oled_dts_info->dev_pinctrl");
            ret = PTR_ERR(oled_dts_info->dev_pinctrl);
            goto err_no_dev;
        }
        oled_dts_info->rst_high = pinctrl_lookup_state(oled_dts_info->dev_pinctrl, "oled_rst_set_high");
        if(IS_ERR(oled_dts_info->dev_pinctrl)) {
            err_msg("error : oled_dts_info->rst_high");
            ret = PTR_ERR(oled_dts_info->rst_high);
            goto err_no_dev;
        }
        oled_dts_info->rst_low = pinctrl_lookup_state(oled_dts_info->dev_pinctrl, "oled_rst_set_low");
        if(IS_ERR(oled_dts_info->dev_pinctrl)) {
            err_msg("error : oled_dts_info->rst_low");
            ret = PTR_ERR(oled_dts_info->rst_low);
            goto err_no_dev;
        }
        // set default state
        pinctrl_select_state(oled_dts_info->dev_pinctrl,oled_dts_info->rst_high);
    }
    mutex_unlock(&oled_dev_info->oled_dev_lock);

err_no_dev:
    kfree(oled_dts_info);
    return ret;
}

EXPORT_SYMBOL(oled_get_dts_info);

module_platform_driver(oled_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("quan, <shusheng1991@gmail.com>");
MODULE_ALIAS("i2c :" OLED_DEV_NAME);
MODULE_DESCRIPTION("OLED Driver, control chip SSD1306");
MODULE_VERSION(OLED_DRIVER_VERSION);
