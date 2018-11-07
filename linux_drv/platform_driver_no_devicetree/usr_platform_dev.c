#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/printk.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>

/*--------------------- macro defined start --------------------------*/
#define usr_msg(args)                                           \
            do{                                                 \
                    printk(KERN_ERR "---> %s\n", args);         \
            }while(0)

#define err_msg(args)                                           \
            do{                                                 \
                    printk(KERN_ERR "-------> %s\n", args);     \
            }while(0)

#define PLT_DEV_NAME    "no_devtree_platform"
/*--------------------- macro defined end --------------------------*/

struct resource	 beep_res[] = {
	[0] = {
		.start = 0x888,
		.end   = 0x888,
		.name  = "fake_irq",
		.flags = IORESOURCE_IRQ,
	},
	[1] = {
		.start = 0x888,
		.end   = 0x888,
		.name  = "fake_irq",
		.flags = IORESOURCE_IRQ,
	},
	[2] = {
		.start = 0x888,
		.end   = 0x888,
		.name  = "fake_irq",
		.flags = IORESOURCE_IRQ,
	},
};
# if 0
// platform_device_register use
struct platform_device pd_device = {
	.name = "no_devtree_platform",
	.id   = -1,
	.num_resources = ARRAY_SIZE(beep_res),
	.resource = beep_res,
};
#endif
#if 1
struct platform_device *pd_device;
#endif

static int __init platform_dev_init(void)
{
	int ret;
	usr_msg("platform_device_register");

#if 0
	ret = platform_device_register(&pd_device);
#endif
#if 1
	pd_device = platform_device_alloc(PLT_DEV_NAME, -1);
	if (!pd_device) {
		err_msg("platform alloc error.");
		return -ENOMEM;
	}
	pd_device->num_resources = ARRAY_SIZE(beep_res);
	pd_device->resource = beep_res;
	ret = platform_device_add(pd_device);
#endif
	return ret;
}
EXPORT_SYMBOL_GPL(platform_dev_init);
static void __exit platform_dev_exit(void)
{
#if 0
	usr_msg("platform_device_unregister");
	platform_device_unregister(&pd_device);
#endif

#if 1
	platform_device_del(pd_device);
	//platform_device_release(&pd_device->dev);
#endif
}
EXPORT_SYMBOL_GPL(platform_dev_exit);


module_init(platform_dev_init);
module_exit(platform_dev_exit);

MODULE_AUTHOR("QUAN");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("platform char device test");



