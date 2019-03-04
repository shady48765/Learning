/***** key dts defined in file rk3288.dtsi ***********************

*******************************************************************************/
#include "key_input.h"

static int usr_pwm_probe(struct platform_device *pdev)
{
    int err;

	
    
	return err;

err_no1:
	return err;
}

static int usr_pwm_remove(struct platform_device *pdev)
{

	return 0;
}

static const struct of_device_id usr_key_match_table[] = {
	{.compatible = "xxxxxxxxxxxxxxxxx",},
    {/** keep this */},
};
// MODULE_DEVICE_TABLE(of, usr_key_match_table);

static const struct platform_device_id usr_key_dev_id[] = {
    {"usr_key", 0},
    {/*keep this*/}
};
MODULE_DEVICE_TABLE(pwm, usr_key_dev_id);

struct platform_driver drv_key = {
	.driver = {
		.owner          = THIS_MODULE,
		.name           = USR_KEY_NAME,
		.of_match_table = of_match_ptr(usr_key_match_table), 
	},
	.id_table = usr_key_dev_id,
	.probe    = usr_pwm_probe,
	.remove   = usr_pwm_remove,
};

module_platform_driver(drv_key);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("quan");
MODULE_DESCRIPTION("Linux kernel pwm driver demo.");
MODULE_ALIAS("KEY DEMO");
