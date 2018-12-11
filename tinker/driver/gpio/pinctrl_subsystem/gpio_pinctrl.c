/**
 * 
 */

#include "./gpio_pinctrl.h"



static int laser_open(struct inode *inode, struct file *filp)
{
    usr_msg("device open and init");

    return 0;
}

static int laser_release(struct inode *inode, struct file *filp)
{
    usr_msg("device close and restore the device default set");
    // device restore code
    return 0;
}

static ssize_t laser_write(struct file *flip, const char __user *buff,
                         size_t counter, loff_t *fops)
{
    int value, ret, index;

    usr_msg("device write");
    if (counter < 0 || counter > 4) return -EINVAL;

    ret = copy_from_user(&value, buff, counter);
    if (ret > 0) {
        err_msg("copy_from_user error");
        return -EFAULT;
    }
    printk(KERN_ERR "--------> value is  %d\n", value);
    usr_msg("start to switch copied data.");
    switch (value) {
        case 0:
            usr_msg("value is : 0");
            gpio_set_value(laser_pin, on);
            msleep(1000);
            break;
        case 1:
            usr_msg("value is : 1");
            gpio_set_value(laser_pin, off);
            msleep(1000);
            break;
        case 2:
            usr_msg("value is : 2");
            for(index = 0; index < 3; index++) {
                gpio_set_value(laser_pin, off);
                msleep(1000);
                gpio_set_value(laser_pin, on);
                msleep(1000);
            }
            break;
        default:
            usr_msg("Out of handle range");
    }
    usr_msg("device write finished.");
    // return data has been copied
    return counter;
}
static long laser_ioctl(struct file *flip, unsigned int cmd,
                      unsigned long param)
{
    usr_msg("device dev_ioctl");
    int delay_timer, ret, temp, gpio_status;


        switch(cmd)
    {
        case GPIO_TEST :
            printk("in key_drv_ioctl : test case\n");
            break;

        case GPIO_SET:
            break;
        case GPIO_RESET:
            break;
        case GPIO_BLINK:
            temp = (int)param;
            ret = copy_from_user(&delay_timer, param, sizeof(int));
            if (ret != 0) {
                err_msg("copy_from_user error");
                return -EFAULT;
            }
            /*TODO : delay function*/
            break;
        case GPIO_STATUS:
            gpio_status = -1;
            /*TODO : read gpio status*/
            ret = copy_to_user(flip, gpio_status, sizeof(int));
            if (ret != 0) {
                err_msg("copy_from_user error");
                return -EFAULT;
            }
            break;
        default:
            usr_msg("unkown cmd");
            return -ENOTTY;
            break;
    }
    return 0;
}
static int get_dts_info(struct device * laser_dev)
{
    int ret;
    ret = 0;
    
    pin_info->laser_pinctrl = devm_pinctrl_get(laser_dev->dev);
    if(IS_ERR(pin_info->laser_pinctrl)) {
        err_msg("dev_pinctrl_get error [%d]\n", __LINE__);
        return PTR_ERR(pin_info->laser_pinctrl);
    }
    
    pin_info->laser_state_on = pinctrl_lookup_state(pin_info->laser_pinctrl, "laser_on");
    if(IS_ERR(pin_info->laser_state_on)) {
        err_msg("pinctrl_lookup_state error [%d]\n", __LINE__);
        devm_pinctrl_put(pin_info->laser_pinctrl);
        return ERR_PTR(PTR_ERR(pin_info->laser_state_on)); 
    }
    
    pin_info->laser_state_off = pinctrl_lookup_state(pin_info->laser_pinctrl, "laser_off");
    if(IS_ERR(pin_info->laser_state_off)) {
        err_msg("pinctrl_lookup_state error [%d]\n", __LINE__);
        devm_pinctrl_put(pin_info->laser_pinctrl);
        return ERR_PTR(PTR_ERR(pin_info->laser_state_off)); 
    }
    
    // lock protect
    mutex_lock(&dev_info->laser_mutex);
    ret = pinctrl_select_state(pin_info->laser_pinctrl, pin_info->laser_state_on);
    if(ret < 0) {
        devm_pinctrl_put(pin_info->laser_pinctrl);
        err_msg("pinctrl_select_state error [%d]\n", __LINE__);
        return ERR_PTR(ret);
    }
    mutex_unlock(&dev_info->laser_mutex);
    
    return ret;
}

static const struct file_operations dev_fops = {
    .write          = laser_write,
    .open           = laser_open,
    .release        = laser_release,
    .unlocked_ioctl = laser_ioctl,
};

/**
 * @Descripthon : laser_probe
 * 		using gpio subsystem to request gpio and get the gpio status
 */

