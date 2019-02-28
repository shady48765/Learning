/***** pwm dts defined in file rk3288.dtsi ***********************
	pwm2: pwm@ff680020 {
		compatible = "rockchip,rk3288-pwm";
		reg = <0x0 0xff680020 0x0 0x10>;
		#pwm-cells = <3>;
		pinctrl-names = "active";
		pinctrl-0 = <&pwm2_pin>;
		clocks = <&cru PCLK_PWM>;
		clock-names = "pwm";
		status = "disabled";
	};

	pwm3: pwm@ff680030 {
		compatible = "rockchip,rk3288-pwm";
		reg = <0x0 0xff680030 0x0 0x10>;
		#pwm-cells = <2>;
		pinctrl-names = "active";
		pinctrl-0 = <&pwm3_pin>;
		clocks = <&cru PCLK_PWM>;
		clock-names = "pwm";
		status = "disabled";
	};
*******************************************************************************/
#include "pwm_structure.h"

/* start define struct -----------------------------------*/

static struct dts_info {
	unsigned int pin;

	unsigned int period;
	unsigned int freq;
	pwm_polarity polarity;
	unsigned int duty_cycle;
};


static struct pwm_dev {	
	struct mutex 		lock;
	struct pwm_chip 	chip;
	struct dts_info 	dts_info;

	int foo;
	int bar;
};

/* end define struct -----------------------------------*/

/*-------------- standard timer start ------------------------------------*/
#if USED_HRS_TIMER

#define MS_TO_NS(x) (x * 1000000)      // ms to ns

struct pwm_timer_info {
	struct 	 mutex		 tim_lock;
    struct   hrtimer     usr_hrtimer;
    struct   timeval     timval;
    ktime_t  tm_period;
};

static struct pwm_timer_info      * usr_tim;

static enum hrtimer_restart pwm_hrtimer_callback(struct hrtimer * arg)
{
    ktime_t now = arg->base->get_time();
    usr_msg("timer running at jiffies=%ld\n", jiffies);
    hrtimer_forward(arg, now, usr_tim->tm_period);
    return HRTIMER_RESTART;
}

int usr_pwm_timer_init(unsigned int usr_ticks)
{
    int retval;

    if(usr_ticks > 5000 || usr_ticks <= 0) {
        err_msg("error : init timer ticks error");
        return -EINVAL;
    }
    usr_tim = kmalloc(sizeof(struct pwm_timer_info), GFP_KERNEL);
    if(!usr_tim) {
        err_msg("error : usr_tim struct malloc");
        retval = -ENOMEM;
        return retval;
    }
    
    mutex_init(&usr_tim->tim_lock);
    mutex_lock(&usr_tim->tim_lock);
    // ktime_set(const s64 secs, const unsigned long nsecs); // param1: second, param2:nanosecond
    usr_tim->tm_period = ktime_set(0, MS_TO_NS(usr_ticks));     // set 1second, 1000 nanosecond.
    hrtimer_init(&usr_tim->usr_hrtimer, CLOCK_REALTIME, HRTIMER_MODE_REL);
    usr_tim->usr_hrtimer.function = pwm_hrtimer_callback;
    hrtimer_start(&usr_tim->usr_hrtimer, usr_tim->tm_period, HRTIMER_MODE_REL);
    mutex_unlock(&usr_tim->tim_lock);

    return retval;
}
#endif // end #if USED_HRS_TIMER

#if DEV_CREATE_FILE
static ssize_t usr_show(struct device *dev, struct device_attribute *attr,
			char *buf)
{
	usr_msg("usr show");
    return 0;
}

