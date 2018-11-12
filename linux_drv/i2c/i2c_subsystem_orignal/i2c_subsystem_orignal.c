/*----------------------------------------------------------------------*
 * Data : 2018-10-02
 * using struct cdev to register char device
 * creat class for device
 * fill read, write, ioctl, close
 *
 *
 *----------------------------------------------------------------------*/

#include "./i2c_subsystem_orignal.h"


#define USR_PLT_DRV             "usr_platform_drv"
#define USR_DEV_NAME            "usr_i2c_dev_name"

// create private data to divide every different part i2c peripheral
struct _eerom_private{
	char *name;             //decription
	unsigned int volum;     //volum
	int version;            //version
};
struct _eerom_info{
	struct i2c_client *client;                  // record probe中client
	struct miscdevice miscdev;                  // using misc device to allocate device node
	struct _eerom_private *eerom_private;        // specific i2c device private data 
};
struct _eerom_info * eerom_info;





// i2c send
// how to defined i2c address (i2c_client.addr). for user or from linux kernel?

int usr_i2c_send(struct i2c_client *client, char *buf, int size)
{	
	int ret;
	struct i2c_adapter *adapter = client->adapter;

	struct i2c_msg msg;
	msg.addr = client->addr;        // target i2c device address
	msg.flags = 0 ;                 // write flag
	msg.len = size;
	msg.buf = buf;
	
	/**
	 * [i2c_transfer description]
	 * int i2c_transfer(struct i2c_adapter *adap, struct i2c_msg *msgs, int num)
	 * @param  adap [data send to which i2c client]
	 * @param  msgs [data package need to be sent]
	 * @param  num  [how mangy data package need to be sent]
	 * @return      [return the number of successfuly sent message]
	 */
	 
	ret = i2c_transfer(adapter, &msg, 1);
	return ((ret >= 1) ? size : ret);
	
}

int usr_i2c_recv(struct i2c_client *client, char *buf, int size)
{
	int ret;
    struct i2c_msg msg;
    struct i2c_adapter *adapter = client->adapter;
    
    printk(KERN_ERR "---> start to run : [%s]\n", __FUNCTION__);
   
	msg.addr = client->addr;    // target i2c device address
	msg.flags = 1 ;             // read flag
	msg.len = size;
	msg.buf = buf;

	ret = i2c_transfer(adapter, &msg, 1);

	return ((ret >= 1) ? size : ret);
}

/*--- divid line -------------------------------------------------------*/

static int eerom_fops_open(struct inode *inode, struct file *filp)
{
	printk(KERN_ERR "---> start to run : [%s]\n", __FUNCTION__);


	return 0;
}

static ssize_t eerom_fops_write(struct file * flip, const char __user * buff,
                         size_t counter, loff_t * fops)
{
	int ret;
    printk(KERN_ERR "---> start to run : [%s]\n", __FUNCTION__);
    
	if(counter<=0 || counter >(eerom_info->eerom_private->volum))
		return -EINVAL;

	char *temp_buf = (char *)kzalloc(counter, GFP_KERNEL);
	if(temp_buf == NULL)
	{
		printk(KERN_ERR "------> error : kzalloc \n");
		return -ENOMEM;
	}
	
	ret = copy_from_user(temp_buf, buff, counter);
	if(ret > 0)
	{
		printk(KERN_ERR "------> error : copy_from_user \n");
		ret = -EFAULT;
		goto err_free;
	}
	// 2, send data to eerom
	#if 0
	    // kernel API
	    i2c_master_send(const struct i2c_client * client,const char * buf,int count)
	#endif
	ret = usr_i2c_send(eerom_info->client, temp_buf, counter);
	if(ret < 0)
	{
		printk(KERN_ERR "------> error : usr_i2c_send \n");
		goto err_free;
	}

	kfree(temp_buf);
	
	return counter;


err_free:
	kfree(temp_buf);
	return ret;

}
                         