static int laser_probe(struct platform_device *pdev)
{
    int index, ret;
    ret = 0;
    
    printk(KERN_ERR "---> (%s) [%d] \n", __func__, __LINE__);

    dev_info = kmalloc(sizeof(struct device_info), GFP_KERNEL);
    if (!dev_info) {
    err_msg("dev_info kmalloc error\n");
    return -ENOMEM;
    }
    memset(dev_info, 0, sizeof(struct device_info));   
     
    mutex_init(&dev_info->laser_mutex);
    get_dts_info(pdev->dev);
    
    

    
    
// // platform match check
//     struct of_device_id * match;
//     match = of_match_node();
//     // -----------------> gpio get start
//     pin_info = kmalloc(sizeof(pinctrl_info), GFP_KERNEL);
//     if(!pin_info) {
//         err_msg("pinctrl_info error\n");
//         return -ENOMEM;
//     }
//     memset(pin_info, 0, sizeof(pinctrl_info));
    
//     // -----------> lock area start
//     mutex_lock(&dev_info->laser_mutex);
    
//     // -----------> lock area end
//     mutex_unlock(&dev_info->laser_mutex);
//     usr_msg("pinctrl state setted\n");
//     //  ----------------->gpio get end











#ifdef GPIO_DEVICE_TREE
    // -----------------> gpio get start
    struct device_node *laser_node = pdev->dev.of_node;
    if (!laser_node) {
        err_msg("device node get error\n");
        return -ENOENT;
    }

    struct of_device_id * match;
    match = of_match_device(of_match_ptr(laser_table), pdev->dev);
    if(!match) {
        printk(KERN_ERR "--------> of_match_device failed\n");
        return -ENODEV;
    }
    /**
     * of_get_named_gpio 的name 为  laser 节点下 的 laser-on = <&gpio5 RK_PC3 GPIO_ACTIVE_HIGH>;
     */
    laser_pin = of_get_named_gpio(laser_node, "laser-on", 0);
    if (!gpio_is_valid(laser_pin)) {
        printk(KERN_ERR "--------> gpio  %d is not valid\n", laser_pin);
        return -EIO;
    }
    //两种写法都可以
    //ret = devm_gpio_request_one(&pdev->dev ,laser_pin, GPIOF_OUT_INIT_LOW, "laser_pin");
    ret = gpio_request(laser_pin, LASER_NAME);
    if(ret < 0) {
        printk(KERN_ERR "--------> gpio  %d request error\n", laser_pin);
        gpio_free(laser_pin);
        return -EBUSY;
    }

    gpio_direction_output(laser_pin, 0);
    gpio_set_value(laser_pin, 1);
    usr_msg("gpio request and set ok");
    //  ----------------->gpio get end


    usr_msg("start to create char device");
    dev_info->dev_no = MKDEV(dev_info->dev_major, 0);
    if(dev_info->dev_major) {
        register_chrdev_region(dev_info->dev_no, 1, LASER_NAME);
    } else {
        ret = alloc_chrdev_region(&dev_info->dev_no, 0, 1, LASER_NAME);
        if(ret < 0) {
            err_msg("alloc_chrdev_region error");
            goto err_out;
        }
    }

    // store node_major to device info structure
    dev_info->dev_major = MAJOR(dev_info->dev_no);
    dev_info->dev_no = MKDEV(dev_info->dev_major, 0);
    printk(KERN_ERR "(created device number is %d) [%d]\n", dev_info->dev_no, __LINE__);

    //-------- set device info, fill struct cdev ------
    cdev_init(&dev_info->dev, &dev_fops);
    // cdev_init has memset(cdev, 0, sizeof *cdev);
    dev_info->dev.owner = THIS_MODULE;

    ret = cdev_add(&dev_info->dev, dev_info->dev_no, 1);
    if (ret) {
        err_msg("cdev add error");
        ret = -ENOMEM;
        goto err_cdev_add;
    }
    usr_msg("ready to create device class.");
    dev_info->dev_class = class_create(THIS_MODULE, LASER_CLS_NAME);
    if (IS_ERR(dev_info->dev_class)) {
        err_msg("class create error");
        ret = PTR_ERR(dev_info->dev_class);
        goto err_class_create;
    }

    printk(KERN_ERR "(ready to create device in path /dev/%s) [%d]\n", LASER_NAME, __LINE__);
    dev_info->dev_device = device_create(dev_info->dev_class, NULL,
                        dev_info->dev_no, NULL, "%s", LASER_NAME);
    if (IS_ERR(dev_info->dev_device)) {
        err_msg("device create error");
        ret = PTR_ERR(dev_info->dev_device);
        goto err_device_create;
    }
    usr_msg("module has been created.");

  for (index = 0; index < 5; index++) {
        printk(KERN_ERR "---------> loop = %d\n", index);
        gpio_set_value(laser_pin, on);
        msleep(2000);
        gpio_set_value(laser_pin, off);
        msleep(2000);
    }

  return 0;

err_device_create:
    class_destroy(dev_info->dev_class);
err_class_create:
    cdev_del(&dev_info->dev);
err_cdev_add:
    unregister_chrdev_region(dev_info->dev_no, 1);
err_out:
    gpio_free(laser_pin);
    kfree(dev_info);
    
#endif
    return ret;
}
static int laser_remove(struct platform_device *pdev)
{
    gpio_free(laser_pin);
    device_destroy(dev_info->dev_class, dev_info->dev_no);
    class_destroy(dev_info->dev_class);
    cdev_del(&dev_info->dev);
    err_msg("laser_remove");
    return 0;
}

static struct of_device_id laser_table[] = {
    {
        .compatible = "laser-leds",
    },
    {/*sentinel*/},
};
MODULE_DEVICE_TABLE(of, laser_table);

static struct platform_driver laser_driver = {
    .probe = laser_probe,
    .remove = laser_remove,
    .driver =
        {
            .name = DRIVER_NAME,
            .owner = THIS_MODULE,
            .of_match_table = of_match_ptr(laser_table),
        },
};

static int __init gpio_init(void)
{
    int ret;
    printk(KERN_INFO "---------> move in function (%s)\n", __func__);
    ret = platform_driver_register(&laser_driver);
    if (ret < 0) {
        printk(KERN_ERR "---------> platform_driver_register error\n");
    }
    return 0;
}

static void __exit gpio_exit(void)
{
    printk(KERN_INFO "---------> move in function : %s", __func__);
    platform_driver_unregister(&laser_driver);
}

module_init(gpio_init);
module_exit(gpio_exit);

MODULE_AUTHOR("QUAN");
MODULE_LICENSE("GPL");
MODULE_ALIAS("laser driver");
MODULE_DESCRIPTION("gpio platform driver");
