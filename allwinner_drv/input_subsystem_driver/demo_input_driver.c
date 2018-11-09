/*----------------------------------------------------------------------*
 * Data : 2018-10-xx
 * input subsystem
 *
 *----------------------------------------------------------------------*/
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

#include <linux/gpio_keys.h>
#include "usr_key.h"


/*--------------------- macro defined start --------------------------*/
#define usr_msg(args)                                           \
            do{                                                 \
                    printk(KERN_ERR "---> %s\n", args);         \
            }while(0)

#define err_msg(args)                                           \
            do{                                                 \
                    printk(KERN_ERR "-------> %s\n", args);     \
            }while(0)
/*--------------------------- macro defined end ---------------------------*/



#define     DEMO_PLT_DRV_NAME           "demo_input_plt"
#define     USR_DEV_NAME                "demo_input_dev"

int usr_get_gpio_info(struct platform_device *pdev);
/*---------------------------------------------------------------------*/

int usr_get_gpio_info(struct platform_device *pdev)
{
    of_machine_is_compatible("");
#if 0
	int ret;

	TPD_DEBUG("[tpd %d] mt_tpd_pinctrl+++++++++++++++++\n", pdev->id);
	pinctrl1 = devm_pinctrl_get(&pdev->dev);
	if (IS_ERR(pinctrl1)) {
		ret = PTR_ERR(pinctrl1);
		dev_err(&pdev->dev, "fwq Cannot find touch pinctrl1!\n");
		return ret;
	}
	pins_default = pinctrl_lookup_state(pinctrl1, "default");
	if (IS_ERR(pins_default)) {
		ret = PTR_ERR(pins_default);
		/* dev_err(&pdev->dev, "fwq Cannot find touch pinctrl default %d!\n", ret);*/
	}
	eint_as_int = pinctrl_lookup_state(pinctrl1, "state_eint_as_int");
	if (IS_ERR(eint_as_int)) {
		ret = PTR_ERR(eint_as_int);
		dev_err(&pdev->dev, "fwq Cannot find touch pinctrl state_eint_as_int!\n");
		return ret;
	}
	eint_output0 = pinctrl_lookup_state(pinctrl1, "state_eint_output0");
	if (IS_ERR(eint_output0)) {
		ret = PTR_ERR(eint_output0);
		dev_err(&pdev->dev, "fwq Cannot find touch pinctrl state_eint_output0!\n");
		return ret;
	}
	eint_output1 = pinctrl_lookup_state(pinctrl1, "state_eint_output1");
	if (IS_ERR(eint_output1)) {
		ret = PTR_ERR(eint_output1);
		dev_err(&pdev->dev, "fwq Cannot find touch pinctrl state_eint_output1!\n");
		return ret;
	}
	rst_output0 = pinctrl_lookup_state(pinctrl1, "state_rst_output0");
	if (IS_ERR(rst_output0)) {
		ret = PTR_ERR(rst_output0);
		dev_err(&pdev->dev, "fwq Cannot find touch pinctrl state_rst_output0!\n");
		return ret;
	}
	rst_output1 = pinctrl_lookup_state(pinctrl1, "state_rst_output1");
	if (IS_ERR(rst_output1)) {
		ret = PTR_ERR(rst_output1);
		dev_err(&pdev->dev, "fwq Cannot find touch pinctrl state_rst_output1!\n");
		return ret;
	}
	TPD_DEBUG("[tpd%d] mt_tpd_pinctrl----------\n", pdev->id);
	return 0;
#endif







}

struct input_dev * usr_dev;
static int irq_no;
static int pd_probe(struct platform_device * pd_dev)
{
    int ret;
    struct device * dev = &pd_dev->dev;
    
    usr_msg("allocate input device.");
    usr_dev = input_allocate_device();
    if(!usr_dev) {
        err_msg("input device allocate error.");
        ret = -ENOMEM;
        goto err_allocate;
    }

    /*---------- set input device param start -------------*/
    
    /*---------- set input device param end ---------------*/

  




    ret = input_register_device(struct input_dev *);
    if(0 != ret) {
        goto err_register_dev;
    }

    err_register_dev:
        
    err_request_irq:
        input_free_device(usr_dev);
    err_allocate:
        return ret;
}

static int pd_remove(struct platform_device * pd_dev)
{
    usr_msg("remove input device");    
    input_unregister_device(usr_dev);
    input_free_device(usr_dev);
}

static const struct of_device_id input_dev_match_table[] = {
    {.compatible = "nano, laser-pin",},
    {.compatible = "nano, laserflash-pin",},
    {},
};
MODULE_DEVICE_TABLE(of, input_dev_match_table);

static struct platform_driver pd_drv = {
    .driver = {
        .owner = THIS_MODULE,
        .name  = DEMO_PLT_DRV_NAME,
        .of_match_table = input_dev_match_table,
     },
    .probe = pd_probe,
    .remove = pd_remove,
};

module_platform_driver(&pd_drv);


MODULE_AUTHOR("QUAN");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("device tree input subsystem");


