#include "dev_sysfs_demo.h"

#define FOO_DEV_NAME                "foo_sysfs"


static struct platform_device *foo_platform_dev;

static int __init sysfs_dev_init(void)
{
	usr_msg("register device name is : %s", FOO_DEV_NAME);

	foo_platform_dev = platform_device_alloc(FOO_DEV_NAME, -1);
	if (!foo_platform_dev) {
		err_msg("platform alloc error.");
		return -ENOMEM;
	}

	return platform_device_add(foo_platform_dev);;
}


static void __exit sysfs_dev_exit(void)
{
	platform_device_del(foo_platform_dev);
	usr_msg("removed device: %s", FOO_DEV_NAME);
}

module_init(sysfs_dev_init);
module_exit(sysfs_dev_exit);

MODULE_AUTHOR("V");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("platform char device test");