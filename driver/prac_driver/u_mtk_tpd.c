/******************************************************************************
 * mtk_tpd.c - MTK Android Linux Touch Panel Device Driver               *
 *                                                                            *
 * Copyright (C) 2016 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 *                                                                            *
 * DESCRIPTION:                                                               *
 *     this file provide basic touch panel event to input sub system          *
 *                                                                            *
 * AUTHOR:                                                                    *
 *     Kirby.Wu (mtk02247)                                                    *
 *                                                                            *
 * NOTE:                                                                      *
 * 1. Sensitivity for touch screen should be set to edge-sensitive.           *
 *    But in this driver it is assumed to be done by interrupt core,          *
 *    though not done yet. Interrupt core may provide interface to            *
 *    let drivers set the sensitivity in the future. In this case,            *
 *    this driver should set the sensitivity of the corresponding IRQ         *
 *    line itself.                                                            *
 ******************************************************************************/

#include "tpd.h"
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/fb.h>

#ifdef CONFIG_COMPAT
#include <linux/compat.h>
#endif

// added by elink_phil start <<<
#include <elink_tp.h>
#include <elink_tp_list.h>

extern void elink_tp_init(const ELINK_TP_INFO *list, int size, long unsigned int *x_res, long unsigned int *y_res);

/* for magnify velocity******************************************** */
// unlocked_ioctl parameter set
// magic number
#define TOUCH_IOC_MAGIC 'A'
// compare cmd, unlocked_ioctl second parameter
#define TPD_GET_VELOCITY_CUSTOM_X _IO(TOUCH_IOC_MAGIC, 0)
#define TPD_GET_VELOCITY_CUSTOM_Y _IO(TOUCH_IOC_MAGIC, 1)
#define TPD_GET_FILTER_PARA _IOWR(TOUCH_IOC_MAGIC, 2, struct tpd_filter_t)
#ifdef CONFIG_COMPAT
#define COMPAT_TPD_GET_FILTER_PARA _IOWR(TOUCH_IOC_MAGIC, 2, struct tpd_filter_t)
#endif
	struct tpd_filter_t tpd_filter;
struct tpd_dts_info tpd_dts_data;
EXPORT_SYMBOL(tpd_dts_data);

struct pinctrl *pinctrl1;
struct pinctrl_state *pins_default;
struct pinctrl_state *eint_as_int, *eint_output0, *eint_output1, *rst_output0, *rst_output1;
struct of_device_id touch_of_match[] = {
	{ .compatible = "mediatek,mt6570-touch", },
	{ .compatible = "mediatek,mt6735-touch", },
	{ .compatible = "mediatek,mt6580-touch", },
	{ .compatible = "mediatek,mt8173-touch", },
	{ .compatible = "mediatek,mt6755-touch", },
	{ .compatible = "mediatek,mt6757-touch", },
	{ .compatible = "mediatek,mt6797-touch", },
	{ .compatible = "mediatek,mt8163-touch", },
	{ .compatible = "mediatek,mt8127-touch", },
	{ .compatible = "mediatek,mt2701-touch", },
	{ .compatible = "mediatek,mt7623-touch", },
	{},
};
EXPORT_SYMBOL(touch_of_match);

void tpd_get_dts_info(void)
{

}
EXPORT_SYMBOL(tpd_get_dts_info);

static DEFINE_MUTEX(tpd_set_gpio_mutex);
void tpd_gpio_as_int(int pin)
{
	mutex_lock(&tpd_set_gpio_mutex);
	TPD_DEBUG("[tpd]tpd_gpio_as_int\n");
	if (pin == 1)
		pinctrl_select_state(pinctrl1, eint_as_int);
	mutex_unlock(&tpd_set_gpio_mutex);
}
EXPORT_SYMBOL(tpd_gpio_as_int);

void tpd_gpio_output(int pin, int level)
{
}
EXPORT_SYMBOL(tpd_gpio_output);

int tpd_get_gpio_info(struct platform_device *pdev)
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

static int tpd_misc_open(struct inode *inode, struct file *file)
{
	return nonseekable_open(inode, file);
}

static int tpd_misc_release(struct inode *inode, struct file *file)
{
	return 0;
}

#ifdef CONFIG_COMPAT
static long tpd_compat_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{

}
#endif
static long tpd_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{

}
static struct work_struct touch_resume_work;
static struct workqueue_struct *touch_resume_workqueue;
static const struct file_operations tpd_fops = {
/* .owner = THIS_MODULE, */
	.open = tpd_misc_open,
	.release = tpd_misc_release,
	.unlocked_ioctl = tpd_unlocked_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = tpd_compat_ioctl,
#endif
};

/*----------------------------------------------------------------------------*/
static struct miscdevice tpd_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "touch",
	.fops = &tpd_fops,
};

/* ********************************************** */
/* #endif */


