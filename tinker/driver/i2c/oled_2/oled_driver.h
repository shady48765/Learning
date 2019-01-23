#ifndef __OLED_DRIVER_H
#define __OLED_DRIVER_H

#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/of.h>                       /* For DT*/
#include <linux/fs.h>                       // ioctl and file_operations support
#include <linux/gpio.h>                     /* For Legacy integer based GPIO */
#include <linux/device.h>

#include <linux/delay.h>
#include <asm/uaccess.h>
#include <linux/major.h>
#include <linux/module.h>
#include <linux/of_gpio.h>                  /* For of_gpio* functions */
#include <linux/of_device.h>
#include <linux/pinctrl/pinctrl.h>
#include <linux/pinctrl/consumer.h>
#include <linux/pinctrl/pinctrl-state.h>
#include <linux/platform_device.h>          /* For platform devices */
#include <linux/printk.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
// #include <linux/proc.h>
#include <linux/time.h>	// for get system current time
#include <linux/proc_fs.h>
// #include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/fb.h>
#include <asm/uaccess.h>
#include <linux/err.h>

#include <linux/i2c.h>



#define OLED_DEBUG					0


#define TAG                         " < OLED > "
#define USR_MSG_LEVEL               KERN_WARNING
#define USR_ERR_LEVEL               KERN_ERR
#define usr_msg(fmt, args...)       printk(USR_MSG_LEVEL TAG " (function : %s), [line : %d] "fmt"\n",__func__, __LINE__, ##args)
#define err_msg(fmt, args...)       printk(USR_ERR_LEVEL TAG " (function : %s), [line : %d] "fmt"\n",__func__, __LINE__, ##args)


#define    OLED_DEV_NAME         "OLED_SSD1306"


typedef enum {
    low     = 0,
    high    = 1
}status;

int gpio_set_state(status state);
int struct_mallock_init(void);
int oled_get_gpio_dts_info(struct platform_device *pdev);
int oled_get_i2c_dts_info(struct device_node *dev_node);
int register_oled_driver(void);

int i2c_send_byte(struct i2c_client * client, unsigned char data);
int i2c_recv_byte(struct i2c_client * client, unsigned char *data);
int i2c_send_matrix(struct i2c_client * client, unsigned char * buff, unsigned char length);
unsigned char * i2c_recv_matrix(struct i2c_client * client, unsigned char * buff, unsigned char length);
void waitqueue_init(void);
	
/* oled device information struction ---------------------------------------*/
struct _oled_device_info {
    unsigned int    oled_major;
    dev_t           oled_devno;
    struct cdev     oled_cdev;
    struct class    * oled_class;
    struct device   * oled_device;
    struct mutex    oled_dev_lock;
};
extern struct _oled_device_info * oled_dev_info;

/* oled dts information struction ---------------------------------------*/
struct _oled_dts_info {
	struct device_node 	       *node;
	int 				       oled_rst;
	unsigned int 		        oled_width;
	unsigned int 		        oled_height;
	unsigned int 		        oled_i2c_clk;

	struct pinctrl 		        *dev_pinctrl;
	struct pinctrl_state 		*rst_high;
	struct pinctrl_state 		*rst_low;
};
extern struct _oled_dts_info * oled_dts_info;

/* oled i2c information struction ---------------------------------------*/
struct _oled_i2c_info {	
	struct i2c_client 	* oled_client;
	char 				* data;
    struct mutex        oled_i2c_lock;
};
extern struct _oled_i2c_info * oled_i2c_info;

/* oled whole information struction ---------------------------------------*/

/* timer parameters initaliztion -------------------------------------------*/
// static struct timer_list oled_time;

#endif