static ssize_t usr_close(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
{
	usr_msg("usr close");
    return 0;
}

static struct device_attribute usr_pwm_attrs = {
	.show = usr_show,
	.store = usr_close,
	.attr = {
		.name = "usr_pwm_attr",
		.mode = 0664,
	},
};


static int usr_pwm_dev_create_file(struct device * dev)
{
    int err;
	usr_msg("pwm device create file start");
	err = device_create_file(dev, &usr_pwm_attrs);
    if(err < 0) {
        err_msg("error : create file");
        err =  -EINVAL;
    }
    return err;
}

static void usr_pwm_destory_sysfs(struct device * dev)
{
	usr_msg("pwm device destory file start");
	device_remove_file(dev, &usr_pwm_attrs);
    return 0;
}
#endif      // end of #if DEV_CREATE_FILE


static int usr_pwm_register_device(struct platform_device *pdev)
{
    int ret;
	struct pwm_dev * pwm_dev;
	pwm_dev = platform_get_drvdata(pdev);	
	
    ret = alloc_chrdev_region(&pwm_dev->pwm_dev_num, 0, 1, USR_PWM_DRV_NAME);
    if(ret < 0) {
        err_msg("error : alloc_chrdev_region");
        ret = -ENODEV;
        return ret;
    }
	
    pwm_dev->pwm_major_num = MAJOR(pwm_dev->pwm_dev_num);
    usr_msg("device major number = %d", pwm_dev->pwm_major_num);

	ret = usr_pwm_dev_create_file(&pdev->dev);
    return ret;
}


static void usr_pwm_unregister_device(struct platform_device *pdev)
{
    struct pwm_dev * pwm_dev;

	pwm_dev = platform_get_drvdata(pdev);
	usr_msg("usr pwm unregister start");
	usr_pwm_destory_sysfs(&pdev->dev);
}

static int usr_pwm_request(void)
{
    pwm_get();
}








static inline struct pwm_dev *to_chip(struct pwm_chip *chip)
{
	return container_of(chip, struct pwm_dev, chip);
}


static int pwm_request(struct pwm_chip *chip, struct pwm_device *pwm)
{
	/*
	 * One may need to do some initialization when a PWM channel
	 * of the controller is requested. This should be done here.
	 *
	 * One may do something like 
	 *     prepare_pwm_device(struct pwm_chip *chip, pwm->hwpwm);
	 */

	return 0;
}

static int pwm_config(struct pwm_chip *chip, struct pwm_device *pwm,
			       int duty_ns, int period_ns)
{

    /*
     * In this function, one ne can do something like:
     *      struct pwm_dev *priv = to_chip(chip);
     *
	 *      return send_command_to_set_config(priv,
     *                      duty_ns, period_ns);
     */

	return 0;
}

static int pwm_enable(struct pwm_chip *chip, struct pwm_device *pwm)
{
    /*
     * In this function, one ne can do something like:
     *  struct pwm_dev *priv = to_chip(chip);
     *
     * return foo_chip_set_pwm_enable(priv, pwm->hwpwm, true);
     */
    
    pr_info("Somebody enabled PWM device number %d of this chip", pwm->hwpwm);
	return 0;
}

static void pwm_disable(struct pwm_chip *chip, struct pwm_device *pwm)
{
    /*
     * In this function, one ne can do something like:
     *  struct pwm_dev *priv = to_chip(chip);
     *
     * return foo_chip_set_pwm_enable(priv, pwm->hwpwm, false);
     */
    
    pr_info("Somebody disabled PWM device number %d of this chip", pwm->hwpwm);
}

static const struct pwm_ops pwm_ops = {
	.request = pwm_request,
	.config  = pwm_config,
	.enable  = pwm_enable,
	.disable = pwm_disable,
	.owner   = THIS_MODULE,
};


static int read_pwm_dts_info(struct platform_device *pdev)
{
	static node 

	of_find_compatible_node(struct device_node * from, const char * type, const char * compat)
}

static int usr_pwm_probe(struct platform_device *pdev)
{
    int err;
   	struct pwm_dev * dev_info;
	
	usr_msg("pwm driver init start");
	dev_info = devm_kmalloc(sizeof(struct pwm_dev), GFP_KERNEL);
	if(IS_ERR(dev_info)) {
		err_msg(" err : devm_kmalloc");
		return -ENOMEM;
	}
	memset(dev_info, 0, sizeof(struct pwm_dev));
	
    mutex_init(&dev_info->lock);
	dev_info->chip.ops  = &pwm_ops;
	dev_info->chip.dev  = &pdev->dev;
	dev_info->chip.base = -1;	// Dynamic base
	dev_info->chip.npwm = 2;	// 2 channel controller

	platform_set_drvdata(pdev, dev_info);

	err = pwmchip_add(&dev_info->chip);
	if(err < 0) {
		err = -ENODEV;
		err_msg("error : pwmchip_add");
		goto err_no1;
	}
	
	return err;


err_no1:
	returnr err;
}

static int usr_pwm_remove(struct platform_device *pdev)
{
	struct pwm_dev * dev_info = platform_get_drvdata(pdev);
	usr_msg("move in usr_pwm_remove.");
	return pwmchip_remove(&dev_info->chip);
}

static int usr_pwm_suspend(struct platform_device * pdev, pm_message_t state)
{
    usr_msg("move in usr_pwm_suspend.");
    return 0;
}

static int usr_pwm_resume(struct platform_device * pdev)
{
	usr_msg("move in usr_pwm_resume.");
    return 0;
}


static const struct of_device_id usr_pwm_match_table[] = {
	{.compatible = "pwm-dimming",},
    {/** keep this */},
};
MODULE_DEVICE_TABLE(of, usr_pwm_match_table);

static const struct platform_device_id usr_pwm_dev_id[] = {
    {"usr_pwm", 0},
    {/*keep this*/}
};
MODULE_DEVICE_TABLE(usr_pwm, usr_pwm_dev_id);

struct platform_driver usr_pwm_drv = {
	.driver = {
		.owner          = THIS_MODULE,
		.name           = USR_PWM_DRV_NAME,
		.of_match_table = of_match_ptr(usr_pwm_match_table), 
	},
	.id_table = usr_pwm_dev_id,
	.probe    = usr_pwm_probe,
	.remove   = usr_pwm_remove,
	.suspend  = usr_pwm_suspend,
	.resume   = usr_pwm_resume,
};

module_platform_driver(usr_pwm_drv);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("quan");
MODULE_DESCRIPTION("Linux kernel pwm driver demo.");
MODULE_ALIAS("PWM DEMO");
