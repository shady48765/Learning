#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/device.h>







static DEVICE_ATTR(dimm_edac_mode, S_IRUGO, dimmdev_edac_mode_show, NULL);





static int __inline foo_init(void)
{
	
	device_create_file(struct device * device, const struct device_attribute * entry);

	sysfs_create_group(struct kobject * kobj, const struct attribute_group * grp);

}

static void __exit foo_exit(void)
{

}



module_init(foo_init);
module_exit(foo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("machester");

