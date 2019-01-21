#ifndef __DRIVER_OLED_H
#define __DRIVER_OLED_H

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
// #include <linux/workqueue.h>
#include <linux/fb.h>
#include <asm/uaccess.h>
#include <linux/err.h>

#include <linux/i2c.h>



#include "common_cmd.h"
#include "oled_operation.h"

#define OLED_DEBUG					0




#define TAG                         " < OLED > "
#define USR_MSG_LEVEL               KERN_WARNING
#define USR_ERR_LEVEL               KERN_ERR
#define usr_msg(fmt, args...)       printk(USR_MSG_LEVEL TAG " (function : %s), [line : %d] "fmt"\n",__func__, __LINE__, ##args)
#define err_msg(fmt, args...)       printk(USR_ERR_LEVEL TAG " (function : %s), [line : %d] "fmt"\n",__func__, __LINE__, ##args)


#define    OLED_DEV_NAME         "OLED_SSD1306"


typedef enum {
    off = 0,
    on  = 1
}status;



extern void oled_write_byte(unsigned dat,unsigned cmd);
extern void oled_show_char(unsigned charx,unsigned char y,
                                unsigned char chr,unsigned char char_size);
extern void oled_show_string(unsigned char x,unsigned char y,
                                    unsigned char *chr,unsigned char char_size);
    
extern void oled_init(void);
extern  void oled_clear(void);
extern void oled_on(void);
extern void oled_off(void);
extern   char Write_IIC_Data(unsigned char IIC_Data);
extern int iic_write_reg(unsigned char cmd, unsigned char val);
extern char Write_IIC_Command(unsigned char IIC_Command);
static int i2c_master_reg8_send(const struct i2c_client *client, const char reg, 
                                                const char *buf, int count, int scl_rate);




    


static int oled_open(struct inode *inode, struct file *filp);
static int oled_close(struct inode *inode, struct file *filp);
static ssize_t oled_write(struct file *flip, const char __user *buff,
                                    size_t counter, loff_t *fops);
static long oled_ioctl (struct file *flip, unsigned int cmd, 
                            unsigned long param);

static int struct_mallock_init(void);
static int oled_get_dts_info(struct platform_device *pdev);
static int register_oled_driver(void);


static struct file_operations fops = {
    .open 			= oled_open,
    .release 		= oled_close,
    .write 			= oled_write,
    .unlocked_ioctl = oled_ioctl,
};
	
/* oled device information struction ---------------------------------------*/
static struct _oled_device_info {
    unsigned int    oled_major;
    dev_t           oled_devno;
    struct cdev     oled_cdev;
    struct class    * oled_class;
    struct device   * oled_device;
    struct mutex    oled_dev_lock;
};
static struct _oled_device_info * oled_dev_info;

/* oled dts information struction ---------------------------------------*/
static struct _oled_dts_info {
	struct device_node 	*node;
	int 				oled_rst;
	unsigned int 		oled_width;
	unsigned int 		oled_height;
	unsigned int 		oled_i2c_clk;

	struct pinctrl 		        *dev_pinctrl;
	struct pinctrl_state 		*rst_high;
	struct pinctrl_state 		*rst_low;
};
static struct _oled_dts_info * oled_dts_info;

/* oled i2c information struction ---------------------------------------*/
static struct _oled_i2c_info {	
	struct i2c_client 	* oled_client;
	char 				* data;
    struct mutex        oled_i2c_lock;
};
static struct _oled_i2c_info * oled_i2c_info;

/* oled whole information struction ---------------------------------------*/

/* timer parameters initaliztion -------------------------------------------*/
static struct timer_list oled_time;

#endif