static ssize_t eerom_fops_read(struct file * flip, char __user * buff, 
						size_t counter, loff_t * fops)
{
    int ret;
    
	printk(KERN_ERR "---> start to run : [%s]\n", __FUNCTION__);
	
	if(counter<=0 || counter>(eerom_info->eerom_private->volum))      // out of range
		return -EINVAL;

	char * temp_buf = (char *)kzalloc(counter, GFP_KERNEL);
	if(temp_buf == NULL)
	{
		printk(KERN_ERR "------> error : kzalloc\n");
		return -ENOMEM;
	}

	// 1,get data from i2c priphal
	ret = usr_i2c_recv(eerom_info->client, temp_buf, counter);
	if(ret < 0)
	{
		printk(KERN_ERR "------> error : usr_i2c_recv \n");
		goto err_free;
	}

	// 2，send data to user
	#if 0
	    // kernel API
	    i2c_master_recv(const struct i2c_client * client,const char * buf,int count)
	#endif
    
	ret = copy_to_user(buff, temp_buf, counter);
	if(ret > 0)
	{
		printk(KERN_ERR "------> error : copy_to_user \n");
		ret = -EFAULT;
		goto err_free;
	}

	kfree(temp_buf);
	
	return ret;

err_free:
	kfree(temp_buf);
	return ret;
}
                        
static long eerom_fops_ioctl (struct file *flip, unsigned int cmd, 
						unsigned long param)
{
    usr_msg("device eerom_fops_ioctl");
	return 0;
}
static int eerom_fops_release(struct inode *inode, struct file *filp)
{
	usr_msg("device close and restore the device default set");
	// device restore code
	return 0;
}                
void eerom_init(void)
{
    ; //!fix
}
/*------------ device parameter declaration start -------------------*/
const struct file_operations  eerom_fops = {
	.open           = eerom_fops_open,
	.write          = eerom_fops_write,
	.read           = eerom_fops_read,
	.release        = eerom_fops_release,
	.unlocked_ioctl = eerom_fops_ioctl,
};
/*------------------------------------------------------------------
step:
    1. record current matched i2c client
    2. reqeste device major
    3. create device node
    4. fill up file operation
    5. hardware init through i2c_transfer()
--------------------------------------------------------------------*/
int eerom_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret = 0;;

	// get specific i2c peripheral data through i2c_device_id
	struct _eerom_private *eerom_private = (struct _eerom_private *)id->driver_data;
    printk(KERN_ERR "mached i2c client info : ");
	printk(KERN_ERR "\t id->name = %s\n", id->name);
	printk(KERN_ERR "\t id->driver_data : name = %s\n", eerom_private->name);
	printk(KERN_ERR "\t id->driver_data : volum = %d\n", eerom_private->volum);
	printk(KERN_ERR "\t id->driver_data : version = %d\n", eerom_private->version);


	eerom_info = kzalloc(sizeof(struct _eerom_info), GFP_KERNEL);
    if(!eerom_info) {
        ret = -ENOMEM;
        goto err_kzmalloc;
    }
    
    //step 1:
	eerom_info->client = client;
    
    //step 2:
    eerom_info->miscdev.minor = MISC_DYNAMIC_MINOR;
    eerom_info->miscdev.name = USR_DEV_NAME;
    eerom_info->miscdev.fops = &eerom_fops;

	ret = misc_register(&eerom_info->miscdev);
    if(ret) {
        printk(KERN_ERR "misc_register error");
        goto err_misc_register;
    }
    //step 4:
    eerom_init();
	
	return 0;

err_misc_register:
    kfree(eerom_info);
err_kzmalloc:
    return ret;
    
}

int eerom_i2c_remove(struct i2c_client *client)
{
	
	misc_deregister(&eerom_info->miscdev);
	kfree(eerom_info);
	return 0;
}


        
// private data
struct _eerom_private c08_private = {
	.name = "c08 e2prom i2c interface",
	.volum = 8,
	.version = 0.1,	
};
struct _eerom_private c32_private = {
	.name = "c08 e2prom i2c interface",
	.volum = 32,
	.version = 0.1,	
};
// connect private data with eerom_id_table
static const struct i2c_device_id eerom_id_table[] = {
    [0] = {
        .name = "c08",
        .driver_data = (kernel_ulong_t)&c08_private,
    },
    [1] = {
        .name = "c32",
        .driver_data = (kernel_ulong_t)&c32_private,
    },
};


