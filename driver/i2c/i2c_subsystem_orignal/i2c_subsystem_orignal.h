#ifndef __I2C_SUBSYTEM_ORIGNAL_h
#define __I2C_SUBSYTEM_ORIGNAL_h

#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>



#include <linux/cdev.h>

#include <linux/printk.h>
#include <linux/major.h>



/*--------------------- macro defined start --------------------------*/
#define usr_msg(args)                                           \
            do{                                                 \
                    printk(KERN_ERR "---> %s\n", args);         \
            }while(0)

#define err_msg(args)                                           \
            do{                                                 \
                    printk(KERN_ERR "-------> %s\n", args);     \
            }while(0)
/*--------------------- macro defined end --------------------------*/
/*------------ global parameter declaration start -------------------*/
static int eerom_fops_open(struct inode *inode, struct file *filp);
static int eerom_fops_release(struct inode *inode, struct file *filp);
static ssize_t eerom_fops_read(struct file * flip, char __user * buff, 
						size_t counter, loff_t * fops);
static ssize_t eerom_fops_write(struct file * flip, const char __user * buff,
                         size_t counter, loff_t * fops);
static long eerom_fops_ioctl (struct file *flip, unsigned int cmd, 
						unsigned long param);

void eerom_init(void);
int usr_i2c_send(struct i2c_client *client, char *buf, int size);
int usr_i2c_recv(struct i2c_client *client, char *buf, int size);
/*------------ global parameter declaration end -------------------*/

#endif
