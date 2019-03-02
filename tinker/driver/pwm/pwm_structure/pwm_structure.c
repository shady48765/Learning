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


static inline struct pwm_info_container *to_chip(struct pwm_chip *chip)
{
	return container_of(chip, struct pwm_info_container, chip);
}

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

static void usr_pwm_remove_file(struct device * dev)
{
	usr_msg("pwm device destory file start");
	device_remove_file(dev, &usr_pwm_attrs);
}
#endif      // end of #if DEV_CREATE_FILE


static int get_current_clk_freq(struct pwm_chip *chip, struct platform_device *pdev)
{
	// pwm clock id/name has define as "pwm" in dts
	int ret;
	struct pwm_info_container * priv = to_chip(chip);
	if(IS_ERR(priv)) {
		err_msg("error : get pwm_info_container failed.");
		return ERR_PTR(-ENODEV);
	}
	mutex_lock(&priv->lock);
	/** get system pwm clock -------------------------------------------------*/
	priv->pwm_clk = devm_clk_get(&pdev->dev, "pwm");
	if(IS_ERR(priv->pwm_clk)) {
		err_msg("error : in devm_clk_get failed.");
		ret = ERR_PTR(-ENODEV);
		goto out;
	}
	// usr_msg("system setted pwm source clock mini_rate = %ld, max_rate = %ld", 
	//				priv->pwm_clk->min_rate, priv->pwm_clk->max_rate);
	/** get system pll clock -------------------------------------------------*/
	priv->pll_clk = devm_clk_get(&pdev->dev, "pclk");
	if(IS_ERR(priv->pll_clk)) {
		err_msg("error : in get APB clock failed.");
		ret = ERR_PTR(-ENODEV);
		goto out;
	}
	// usr_msg("get system APB clock mini_rate = %ld, max_rate = %ld", 
	//				priv->pll_clk->min_rate, priv->pll_clk->max_rate);
	mutex_unlock(&priv->lock);
	return 0;

out:
	mutex_unlock(&priv->lock);
	return ret;
}

static int usr_pwm_request(struct pwm_chip *chip, struct pwm_device *pwm)
{
	/*
	 * One may need to do some initialization when a PWM channel
	 * of the controller is requested. This should be done here.
	 *
	 * One may do something like 
	 *     prepare_pwm_device(struct pwm_chip *chip, pwm->hwpwm);
	 */

	int ret;
	struct pwm_info_container * pwm_info = to_chip(chip);
	if(IS_ERR(pwm_info)) {
		err_msg("error : get pwm_info_container failed.");
		return ERR_PTR(-ENODEV);
	}
	mutex_lock(&pwm_info->lock);
	//struct pwm_device *pwm_request(int pwm, const char *label)
    pwm = pwm_request(2, "pwm-dimming");
	if(IS_ERR(pwm)) {
		err_msg("error : get pwm_info_container failed.");
		ret = ERR_PTR(-ENODEV);
		goto out;
	}
	usr_msg("success requset pwm channel %d, label = %s, flags = %ld", pwm->pwm, pwm->label, pwm->flags);
	pwm_info->chip.pwms = pwm;

	mutex_unlock(&pwm_info->lock);
	return 0;

out:
	mutex_unlock(&pwm_info->lock);
	return ret;
}

static int usr_pwm_config(struct pwm_chip *chip, struct pwm_device *pwm,
			       int duty_ns, int period_ns)
{
    /*
     * In this function, one ne can do something like:
     *      struct pwm_info_container *priv = to_chip(chip);
     *
	 *      return send_command_to_set_config(priv,
     *                      duty_ns, period_ns);
     */

	int ret;
	struct pwm_info_container *priv = to_chip(chip);
	if(IS_ERR(priv)) {
		err_msg("error : get pwm_info_container failed.");
		return ERR_PTR(-ENODEV);
	}

	ret = pwm_config(priv->chip.pwms, duty_ns, period_ns);

	if(ret < 0) {
		err_msg("error : pwm_config");
		return -EIO;
	}
	
	return 0;
}

static int usr_pwm_enable(struct pwm_chip *chip, struct pwm_device *pwm)
{
    /*
     * In this function, one ne can do something like:
     *  struct pwm_info_container *priv = to_chip(chip);
     *
     * return foo_chip_set_pwm_enable(priv, pwm->hwpwm, true);
     */
	int ret;
	
    usr_msg("Ready to enable pwm device number = %d, pwm = %d", pwm->hwpwm, pwm->pwm);
	ret = pwm_enable(pwm);
	if(ret < 0) {
		err_msg("error : pwm_enable");
		return -EIO;
	}
	return 0;
}