struct i2c_driver errom_i2c_drv = {
	.probe = eerom_i2c_probe,
	.remove = eerom_i2c_remove,
	.driver = {
		.name = "demo_errom_i2c_drv", 
	},
	.id_table = eerom_id_table,
};

static int __init usr_i2c_init(void)
{
    return (i2c_add_driver(&errom_i2c_drv));
}
static void __exit uer_i2c_exit(void)
{
    i2c_del_driver(&errom_i2c_drv);
}

module_init(usr_i2c_init);
module_exit(uer_i2c_exit);

//module_param(node_major, int, S_IRUGO);

MODULE_AUTHOR("QUAN");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("test i2c device driver");

















#if 0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/miscdevice.h>

#include <asm/uaccess.h>

//设计不同设备存在差异化的数据
//设计一个驱动的私有数据--自由定义
struct at24_private{
	char *name;//描述语句
	unsigned int volum; //容量
	int version;//版本
};


//设计一个全局的设备对象
struct at24_global{
	struct i2c_client *client;//用于记录probe中client
	struct miscdevice miscdev;
	struct at24_private *drv_private;
};

struct at24_global *at24_dev;


//自己去编写类似i2c_master_recv/i2c_master_send方法
int at24_i2c_e2prom_send(struct i2c_client *client, char *buf, int size)
{	
	int ret;
	struct i2c_adapter *adapter = client->adapter;

	struct i2c_msg msg;
	msg.addr = client->addr; //消息是发送给哪个从设备
	msg.flags = 0 ;//写
	msg.len = size;
	msg.buf = buf;
	
	//参数1--适配器，来自于i2c client
	//参数2--发送的数据包
	//参数3--发送的数据包的个数
	ret = i2c_transfer(adapter, &msg, 1);

	return ret==1?size:ret;
	
}

int at24_i2c_e2prom_recv(struct i2c_client *client, char *buf, int size)
{
	int ret;
	struct i2c_adapter *adapter = client->adapter;

	struct i2c_msg msg;
	msg.addr = client->addr; //消息是发送给哪个从设备
	msg.flags = 1 ;//读
	msg.len = size;
	msg.buf = buf;
	
	//参数1--适配器，来自于i2c client
	//参数2--发送的数据包
	//参数3--发送的数据包的个数
	//返回值--正确为消息的个数， 错误为负数
	ret = i2c_transfer(adapter, &msg, 1);

	return ret==1?size:ret;
}


int at24_i2c_drv_open(struct inode *inode, struct file *filp)
{
	printk("-------^_^-----%s---\n", __FUNCTION__);
	return 0;
}

//应用 read(fd, buf, size);
ssize_t at24_i2c_drv_read(struct file *filp, char __user *buf, size_t count, loff_t *fpos)
{
	printk("-------^_^-----%s---\n", __FUNCTION__);
	int ret;

	if(count<=0 || count>at24_dev->drv_private->volum)
		return -EINVAL;

	char *temp_buf = kzalloc(count, GFP_KERNEL);
	if(temp_buf == NULL)
	{
		printk("kzalloc error\n");
		return -ENOMEM;
	}

	// 1,从e2prom中获取数据
	//i2c_master_recv(const struct i2c_client * client,char * buf,int count)
	ret = at24_i2c_e2prom_recv(at24_dev->client, temp_buf, count);
	if(ret < 0)
	{
		printk("at24_i2c_e2prom_recv error\n");
		goto err_free;
	}

	// 2，将数据交给用户
	ret = copy_to_user(buf, temp_buf, count);
	if(ret > 0)
	{
		printk("copy_to_user error\n");
		ret = -EFAULT;
		goto err_free;
	}

	kfree(temp_buf);
	
	return count;

err_free:
	kfree(temp_buf);
	return ret;
}

