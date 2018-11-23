#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/property.h>

int pd_probe(struct platform_device *pdev)
{
    struct fwnode_handle *child;    
    const char *p, *p2[2];
    int  n, n2[2];
    char mymac[6];

    //取属性what的值
    if (device_property_read_string(&pdev->dev, "what", &p) < 0)
        return -ENODEV;

    //取属性hello的字符串数组值
    if (device_property_read_string_array(&pdev->dev, "hello", p2, 2) < 0)
        return -ENODEV;

    //取属性haha的整型值
    if (device_property_read_u32_array(&pdev->dev, "haha", &n, 1) < 0)
        return -ENODEV; 

    //取属性hehe的整型数组  
    if (device_property_read_u32_array(&pdev->dev, "hehe", n2, 2) < 0)
        return -ENODEV;

    //取属性mymac的字节数组
    if (device_property_read_u8_array(&pdev->dev, "mymac", mymac, 6) < 0)
        return -ENODEV;


    printk(KERN_ERR"in myprobe ...%s\n", p);
    printk(KERN_ERR"%s,%s\n", p2[0], p2[1]);
    printk(KERN_ERR"n = %d\n", n);
    printk(KERN_ERR"n2: %d, %d\n", n2[0], n2[1]);
    printk(KERN_ERR"mymac=%x,%x,%x,%x,%x,%x\n", mymac[0], mymac[1], mymac[2], mymac[3],
                mymac[4], mymac[5]);    
    printk(KERN_ERR"######################################\n");
	//获取设备子节点的属性值


    //循环获取每个子节点的属性值
    device_for_each_child_node(&pdev->dev, child)
    {
        //取子节点的属性what的值
        if (0 <= fwnode_property_read_string(child, "what", &p))
            printk(KERN_ERR"child what=%s\n", p);

        //取子节点的属性hello的字符串数组值
        if (0 <= fwnode_property_read_string_array(child, "hello", p2, 2))
            printk(KERN_ERR"child hello=%s, %s\n", p2[0], p2[1]);

        //取子节点的属性haha的整型值
        if (0 <= fwnode_property_read_u32_array(child, "haha", &n, 1))
            printk(KERN_ERR"child haha=%d\n", n);

        //取子节点的属性hehe的整型数组  
        if (0 <= fwnode_property_read_u32_array(child, "hehe", n2, 2))
            printk(KERN_ERR"child hehe=%d,%d\n", n2[0], n2[1]);

        //取子节点属性mymac的字节数组
        if (0 <= fwnode_property_read_u8_array(child, "mymac", mymac, 6))
            printk(KERN_ERR"child mymac=%x,%x,%x,%x,%x,%x\n", mymac[0], mymac[1], mymac[2], mymac[3],
                mymac[4], mymac[5]);    
        printk(KERN_ERR"---------------------\n");
    }

    return 0;
}

int pd_remove(struct platform_device *pdev)
{
    printk(KERN_ERR"in pd_remove ...\n");
    return 0;
}


struct of_device_id ids[] = {
    {.compatible = "mydt,test"},
    {},
};
    
MODULE_DEVICE_TABLE(of, ids);

struct platform_driver pdrv = {
    .driver = {
        .name = "mydrv",
        .owner = THIS_MODULE,
        .of_match_table = ids,
    },

    .probe = pd_probe,
    .remove = pd_remove,
};

module_platform_driver(pdrv);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("QUAN");
MODULE_DESCRIPTION("no");
MODULE_VERSION("test_1.0");