static void usr_pwm_disable(struct pwm_chip *chip, struct pwm_device *pwm)
{
    /*
     * In this function, one ne can do something like:
     *  struct pwm_info_container *priv = to_chip(chip);
     *
     * return foo_chip_set_pwm_enable(priv, pwm->hwpwm, false);
     */
	
    usr_msg("Ready to disable pwm device number = %d, pwm = %d", pwm->hwpwm, pwm->pwm);
	pwm_disable(pwm);
}

static const struct pwm_ops pwm_ops = {
	.request = usr_pwm_request,
	.config  = usr_pwm_config,
	.enable  = usr_pwm_enable,
	.disable = usr_pwm_disable,
	.owner   = THIS_MODULE,
};


static int read_pwm_dts_info(struct platform_device *pdev, struct pwm_chip * chip)
{
	int ret;
	struct pwm_info_container * pwm_info = to_chip(chip);
	if(IS_ERR(pwm_info)) {
		err_msg("error : get pwm_info_container failed.");
		return ERR_PTR(-ENODEV);
	}
	if(!pdev->dev.of_node) {
		err_msg("error : read dts info, node error");
		return -EFAULT;
	}
	mutex_lock(&pwm_info->lock);
	ret = of_property_read_u32(pdev->dev.of_node, "pwm_period", &pwm_info->dts.period);
	if(ret < 0) {
		err_msg("error : read dts period");
		goto out;
	}
	ret = of_property_read_u32(pdev->dev.of_node, "pwm_freqency", &pwm_info->dts.freq);
	if(ret < 0) {
		err_msg("error : read dts freq");
		goto out;
	}
	ret = of_property_read_u32(pdev->dev.of_node, "pwm_polarity", &pwm_info->dts.polarity);
	if(ret < 0) {
		err_msg("error : read dts polarity");
		goto out;
	}
	mutex_unlock(&pwm_info->lock);
	return ret;
out:
	mutex_unlock(&pwm_info->lock);
	return ret;
}

static int usr_pwm_register_device(struct platform_device *pdev)
{
	return 0;
}


static void usr_pwm_unregister_device(struct platform_device *pdev)
{

	
#if 0
    struct pwm_dev * pwm_dev;

	pwm_dev = platform_get_drvdata(pdev);
	usr_msg("usr pwm unregister start");
	usr_pwm_remove(&pdev->dev);
#endif
}

static int usr_pwm_probe(struct platform_device *pdev)
{
    int err;
   	struct pwm_info_container * dev_info;
	
	usr_msg("pwm driver init start");
	dev_info = devm_kmalloc(&pdev->dev, sizeof(struct pwm_info_container), GFP_KERNEL);
	if(IS_ERR(dev_info)) {
		err_msg(" err : devm_kmalloc");
		return -ENOMEM;
	}
	memset(dev_info, 0, sizeof(struct pwm_info_container));
	
    mutex_init(&dev_info->lock);
	dev_info->chip.ops  = &pwm_ops;
	dev_info->chip.dev  = &pdev->dev;
	dev_info->chip.base = -1;	// Dynamic base
	dev_info->chip.npwm = 2;	// 2 channel controller / pwm2

	read_pwm_dts_info(pdev, &dev_info->chip);
	
	err = pwmchip_add(&dev_info->chip);
	if(err < 0) {
		err = -ENODEV;
		err_msg("error : pwmchip_add");
		goto err_no1;
	}
	usr_pwm_dev_create_file(&pdev->dev);

	platform_set_drvdata(pdev, dev_info);
	
	return err;


err_no1:
	return err;
}

static int usr_pwm_remove(struct platform_device *pdev)
{
	struct pwm_info_container * dev_info = platform_get_drvdata(pdev);
	usr_msg("move in usr_pwm_remove.");
	usr_pwm_remove_file(&pdev->dev);
	pwmchip_remove(&dev_info->chip);
	return 0;
}

static int usr_pwm_suspend(struct platform_device * pdev, pm_message_t state)
{
    usr_msg("move in usr_pwm_suspend.");
	/** TODO */
    return 0;
}

static int usr_pwm_resume(struct platform_device * pdev)
{
	usr_msg("move in usr_pwm_resume.");
	/** TODO */
    return 0;
}


static const struct of_device_id usr_pwm_match_table[] = {
	{.compatible = "pwm-dimming",},
    {/** keep this */},
};
// MODULE_DEVICE_TABLE(of, usr_pwm_match_table);

static const struct platform_device_id usr_pwm_dev_id[] = {
    {"usr_pwm", 0},
    {/*keep this*/}
};
MODULE_DEVICE_TABLE(pwm, usr_pwm_dev_id);

struct platform_driver drv_dimming = {
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

module_platform_driver(drv_dimming);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("quan");
MODULE_DESCRIPTION("Linux kernel pwm driver demo.");
MODULE_ALIAS("PWM DEMO");