// write(fd, buf, size);
ssize_t at24_i2c_drv_write(struct file *filp, const char __user *buf, size_t count, loff_t *fpos)
{
	printk("-------^_^-----%s---\n", __FUNCTION__);

	int ret;

	if(count<=0 || count>at24_dev->drv_private->volum)
		return -EINVAL;

	char *temp_buf = kzalloc(count, GFP_KERNEL);
	if(temp_buf == NULL)
	{
		printk("kzalloc error\n");
		return -ENOMEM;
	}
	
	// 1，从用户空间获取数据
	ret = copy_from_user(temp_buf, buf, count);
	if(ret > 0)
	{
		printk("copy_to_user error\n");
		ret = -EFAULT;
		goto err_free;
	}
	// 2, 将数据传输给e2prom
	//i2c_master_send(const struct i2c_client * client,const char * buf,int count)
	ret = at24_i2c_e2prom_send(at24_dev->client, temp_buf, count);
	if(ret < 0)
	{
		printk("at24_i2c_e2prom_send error\n");
		goto err_free;
	}

	kfree(temp_buf);
	
	return count;


err_free:
	kfree(temp_buf);
	return ret;
}

int at24_i2c_drv_close(struct inode *inode, struct file *filp)
{
	printk("-------^_^-----%s---\n", __FUNCTION__);

	
	
	return 0;
}


const struct file_operations  at24_fops = {
	.open = at24_i2c_drv_open,
	.write = at24_i2c_drv_write,
	.read = at24_i2c_drv_read,
	.release = at24_i2c_drv_close,
};

int at24_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret;

	//可以通过id获取到不同设备的数据
	struct at24_private *p = (struct at24_private *)id->driver_data;
	printk("id->name = %s\n", id->name);
	printk("id->driver_data : name = %s\n", p->name);
	printk("id->driver_data : volum = %d\n", p->volum);
	printk("id->driver_data : version = 0x%x\n", p->version);


	at24_dev = kzalloc(sizeof(struct at24_global), GFP_KERNEL);

	// 0, 记录当前匹配成功之后的i2c client
	at24_dev->client = client;

	at24_dev->drv_private = (struct at24_private *)id->driver_data;
 
	//一个函数搞定:1,申请主设备号 2，创建设备节点 4, 实现fops
	at24_dev->miscdev.minor = 179;//MISC_DYNAMIC_MINOR ; //动态指定次设备号--用于创建设备节点
	at24_dev->miscdev.name ="at24_dev";//用于指定设备节点的名字
	at24_dev->miscdev.fops = &at24_fops ;//文件操作对象
	
	//misc_register默认自带一个主设备号10,只需要一个次设备号
	ret = misc_register(&at24_dev->miscdev);
	
	// 4， 硬件初始化--通过i2c_transfer()
	//因为e2prom上电就能正常工作，不需要做一些特殊初始化
	
	return 0;
}

int at24_i2c_remove(struct i2c_client *client)
{
	
	misc_deregister(&at24_dev->miscdev);
	kfree(at24_dev);
	return 0;
}



struct at24_private c02_private = {
	.name = "at24c02 e2prom i2c interface",
	.volum = 256,
	.version = 0x007,	
};

struct at24_private c04_private = {
	.name = "at24c04 e2prom i2c interface",
	.volum = 512,
	.version = 0x007,	
};

struct at24_private c08_private = {
	.name = "at24c08 e2prom i2c interface",
	.volum = 1024,
	.version = 0x007,	
};

//私有数据的作用，就是统一驱动针对不同的设备，能够利用差异化的数据去做一些处理
const struct i2c_device_id at24_id_table[] = {
		{"at24c02a", (kernel_ulong_t)&c02_private},
		{"at24c04a", (kernel_ulong_t)&c04_private},
		{"at24c08a", (kernel_ulong_t)&c08_private},
};


struct i2c_driver at24_i2c_drv = {
	.probe = at24_i2c_probe,
	.remove = at24_i2c_remove,
	.driver = {
		.name = "at24_i2c_drv", //一定要初始化，这个名字不是用匹配
			// /sys/bus/i2c/drivers/at24_i2c_drv
	},
	.id_table = at24_id_table,

};

static int __init at24_i2c_drv_init(void)
{

	//注册i2c driver
	return i2c_add_driver(&at24_i2c_drv);
}

static void __exit at24_i2c_drv_exit(void)
{
	i2c_del_driver(&at24_i2c_drv);
}

module_init(at24_i2c_drv_init);
module_exit(at24_i2c_drv_exit);
MODULE_LICENSE("GPL");

#endif

