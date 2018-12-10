/**
	laser_led {
		compatible = "laser_led";
		laser = <&gpio5, RK_PC3, GPIO_ACTIVE_LOW>;
		label = "laser_pin";
		pinctrl-name = "default";
		pinctrl-0 = <&laser_blink>;
		status = "okay";
	};
    &pinctrl {

        laser_blink: laser-blink {

                laser-on = <RK_GPIO5 RK_PC3 GPIO_ACTIVE_HIGH, &pcfg_pull_down>;
                laser-off = <RK_GPIO5 RK_PC3 GPIO_ACTIVE_LOW, &pcfg_pull_down>;
        }

        ......
    }


 */
#ifndef __GPIO_PINCTRL_H


#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/gpio.h> /* For Legacy integer based GPIO */
#include <linux/init.h>
// #include <linux/interrupt.h> /* For IRQ */
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/module.h>
#include <linux/of.h>      /* For DT*/
#include <linux/of_gpio.h> /* For of_gpio* functions */
#include <linux/of_device.h>
#include <linux/pinctrl/pinctrl.h>
#include <linux/pinctrl/consumer.h>
#include <linux/pinctrl/pinctrl-state.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h> /* For platform devices */
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>




/*--------------------- macro defined start --------------------------*/
// #define usr_msg(args)                                               \
//     do {                                                            \
//         printk(KERN_ERR "-------> [%s] (%s)\n", args, __func__);    \
//     } while (0)

// #define err_msg(args)                                               \
//     do {                                                            \
//         printk(KERN_ERR "-------> [%s] (%s)\n", args, __func__);    \
//     } while (0)

#define usr_msg(fmt, arg...)                                        \
    do {                                                            \
        printk(KERN_ERR "-------> [info] :" fmt, ##arg);            \
    } while (0)

#define err_msg(fmt, arg...)                                                \
    do {                                                                    \
        printk(KERN_ERR "-------> [error] : "fmt, ##arg));                  \
    } while (0)

#define CHECK(expr)           \
        do {                  \
            int ret = (expr); \
            ASSERT(0 == ret); \
        }while(0)
        
        
#define CHR_IOC_MAGIC       'G'

#define DRIVER_NAME         "laser_driver"
#define LASER_NAME          "laser"
#define LASER_CLS_NAME      "laser_class"
#define NODE_FROM           NULL
/*--------------------- macro defined end --------------------------*/

// struct device_node	* laser_node;
static int get_dts_info(const char *compat);
static int dev_open(struct inode *inode, struct file *filp);
static int dev_release(struct inode *inode, struct file *filp);
static ssize_t dev_write(struct file *flip, const char __user *buff,
                         size_t counter, loff_t *fops) ;
static long dev_ioctl(struct file *flip, unsigned int cmd,
                      unsigned long param);

static int get_dts_info(struct device * dev);




#endif
/**--------------------------------------- END LINE -------------------------------------*/