
#include "driver_oled.h"


int oled_probe (struct platform_device * pdev)
{
    
}

int oled_remove (struct platform_device * pdev)
{

}

static struct of_device_id oled_match[] = {
    {"compatiable = oled_driver", },
    {/*keep empty*/},
}
MODULE_DEVICE_TABLE(of, oled_match);

struct platform_driver oled_driver = {
    .probe = oled_probe;
    .remove = oled_remove;
    .driver = {
        .name = OLED_DRV_NAME,
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(oled_match);
    },
};


module_platform_driver(oled_driver);

MODULE_AUTHOR("V");
MODULE_ALIAS("OLED driver");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("/*FIXME*/");