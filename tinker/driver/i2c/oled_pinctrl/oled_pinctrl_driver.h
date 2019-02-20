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
#include <linux/device.h>					// device_create_file

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
#include <linux/err.h>
#include <linux/timer.h>        //for timer_list, jiffy timer, standard timer
#include <linux/i2c.h>



#define OLED_DEBUG					0
#define HRTIMER_DEFINE              1
#define I2C_16_BIT_MODE				0
#define CHECK_I2C                   1


#define READ_FLAG			0x0001
#define WRITE_FLAG			0x0000


#define TAG                         " <OLED> "
#define USR_MSG_LEVEL               KERN_WARNING
#define USR_ERR_LEVEL               KERN_ERR
#define usr_msg(fmt, args...)       printk(USR_MSG_LEVEL TAG ""fmt"\n", ##args)
#define err_msg(fmt, args...)       printk(USR_ERR_LEVEL TAG " (function : %s), [line : %d] "fmt"\n",__func__, __LINE__, ##args)


#define    	OLED_DEV_NAME         "oled_ssd1306"

typedef enum {
    low     = 0,
    high    = 1
}status;

/* oled device information struction ---------------------------------------*/
struct _oled_dev_info {
    unsigned int    major;
    dev_t           devno;
    struct cdev     cdev;
    struct class    * cls;
    struct device   * dev;
	struct mutex 	dev_lock;
};
#if 0
/* oled i2c dts information struction ---------------------------------------*/
struct _oled_i2c_dts_info {
	struct device_node      	*i2c_node;
	struct device_node 	       	*dev_node;

	unsigned int 				oled_rst_pin;
	unsigned int 		        oled_width;
	unsigned int 		        oled_height;
	unsigned int 		        oled_i2c_clk;
	
	struct pinctrl 		        *dev_pinctrl;
	struct pinctrl_state 		*rst_high;
	struct pinctrl_state 		*rst_low;
	
};
#endif
/* oled i2c dts information struction ---------------------------------------*/
struct _oled_i2c_dts_info {
	struct device_node      	*i2c_node;

	unsigned int 				oled_status_pin;
	unsigned int 		        oled_width;
	unsigned int 		        oled_height;
	unsigned int 		        oled_i2c_clk;
	
};
/* oled dev dts information struction ---------------------------------------*/
struct _oled_dev_dts_info {
	struct device_node 	       	*dev_node;
	
	struct pinctrl 		        *dev_pinctrl;
	struct pinctrl_state 		*statu_high;
	struct pinctrl_state 		*statu_low;

	struct mutex 				dts_lock;
	
};
/* oled i2c information struction ---------------------------------------*/
struct _oled_i2c_info {
	char 				        * data;
	struct i2c_client 	        * client;
    struct mutex                oled_lock;
    struct i2c_msg		        msg;
    struct _oled_dev_dts_info   * dev_dts_info;
	struct _oled_i2c_dts_info 	* i2c_dts_info;
	struct _oled_dev_info		* dev_info;
	struct platform_device 		* oled_pdev;
};
/* oled timer -----------------------------------------------------------*/
struct _oled_timer {
    struct mutex            timer_lock;
    struct hrtimer          oled_timer;
    ktime_t                 tm_period;
};
struct _oled_timer          * tim;



/* oled whole information struction ---------------------------------------*/

static int oled_get_i2c_dts_info(struct i2c_client * client);

int oled_i2c_send_byte(struct i2c_client * client, unsigned char addr, unsigned char data);
int oled_i2c_send_matrix(struct i2c_client * client, unsigned char addr, unsigned char *data, unsigned int length);

static int register_oled_i2c_driver(struct _oled_i2c_info *i2c_info);
static int unregister_oled_i2c_driver(void);

static int oled_get_dev_dts_info(struct platform_device * pdev);
static int gpio_set_state(status state);
/* waitqueue function ------------------------------------------------*/
void waitqueue_init(void);

/* extern function declartion ------------------------------------------------*/
extern void oled_power_on(void);
extern void oled_init(void);



#if HRTIMER_DEFINE
    static int oled_timer_init(unsigned long ticks);
	static void oled_timer_remove(void);
#endif

#endif
