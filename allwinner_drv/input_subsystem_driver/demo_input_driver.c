/*----------------------------------------------------------------------*
 * Data : 2018-10-xx
 * using struct cdev to register char device
 * creat class for device
 * fill read, write, ioctl, close
 *
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
/*-----------------------------------------------
* register input device
* 
struct input_dev {
	const char *name;
	const char *phys;
	const char *uniq;
	struct input_id id;

	unsigned long propbit[BITS_TO_LONGS(INPUT_PROP_CNT)];

	unsigned long evbit[BITS_TO_LONGS(EV_CNT)];
	unsigned long keybit[BITS_TO_LONGS(KEY_CNT)];
	unsigned long relbit[BITS_TO_LONGS(REL_CNT)];
	unsigned long absbit[BITS_TO_LONGS(ABS_CNT)];
	unsigned long mscbit[BITS_TO_LONGS(MSC_CNT)];
	unsigned long ledbit[BITS_TO_LONGS(LED_CNT)];
	unsigned long sndbit[BITS_TO_LONGS(SND_CNT)];
	unsigned long ffbit[BITS_TO_LONGS(FF_CNT)];
	unsigned long swbit[BITS_TO_LONGS(SW_CNT)];

	unsigned int hint_events_per_packet;

	unsigned int keycodemax;
	unsigned int keycodesize;
	void *keycode;

	int (*setkeycode)(struct input_dev *dev,
			  const struct input_keymap_entry *ke,
			  unsigned int *old_keycode);
	int (*getkeycode)(struct input_dev *dev,
			  struct input_keymap_entry *ke);

	struct ff_device *ff;

	unsigned int repeat_key;
	struct timer_list timer;

	int rep[REP_CNT];

	struct input_mt *mt;

	struct input_absinfo *absinfo;

	unsigned long key[BITS_TO_LONGS(KEY_CNT)];
	unsigned long led[BITS_TO_LONGS(LED_CNT)];
	unsigned long snd[BITS_TO_LONGS(SND_CNT)];
	unsigned long sw[BITS_TO_LONGS(SW_CNT)];

	int (*open)(struct input_dev *dev);
	void (*close)(struct input_dev *dev);
	int (*flush)(struct input_dev *dev, struct file *file);
	int (*event)(struct input_dev *dev, unsigned int type, unsigned int code, int value);

	struct input_handle __rcu *grab;

	spinlock_t event_lock;
	struct mutex mutex;

	unsigned int users;
	bool going_away;

	struct device dev;

	struct list_head	h_list;
	struct list_head	node;

	unsigned int num_vals;
	unsigned int max_vals;
	struct input_value *vals;

	bool devres_managed;
};
------------------------------------------------*/
// need change to fix allwinner chip platform
int usr_get_gpio_info(struct platform_device *pdev)
{
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
}
/*------------------------------------------------------------------------

-------------------------------------------------------------------------*/
#include <linux/gpio_keys.h>

