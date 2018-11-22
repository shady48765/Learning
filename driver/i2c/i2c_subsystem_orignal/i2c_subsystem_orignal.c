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
	struct miscdevice *miscdev;                  // using misc device to allocate device node
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
    eerom_info->miscdev->minor = MISC_DYNAMIC_MINOR;
    eerom_info->miscdev->name = USR_DEV_NAME;
    eerom_info->miscdev->fops = &eerom_fops;

	ret = misc_register(eerom_info->miscdev);
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
	
	misc_deregister(eerom_info->miscdev);
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