/* function definitions */
static int tpd_probe(struct platform_device *pdev);
static int tpd_remove(struct platform_device *pdev);
static struct work_struct tpd_init_work;
static struct workqueue_struct *tpd_init_workqueue;
static int tpd_suspend_flag;
int tpd_register_flag = 0;
/* global variable definitions defined in tpd.h*/
struct tpd_device *tpd = 0;
static struct tpd_driver_t tpd_driver_list[TP_DRV_MAX_COUNT];	/* = {0}; */

struct platform_device tpd_device = {
	.name		= TPD_DEVICE,
	.id			= -1,
};
const struct dev_pm_ops tpd_pm_ops = {
	.suspend = NULL,
	.resume = NULL,
};
static struct platform_driver tpd_driver = {
	.remove = tpd_remove,
	.shutdown = NULL,
	.probe = tpd_probe,
	.driver = {
			.name = TPD_DEVICE,
			.pm = &tpd_pm_ops,	// .pm : power manager
			.owner = THIS_MODULE,
			.of_match_table = touch_of_match,
	},
};
static struct tpd_driver_t *g_tpd_drv;
/* hh: use fb_notifier */
static struct notifier_block tpd_fb_notifier;
/* use fb_notifier */
static void touch_resume_workqueue_callback(struct work_struct *work)
{
	TPD_DEBUG("GTP touch_resume_workqueue_callback\n");

}
// got LCD status and run suitable event
static int tpd_fb_notifier_callback(struct notifier_block *self, unsigned long event, void *data)
{

}

int tpd_driver_remove(struct tpd_driver_t *tpd_drv)
{

}
EXPORT_SYMBOL(tpd_driver_remove);

static void tpd_create_attributes(struct device *dev, struct tpd_attrs *attrs)
{

}

/* touch panel probe */
// run after platform device match to platform driver
static int tpd_probe(struct platform_device *pdev)
{

}


static int tpd_remove(struct platform_device *pdev)
{

}


/* Add driver: if find TPD_TYPE_CAPACITIVE driver successfully, loading it */
int tpd_driver_add(struct tpd_driver_t *tpd_drv)
{

}
EXPORT_SYMBOL(tpd_driver_add);

#define TPD_DEVICE "test_tpd"

// ---> fill up struct platform_device ---> start
static struct platform_device tp_device = {
	.name = TPD_DEVICE,				// platform device name
	.id   = -1,
}
// <--- fill up struct platform_device <--- end
// ---> fill up struct platform_driver ---> start
// question: of_device_id 找不到
static struct  of_device_id tpd_id_table[] = {
	{ .compatible = "mediatek,mt6570-touch", },
	{ .compatible = "mediatek,mt6735-touch", },
	{ .compatible = "mediatek,mt6580-touch", },
	{ .compatible = "mediatek,mt8173-touch", },
	{ .compatible = "mediatek,mt6755-touch", },
	{ .compatible = "mediatek,mt6757-touch", },
	{ .compatible = "mediatek,mt6797-touch", },
	{ .compatible = "mediatek,mt8163-touch", },
	{ .compatible = "mediatek,mt8127-touch", },
	{ .compatible = "mediatek,mt2701-touch", },
	{ .compatible = "mediatek,mt7623-touch", },
	{},
};
EXPORT_SYMBOL(tpd_id_table)
static struct platform_driver tp_driver = {
	.remove = tpd_remove,
	.probe = tpd_probe,
	.driver = {
		.name = TPD_DEVICE,		//platform driver name
		.owner = THIS_MODULE,
		.of_match_table = tpd_id_table,
	},
};
// <--- fill up struct platform_driver <--- end

/**
 * register tpd_platform_driver
 * @param work : none
 */
static void tpd_init_work_callback(struct work_struct *work)
{
	TPD_DEBUG("---> touch panel init");
	// 匹配 tpd_id_table 信息注册驱动
	if(0 != platform_driver_register(&tp_driver))
		TPD_DMESG("---> unable to register touch panel platform driver\n");
}
EXPORT_SYMBOL(tpd_init_work_callback);

static struct workqueue_struct *tpd_init_workqueue;
int tpd_device_init(void)
{
	int res = 0;
	// declaration sigle thread work queue, entry is tpd_init_workqueue
	// work queue name is mtk-tpd
	tpd_init_workqueue = create_singlethread_workqueue("mtk-tpd");
	// add tpd_init_work_callback to tpd_init_work work queue and initialize it
	INIT_WORK(&tpd_init_work , tpd_init_work_callback);
	// call queue_work funciton to run tpd_init_workqueue
	res = queue_work(tpd_init_workqueue, &tpd_init_work);
	if (!res)
		pr_err("tpd : touch device init failed res:%d\n", res);
	return 0;
}
EXPORT_SYMBOL(tpd_device_init);
/* should never be called */
void  tpd_device_exit(void)
{
	TPD_DMESG("MediaTek touch panel driver exit\n");
	/* input_unregister_device(tpd->dev); */
	platform_driver_unregister(&tpd_driver);
}
EXPORT_SYMBOL(tpd_device_exit);