struct input_dev * usr_dev;
static int irq_no;
static int plt_probe(struct platform_device * plt_dev)
{
    int ret;
    struct device * dev = &plt_dev->dev;
    usr_msg("allocate input device.");
    usr_dev = input_allocate_device();
    if(!usr_dev) {
        err_msg("input device allocate error.");
        ret = -ENOMEM;
        goto err_allocate;
    }
    //confirm device tree is enable and build in kernel
    // hardware init and requester irq
    usr_get_gpio_info(plt_dev);
        
    ret = request_irq(irqno, input_key_irq, IRQF_TRIGGER_FALLING|IRQF_TRIGGER_RISING,
						"key1-eint1", NULL);
    if(0 != ret) {
        err_msg("request irq error.")
        goto err_request_irq;
    }
    
    #if 0
    // no use
	irqno = gpio_to_irq(S5PV210_GPH0(1));
	ret = request_irq(irqno, input_key_irq, IRQF_TRIGGER_FALLING|IRQF_TRIGGER_RISING,
						"key1-eint1", NULL);
    irq_no = request_irq();
    #endif

    int __must_check input_register_device(struct input_dev *);
    err_request_irq:
        input_free_device(usr_dev);
    err_allocate:
        return ret;


gpiod_get_value();
gpiod_set_value();

#if 0
1static int gpio_keys_probe(struct platform_device *pdev)
2{
3 struct device *dev = &pdev->dev;
4 const struct gpio_keys_platform_data *pdata = dev_get_platdata(dev);
5 struct gpio_keys_drvdata *ddata;
6 struct input_dev *input;
7 size_t size;
8 int i, error;
9 int wakeup = 0;
10
11 if (!pdata) {
12 pdata = gpio_keys_get_devtree_pdata(dev);
13 if (IS_ERR(pdata))
14 return PTR_ERR(pdata);
15 }
16
17 size = sizeof(struct gpio_keys_drvdata) +
18 pdata->nbuttons * sizeof(struct gpio_button_data);
19 ddata = devm_kzalloc(dev, size, GFP_KERNEL);
20 if (!ddata) {
21 dev_err(dev, "failed to allocate state\n");
22 return -ENOMEM;
23 }
24
25 input = devm_input_allocate_device(dev);
26 if (!input) {
27 dev_err(dev, "failed to allocate input device\n");
28 return -ENOMEM;
29 }
30
31 ddata->pdata = pdata;
32 ddata->input = input;
33 mutex_init(&ddata->disable_lock);
34
35 platform_set_drvdata(pdev, ddata);
36 input_set_drvdata(input, ddata);
37
38 input->name = pdata->name : pdev->name;
39 input->phys = "gpio-keys/input0";
40 input->dev.parent = &pdev->dev;
41 input->open = gpio_keys_open;
42 input->close = gpio_keys_close;
43
44 input->id.bustype = BUS_HOST;
45 input->id.vendor = 0x0001;
46 input->id.product = 0x0001;
47 input->id.version = 0x0100;
48
49 /* Enable auto repeat feature of Linux input subsystem */
50 if (pdata->rep)
51 __set_bit(EV_REP, input->evbit);
52
53 for (i = 0; i < pdata->nbuttons; i++) {
54 const struct gpio_keys_button *button = &pdata->buttons[i];
55 struct gpio_button_data *bdata = &ddata->data[i];
56
57 error = gpio_keys_setup_key(pdev, input, bdata, button);
58 if (error)
59 return error;
60
61 if (button->wakeup)
62 wakeup = 1;
63 }
64
65 error = sysfs_create_group(&pdev->dev.kobj, &gpio_keys_attr_group);
66 ...
67 error = input_register_device(input);

#endif

}

static int plt_remove(struct platform_device * plt_dev)
{
    input_free_device(usr_dev);
    input_unregister_device(usr_dev);
}

static const struct of_device_id input_dev_match_table[] = {
    {.compatible = "usr, usr_input_device",},
    {.compatible = "usr, usr_key-device",},
    {},
};

#if 1
static struct platform_driver plt_drv = {
    .driver = {
        .owner = THIS_MODULE,
        .name  = DEMO_PLT_DRV_NAME,
        .of_match_table = &input_dev_match_table,
     }
    .probe = plt_probe,
    .remove = plt_remove,
};
#endif
#if 0
static struct platform_driver *plt_drv;
#endif
static int __init demo_input_system_init(void)
{
    int ret;
    #if 0
    plt_drv = platform_device_alloc(DEMO_PLT_DRV_NAME, -1);
    #endif 
    usr_msg("start register platform driver.");
    ret = platform_driver_register(&plt_drv);
    if(ret) {
        err_msg("platform driver register failed.");
    }
    return ret;
}
static void __init demo_input_system_exit(void)
{
    usr_msg("start unregister platform driver.");
    platform_driver_unregister(&plt_drv);
}

module_init(demo_input_system_init);
module_exit(demo_input_system_exit);

module_param(node_major, int, S_IRUGO);

MODULE_AUTHOR("QUAN");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("test char device driver");
module_i2c_driver

