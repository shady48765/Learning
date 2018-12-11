#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/bitops.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/byteorder/generic.h>
#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/of_irq.h>
#include <linux/dma-mapping.h>

#include <linux/device.h> 
#include <linux/cdev.h> 
#include <linux/fs.h> 
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/stat.h>
#include <linux/wakelock.h>
#include <linux/timer.h>
#include <mach/upmu_sw.h>

#include "tpd.h"
#include "upmu_common.h"
#include "mt_boot_common.h"

//#include "GSLxxx_xxx_xx_xx.h"

// added by elink_phil start <<<
#include <elink_tp.h>

#include <gsl_common.h>
#include <configs/SILEAD_COMPATIBILITY.h>

extern void init_tp_power(struct device *dev, char *name);
extern void tp_power_on(int on);

extern void elink_tp_get_config(u8 *r_buf, const struct fw_data **ptr_fw, unsigned int *source_len, unsigned int **data_id);
extern void elink_tp_misc_operation(struct i2c_client *client, int (*get_tp_resolution)(struct i2c_client *client, u16 reg));
extern void elink_tp_remap(int *x, int *y);

const struct fw_data *g_ptr_fw = GSL_COMPAT_FW;
unsigned int g_source_len = ARRAY_SIZE(GSL_COMPAT_FW);
unsigned int *g_config_data_id;
// added end >>>

static const struct of_device_id tpd_of_match[] = {
	{.compatible = "mediatek,silead_touch"},
	{},
};

//#define PEN_Adjust_Freq
//#define TPD_PROXIMITY
//#define HIGH_SPEED_I2C
//#define FILTER_POINT
#define GSL_NOID_VERSION
#define GSLX680_NAME	"gslX680"
#define GSLX680_ADDR	0x40
#define MAX_FINGERS	  	10
#define MAX_CONTACTS	10
#define DMA_TRANS_LEN	0x20
#define SMBUS_TRANS_LEN	0x01
#define GSL_PAGE_REG		0xf0

#ifdef FILTER_POINT
#define FILTER_MAX	9
#endif

// added by heliang for tp ps start <<<
#ifdef CONFIG_TP_PROXIMITY_SUPPORT 
//#include <sensors_io.h>
//#include <hwmsensor.h>
//#include <hwmsen_dev.h>
#include "inc/aal_control.h"
#include "hwmsensor.h"
#include "sensors_io.h"
#include "hwmsen_helper.h"
#include <linux/io.h>
#include <linux/wakelock.h>
#include <inc/alsps.h>
#include <linux/types.h>
#include <linux/ioctl.h>

static char TPD_PROC_DISABLE = 0;

static u8 tpd_proximity_flag = 0; //flag whether start alps
static u8 tpd_proximity_detect = 1;//0-->close ; 1--> far away
//static struct wake_lock ps_lock;
//static u8 gsl_psensor_data[8]={0};
//static u8 tdp_proximity_enabled = 1;
extern int ps_flush_report(void);
static int ps_flush(void);
struct gsl_priv {
	struct work_struct eint_work;
	ulong enable;		/*enable mask */
	ulong pending_intr;	/*pending interrupt */

	/*early suspend */
#if defined(CONFIG_HAS_EARLYSUSPEND)
	struct early_suspend early_drv;
#endif
	bool ps_flush;

};
static struct gsl_priv *gsl_obj;
enum {
	CMC_BIT_ALS = 1,
	CMC_BIT_PS = 2,
} CMC_BIT;
#endif
// added end >>>

#define TP_POWER "vtouch"//#define TP_POWER "VGP1" //MT6328_POWER_LDO_VGP1

int TOUCH_IRQ;
u8 int_type = 0;

#ifdef TPD_PROC_DEBUG
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#if 0
static struct proc_dir_entry *gsl_config_proc = NULL;
#endif
#define GSL_CONFIG_PROC_FILE "gsl_config"
#define CONFIG_LEN 31
static char gsl_read[CONFIG_LEN];
static u8 gsl_data_proc[8] = {0};
static u8 gsl_proc_flag = 0;
#endif

//static unsigned int GSL_TP_ID_USED = 0;

static int tpd_flag = 0;
static int tpd_halt=0;
static char eint_flag = 0;
extern struct tpd_device *tpd;
static struct i2c_client *i2c_client = NULL;
static struct task_struct *thread = NULL;
#ifdef GSL_MONITOR
static struct delayed_work gsl_monitor_work;
static struct workqueue_struct *gsl_monitor_workqueue = NULL;
static u8 int_1st[4] = {0};
static u8 int_2nd[4] = {0};
static char bc_counter = 0;
static char b0_counter = 0;
static char i2c_lock_flag = 0;
#endif

static uint32_t id_sign[MAX_CONTACTS+1] = {0};
static uint8_t id_state_flag[MAX_CONTACTS+1] = {0};
static uint8_t id_state_old_flag[MAX_CONTACTS+1] = {0};
static uint16_t x_old[MAX_CONTACTS+1] = {0};
static uint16_t y_old[MAX_CONTACTS+1] = {0};
static uint16_t x_new = 0;
static uint16_t y_new = 0;

static DECLARE_WAIT_QUEUE_HEAD(waiter);
extern void mt_eint_unmask(unsigned int line);
extern void mt_eint_mask(unsigned int line);

#ifdef GSL_DEBUG 
#define print_info(fmt, args...)   \
        do{                              \
                printk(fmt, ##args);     \
        }while(0)
#else
#define print_info(fmt, args...)
#endif

#if (defined(TPD_WARP_START) && defined(TPD_WARP_END))
static int tpd_wb_start_local[TPD_WARP_CNT] = TPD_WARP_START;
static int tpd_wb_end_local[TPD_WARP_CNT]   = TPD_WARP_END;
#endif
#if (defined(TPD_HAVE_CALIBRATION) && !defined(TPD_CUSTOM_CALIBRATION))
static int tpd_calmat_local[8]     = TPD_CALIBRATION_MATRIX;
static int tpd_def_calmat_local[8] = TPD_CALIBRATION_MATRIX;
#endif

static struct mutex gsl_i2c_lock;
#define I2C_TRANS_SPEED 400	//100 khz or 400 khz
/*****************************************************************************
Prototype    	: gsl_read_interface
Description  	: gsl chip tranfer function
Input        	: struct i2c_client *client
u8 reg
u8 *buf
u32 num
Output		: None
Return Value 	: static

 *****************************************************************************/
static int gsl_read_interface(struct i2c_client *client,
        u8 reg, u32 num, u8 *buf)
{
	int err = 0;
	int i;
	u8 temp = reg;
	mutex_lock(&gsl_i2c_lock);
	if(temp < 0x80)
	{
		if(temp==0x7c){
			temp =0x78;
			i2c_master_send(client,&temp,1);
			err = i2c_master_recv(client,&buf[0],4);
			temp = 0x7c;
		}else{
			i2c_master_send(client,&temp,1);
			err = i2c_master_recv(client,&buf[0],4);
		}

	}
	for(i=0;i<num;)
	{
		temp = reg + i;
		i2c_master_send(client,&temp,1);
		if((i+8)<num)
			err = i2c_master_recv(client,(buf+i),8);
		else
			err = i2c_master_recv(client,(buf+i),(num-i));
		i+=8;
	}
	mutex_unlock(&gsl_i2c_lock);

	return err;
}

/*****************************************************************************
Prototype    : gsl_write_interface
Description  : gsl chip tranfer function
Input        : struct i2c_client *client
const u8 reg
u8 *buf
u32 num
Output       : None
Return Value : static

 *****************************************************************************/
static int gsl_write_interface(struct i2c_client *client,
        const u8 reg, u32 num, u8 *buf)
{
	struct i2c_msg xfer_msg[1] = {{0}};
	int err;
	u8 tmp_buf[num + 1];
//	u8 retry = 0;

	tmp_buf[0] = reg;
	memcpy(tmp_buf + 1, buf, num);

	xfer_msg[0].addr = client->addr;
	xfer_msg[0].len = num + 1;
	xfer_msg[0].flags = client->flags & I2C_M_TEN;
	xfer_msg[0].buf = tmp_buf;

	mutex_lock(&gsl_i2c_lock);
	err = i2c_transfer(client->adapter, xfer_msg, 1);
	mutex_unlock(&gsl_i2c_lock);

	return 0;
}

static void startup_chip(struct i2c_client *client)
{
	char write_buf = 0x00;

	gsl_write_interface(client, 0xe0, 1, &write_buf); 	

	gsl_DataInit(g_config_data_id);

	msleep(10);
}

#ifdef PEN_Adjust_Freq
//struct mutex pen_mutex;
//spinlock_t spin_locked;//自旋锁变量定义
static int GSL_Adjust_Freq(struct i2c_client *client)
{
	 //unsigned int mode = 0;
	//u64 time_freq,time_start,time_end;
	static unsigned int cpu_start, cpu_end, ret,real_time;
	u8 buf[4];
	struct timeval time_start,time_end;
	printk("gsl pen test\n");
		buf[3] = 0x01;
		buf[2] = 0xfe;
		buf[1] = 0x02;
		buf[0] = 0x00;
		i2c_smbus_write_i2c_block_data(client,0xf0,4,buf);
		printk("--gsl pen test 4--\n");
		buf[3] = 0x00;
		buf[2] = 0x00;
		buf[1] = 0x00;
		buf[0] = 0x00;	
		i2c_smbus_write_i2c_block_data(client,0x0c,4,buf);
	    printk("--gsl pen test 3--\n");
		buf[3] = 0x01;
		buf[2] = 0xfe;
		buf[1] = 0x02;
		buf[0] = 0x00;
		i2c_smbus_write_i2c_block_data(client,0xf0,4,buf);
	    printk("--gsl pen test 2--\n");
		buf[3] = 0xff;
		buf[2] = 0xff;
		buf[1] = 0xff;
		buf[0] = 0xff;	
		i2c_smbus_write_i2c_block_data(client,0x04,4,buf);
	   printk("--gsl pen test 1--\n");
		buf[3] = 0x01;
		buf[2] = 0xfe;
		buf[1] = 0x02;
		buf[0] = 0x00;
		i2c_smbus_write_i2c_block_data(client,0xf0,4,buf);
		buf[3] = 0x00;
		buf[2] = 0x00;
		buf[1] = 0x00;
		buf[0] = 0x09;
		i2c_smbus_write_i2c_block_data(client,0x08,4,buf);
        printk("--gsl pen test 5--\n");
	//	spin_lock(&spin_locked);
	//	mutex_lock(&pen_mutex);
		mdelay(200);
		buf[3] = 0x01;
		buf[2] = 0xfe;
		buf[1] = 0x02;
		buf[0] = 0x00;
		i2c_smbus_write_i2c_block_data(client, 0xf0, 4, buf);
		 printk("--gsl pen test 11--\n");
		i2c_smbus_read_i2c_block_data(client, 0, 4, buf);
		i2c_smbus_read_i2c_block_data(client, 0, 4, buf);
		printk("--gsl pen test 12--\n");
		cpu_start = (buf[3]<<24) + (buf[2]<<16) + (buf[1]<<8) + buf[0];
		printk(" gsl cpu_start = %d\n",cpu_start);
		printk("--gsl pen test 6--\n");
 		do_gettimeofday(&time_start);
	//	spin_unlock(&spin_locked);
	//	mutex_unlock(&pen_mutex);
		// start count time
		mdelay(1200);
		
		//spin_lock(&spin_locked);
		//	mutex_lock(&pen_mutex);
		buf[3] = 0x01;
		buf[2] = 0xfe;
		buf[1] = 0x02;
		buf[0] = 0x00;
		i2c_smbus_write_i2c_block_data(client, 0xf0, 4, buf);
		i2c_smbus_read_i2c_block_data(client, 0, 4, buf);
		i2c_smbus_read_i2c_block_data(client, 0, 4, buf);
		cpu_end = (buf[3]<<24) + (buf[2]<<16) + (buf[1]<<8) + buf[0];
		do_gettimeofday(&time_end);
	//	spin_unlock(&spin_locked);
	//	mutex_unlock(&pen_mutex);
		printk("--gsl pen test 7--\n");
		real_time = ((time_end.tv_sec-time_start.tv_sec)*10000+(time_end.tv_usec-time_start.tv_usec)/100);
		if(real_time > 10000){ // 1 s 以上
			ret = (u32)((cpu_start - cpu_end )*100/ real_time)*0x1000/9245;
			//芯片计数器计算出来的时间0x1000/9245为换算系数
			if(ret >= 0x1000/2 && ret <= 0x1000*2){//频率偏差应该不会太大
				buf[3] = 0x00;
				buf[2] = 0x00;
				buf[1] = 0x00;
				buf[0] = 0x03;
				i2c_smbus_write_i2c_block_data(client,0xf0,4,buf);
				buf[3] = (u8)((ret>>24)&0xff);
				buf[2] = (u8)((ret>>16)&0xff);
				buf[1] = (u8)((ret>>8)&0xff);
				buf[0] = (u8)(ret&0xff);
				i2c_smbus_write_i2c_block_data(client,0x7c,4,buf); //把校正结果写入
			    printk("--gsl pen test 9--\n");
			}
			else{

				//矫正失败重新矫正
				return -1;
			}
		}
		return 0;
}
#endif
static void reset_chip(struct i2c_client *client)
{
	u8 write_buf[4]	= {0};

	tpd_gpio_output(GTP_RST_PORT, 0);
	msleep(10);

	tpd_gpio_output(GTP_RST_PORT, 1);
	msleep(10); 

	write_buf[0] = 0x04;
	gsl_write_interface(client, 0xe4, 1, &write_buf[0]); 	
	msleep(10);

	write_buf[0] = 0x00;
	write_buf[1] = 0x00;
	write_buf[2] = 0x00;
	write_buf[3] = 0x00;
	gsl_write_interface(client, 0xbc, 4, write_buf); 	
	msleep(10);
}

static void clr_reg(struct i2c_client *client)
{
	char write_buf[4]	= {0};

    tpd_gpio_output(GTP_RST_PORT, 0);
	msleep(10); 	

	tpd_gpio_output(GTP_RST_PORT, 1);
	msleep(10); 

	write_buf[0] = 0x03;
	gsl_write_interface(client, 0x80, 1, &write_buf[0]); 	
	msleep(5);
	
	write_buf[0] = 0x04;
	gsl_write_interface(client, 0xe4, 1, &write_buf[0]); 	
	msleep(5);

	write_buf[0] = 0x00;
	gsl_write_interface(client, 0xe0, 1, &write_buf[0]); 	
	msleep(20);
}

static void gsl_load_fw(struct i2c_client *client);

static void cfg_adjust(struct i2c_client *client)
{
	u8 read_buf[4]  = {0};
	int ret = 0;
	
	printk("=============gsl_load_cfg_adjust check start==============\n");
	msleep(500);
	ret = gsl_read_interface(client,0xb8, sizeof(read_buf), read_buf);
    printk("fuc:cfg_adjust, b8: %x %x %x %x...ret = %d\n", read_buf[3], read_buf[2], read_buf[1], read_buf[0], ret);

	elink_tp_get_config(read_buf, &g_ptr_fw, &g_source_len, &g_config_data_id);

	clr_reg(client);
	reset_chip(client);

	gsl_load_fw(client);
	startup_chip(client);
	reset_chip(client);
	startup_chip(client);
}

static void gsl_load_fw(struct i2c_client *client)
{
	unsigned char buf[SMBUS_TRANS_LEN*4] = {0};
	unsigned char reg = 0, send_flag = 1, cur = 0;
	
	unsigned int source_line = 0;
	unsigned int source_len ;
	const struct fw_data *ptr_fw;

	ptr_fw = g_ptr_fw;
	source_len = g_source_len;

	printk("=============gsl_load_fw start==============\n");

	for (source_line = 0; source_line < source_len; source_line++) 
	{
		if(1 == SMBUS_TRANS_LEN)
		{
			reg = ptr_fw[source_line].offset;
			memcpy(&buf[0], &ptr_fw[source_line].val, 4);
			gsl_write_interface(client, reg, 4, buf); 	
		}
		else
		{
			/* init page trans, set the page val */
			if (GSL_PAGE_REG == ptr_fw[source_line].offset)
			{
				buf[0] = (char)(ptr_fw[source_line].val & 0x000000ff);
				gsl_write_interface(client, GSL_PAGE_REG, 1, &buf[0]); 	
				send_flag = 1;
			}
			else 
			{
				if (1 == send_flag % (SMBUS_TRANS_LEN < 0x08 ? SMBUS_TRANS_LEN : 0x08))
					reg = ptr_fw[source_line].offset;

				memcpy(&buf[cur], &ptr_fw[source_line].val, 4);
				cur += 4;

				if (0 == send_flag % (SMBUS_TRANS_LEN < 0x08 ? SMBUS_TRANS_LEN : 0x08)) 
				{
					gsl_write_interface(client, reg, SMBUS_TRANS_LEN*4, buf); 	
					cur = 0;
				}

				send_flag++;

			}
		}
	}

	printk("=============gsl_load_fw end==============\n");
}

static int test_i2c(struct i2c_client *client)
{
	char read_buf = 0;
	char write_buf = 0x12;
	int ret, rc = 1;
	
	ret = gsl_read_interface( client, 0xf0, 1, &read_buf );
	if  (ret  < 0)  
    		rc --;
	else
		printk("I read reg 0xf0 is %x\n", read_buf);

	msleep(2);
	ret = gsl_write_interface( client, 0xf0, 1, &write_buf );
	if(ret  >=  0 )
		printk("I write reg 0xf0 0x12\n");
	
	msleep(2);
	ret = gsl_read_interface( client, 0xf0, 1, &read_buf );
	if(ret <  0 )
		rc --;
	else
		printk("I read reg 0xf0 is 0x%x\n", read_buf);

	return rc;
}
static void init_chip_without_rst(struct i2c_client *client)
{
	clr_reg(client);
	reset_chip(client);

	gsl_load_fw(client);
	
	startup_chip(client);
}

static int init_chip(struct i2c_client *client)
{
	tpd_gpio_output(GTP_RST_PORT, 0);
	msleep(20);
	tpd_gpio_output(GTP_RST_PORT, 1);
	msleep(20);

	init_chip_without_rst(client);

	return 0;
}

// added by heliang for tp ps start <<<
#ifdef CONFIG_TP_PROXIMITY_SUPPORT
/* if use  this typ of enable , Gsensor should report inputEvent(x, y, z ,stats, div) to HAL */

static int tpd_get_ps_value(void)
{
	return tpd_proximity_detect;//send to OS to controll backlight on/off
}

static int tpd_enable_ps(int enable)
{
	u8 buf[4];
	if (enable) {
		//wake_lock(&ps_lock);

		buf[3] = 0x00;
		buf[2] = 0x00;
		buf[1] = 0x00;
		buf[0] = 0x4;
		i2c_smbus_write_i2c_block_data(i2c_client, 0xf0, 4, buf);
		buf[3] = 0x0;
		buf[2] = 0x0;
		buf[1] = 0x0;
		buf[0] = 0x2;
		i2c_smbus_write_i2c_block_data(i2c_client, 0x00, 4, buf);
		tpd_proximity_flag = 1;
		//add alps of function
		printk("tpd-ps function is on\n");
	}
	else 
	{
		tpd_proximity_flag = 0;
		//wake_unlock(&ps_lock);

		buf[3] = 0x00;
		buf[2] = 0x00;
		buf[1] = 0x00;
		buf[0] = 0x4;

		i2c_smbus_write_i2c_block_data(i2c_client, 0xf0, 4, buf);
		buf[3] =0x0;
		buf[2] = 0x0;
		buf[1] = 0x0;
		buf[0] = 0x0;
		i2c_smbus_write_i2c_block_data(i2c_client, 0x00, 4, buf);	
		printk("tpd-ps function is off\n");
	}
	return 0;
}
static int ps_open_report_data(int open)
{
	/* should queuq work to report event if  is_report_input_direct=true */
	return 0;
}

/* if use  this typ of enable , Gsensor only enabled but not report inputEvent to HAL */

static int ps_enable_nodata(int en)
{
	//return 0;
	//int res = 0;
#ifdef CUSTOM_KERNEL_SENSORHUB
	SCP_SENSOR_HUB_DATA req;
	int len;
#endif				/* #ifdef CUSTOM_KERNEL_SENSORHUB */

	printk("gsl_obj ps enable value = %d\n", en);
	if (en) {
		tpd_enable_ps(1);
		set_bit(CMC_BIT_PS, &gsl_obj->enable);
	} else {
		tpd_enable_ps(0);
		clear_bit(CMC_BIT_PS, &gsl_obj->enable);
	}
	return 0;
}

static int ps_set_delay(u64 ns)
{
	return 0;
}

static int ps_get_data(int *value, int *status)
{
//	int err = 0;
	/* #ifdef CUSTOM_KERNEL_SENSORHUB */
	//mutex_lock(&Ltr559_lock);
	*value = tpd_get_ps_value();
	*status = SENSOR_STATUS_ACCURACY_MEDIUM;

//#endif				/* #ifdef CUSTOM_KERNEL_SENSORHUB */
	//mutex_unlock(&Ltr559_lock);
	return 0;
}

static int ps_batch(int flag, int64_t samplingPeriodNs, int64_t maxBatchReportLatencyNs)
{
	return 0;
}

static int ps_flush(void)
{
	return ps_flush_report();
}
#endif
// added end >>>

static void check_mem_data(struct i2c_client *client)
{
	char read_buf[4]  = {0};
	
	msleep(30);
	gsl_read_interface(client,0xb0, sizeof(read_buf), read_buf);
	
	if (read_buf[3] != 0x5a || read_buf[2] != 0x5a || read_buf[1] != 0x5a || read_buf[0] != 0x5a)
	{
		printk("#########check mem read 0xb0 = %x %x %x %x #########\n", read_buf[3], read_buf[2], read_buf[1], read_buf[0]);
		init_chip(client);
	}
}

#ifdef TPD_PROC_DEBUG
static int char_to_int(char ch)
{
    if(ch>='0' && ch<='9')
        return (ch-'0');
    else
        return (ch-'a'+10);
}

static int gsl_config_read_proc(struct seq_file *m,void *v)
{
	//char *ptr = page;
	char temp_data[5] = {0};
	unsigned int tmp=0;
	unsigned int *ptr_fw;
	
	if('v'==gsl_read[0]&&'s'==gsl_read[1])
	{
#ifdef GSL_NOID_VERSION
		tmp=gsl_version_id();
#else 
		tmp=0x20121215;
#endif
		seq_printf(m,"version:%x\n",tmp);
	}
	else if('r'==gsl_read[0]&&'e'==gsl_read[1])
	{
		if('i'==gsl_read[3])
		{
#ifdef GSL_NOID_VERSION 

	ptr_fw = g_config_data_id;
 
	        tmp=(gsl_data_proc[5]<<8) | gsl_data_proc[4];
#endif
		}
		else 
		{
			gsl_write_interface(i2c_client,0xf0,4,&gsl_data_proc[4]);
			if(gsl_data_proc[0] < 0x80)
				gsl_read_interface(i2c_client,gsl_data_proc[0],4,temp_data);
			gsl_read_interface(i2c_client,gsl_data_proc[0],4,temp_data);

			seq_printf(m,"offset : {0x%02x,0x",gsl_data_proc[0]);
			seq_printf(m,"%02x",temp_data[3]);
			seq_printf(m,"%02x",temp_data[2]);
			seq_printf(m,"%02x",temp_data[1]);
			seq_printf(m,"%02x};\n",temp_data[0]);
		}
	}
	return 0;
}
static ssize_t gsl_config_write_proc(struct file *file, const char *buffer, size_t count, loff_t *data)
{
	u8 buf[8] = {0};
	char temp_buf[CONFIG_LEN];
	char *path_buf;
	int tmp = 0;
	int tmp1 = 0;
	print_info("[tp-gsl][%s] \n",__func__);
	if(count > 512)
	{
		print_info("size not match [%d:%ld]\n", CONFIG_LEN, (long int)count);
        return -EFAULT;
	}
	path_buf=kzalloc(count,GFP_KERNEL);
	if(!path_buf)
	{
		printk("alloc path_buf memory error \n");
	}	
	if(copy_from_user(path_buf, buffer, count))
	{
		print_info("copy from user fail\n");
		goto exit_write_proc_out;
	}
	memcpy(temp_buf,path_buf,(count<CONFIG_LEN?count:CONFIG_LEN));
	print_info("[tp-gsl][%s][%s]\n",__func__,temp_buf);
	
	buf[3]=char_to_int(temp_buf[14])<<4 | char_to_int(temp_buf[15]);	
	buf[2]=char_to_int(temp_buf[16])<<4 | char_to_int(temp_buf[17]);
	buf[1]=char_to_int(temp_buf[18])<<4 | char_to_int(temp_buf[19]);
	buf[0]=char_to_int(temp_buf[20])<<4 | char_to_int(temp_buf[21]);
	
	buf[7]=char_to_int(temp_buf[5])<<4 | char_to_int(temp_buf[6]);
	buf[6]=char_to_int(temp_buf[7])<<4 | char_to_int(temp_buf[8]);
	buf[5]=char_to_int(temp_buf[9])<<4 | char_to_int(temp_buf[10]);
	buf[4]=char_to_int(temp_buf[11])<<4 | char_to_int(temp_buf[12]);
	if('v'==temp_buf[0]&& 's'==temp_buf[1])//version //vs
	{
		memcpy(gsl_read,temp_buf,4);
		printk("gsl version\n");
	}
	else if('s'==temp_buf[0]&& 't'==temp_buf[1])//start //st
	{
		gsl_proc_flag = 1;
		reset_chip(i2c_client);
	}
	else if('e'==temp_buf[0]&&'n'==temp_buf[1])//end //en
	{
		msleep(20);
		reset_chip(i2c_client);
		startup_chip(i2c_client);
		gsl_proc_flag = 0;
	}
	else if('r'==temp_buf[0]&&'e'==temp_buf[1])//read buf //
	{
		memcpy(gsl_read,temp_buf,4);
		memcpy(gsl_data_proc,buf,8);
	}
	else if('w'==temp_buf[0]&&'r'==temp_buf[1])//write buf
	{
		gsl_write_interface(i2c_client,buf[4],4,buf);
	}
#ifdef GSL_NOID_VERSION
	else if('i'==temp_buf[0]&&'d'==temp_buf[1])//write id config //
	{
		tmp1=(buf[7]<<24)|(buf[6]<<16)|(buf[5]<<8)|buf[4];
		tmp=(buf[3]<<24)|(buf[2]<<16)|(buf[1]<<8)|buf[0];

		if(tmp1>=0 && tmp1<256) {
		
		g_config_data_id[tmp1] = tmp;

		}			  		
	}
#endif
exit_write_proc_out:
	kfree(path_buf);
	return count;
}

static int gsl_server_list_open(struct inode *inode,struct file *file)
{
	return single_open(file,gsl_config_read_proc,NULL);
}

static const struct file_operations gsl_seq_fops = {
	.open = gsl_server_list_open,
	.read = seq_read,
	.release = single_release,
	.write = gsl_config_write_proc,
	.owner = THIS_MODULE,
};
#endif

#ifdef FILTER_POINT
static void filter_point(u16 x, u16 y , u8 id)
{
	u16 x_err =0;
	u16 y_err =0;
	u16 filter_step_x = 0, filter_step_y = 0;
	
	id_sign[id] = id_sign[id] + 1;
	if(id_sign[id] == 1)
	{
		x_old[id] = x;
		y_old[id] = y;
	}
	
	x_err = x > x_old[id] ? (x -x_old[id]) : (x_old[id] - x);
	y_err = y > y_old[id] ? (y -y_old[id]) : (y_old[id] - y);

	if( (x_err > FILTER_MAX && y_err > FILTER_MAX/3) || (x_err > FILTER_MAX/3 && y_err > FILTER_MAX) )
	{
		filter_step_x = x_err;
		filter_step_y = y_err;
	}
	else
	{
		if(x_err > FILTER_MAX)
			filter_step_x = x_err; 
		if(y_err> FILTER_MAX)
			filter_step_y = y_err;
	}

	if(x_err <= 2*FILTER_MAX && y_err <= 2*FILTER_MAX)
	{
		filter_step_x >>= 2; 
		filter_step_y >>= 2;
	}
	else if(x_err <= 3*FILTER_MAX && y_err <= 3*FILTER_MAX)
	{
		filter_step_x >>= 1; 
		filter_step_y >>= 1;
	}	
	else if(x_err <= 4*FILTER_MAX && y_err <= 4*FILTER_MAX)
	{
		filter_step_x = filter_step_x*3/4; 
		filter_step_y = filter_step_y*3/4;
	}	
	
	x_new = x > x_old[id] ? (x_old[id] + filter_step_x) : (x_old[id] - filter_step_x);
	y_new = y > y_old[id] ? (y_old[id] + filter_step_y) : (y_old[id] - filter_step_y);

	x_old[id] = x_new;
	y_old[id] = y_new;
}
#else

static void record_point(u16 x, u16 y , u8 id)
{
	u16 x_err =0;
	u16 y_err =0;

	id_sign[id]=id_sign[id]+1;
	
	if(id_sign[id]==1){
		x_old[id]=x;
		y_old[id]=y;
	}

	x = (x_old[id] + x)/2;
	y = (y_old[id] + y)/2;
		
	if(x>x_old[id]){
		x_err=x -x_old[id];
	}
	else{
		x_err=x_old[id]-x;
	}

	if(y>y_old[id]){
		y_err=y -y_old[id];
	}
	else{
		y_err=y_old[id]-y;
	}

	if( (x_err > 3 && y_err > 1) || (x_err > 1 && y_err > 3) ){
		x_new = x;     x_old[id] = x;
		y_new = y;     y_old[id] = y;
	}
	else{
		if(x_err > 3){
			x_new = x;     x_old[id] = x;
		}
		else
			x_new = x_old[id];
		if(y_err> 3){
			y_new = y;     y_old[id] = y;
		}
		else
			y_new = y_old[id];
	}

	if(id_sign[id]==1){
		x_new= x_old[id];
		y_new= y_old[id];
	}
	
}
#endif

void tpd_down( int id, int x, int y, int p) 
{

	//print_info("------tpd_down id: %d, x:%d, y:%d------ \n", id, x, y);

	elink_tp_remap(&x, &y);

	input_report_key(tpd->dev, BTN_TOUCH, 1);
	//input_report_abs(tpd->dev, ABS_MT_TOUCH_MAJOR, 1);
	input_report_abs(tpd->dev, ABS_MT_POSITION_X, x);
	input_report_abs(tpd->dev, ABS_MT_POSITION_Y, y);
	input_report_abs(tpd->dev, ABS_MT_TRACKING_ID, id); 	
	input_mt_sync(tpd->dev);

	if (FACTORY_BOOT == get_boot_mode()|| RECOVERY_BOOT == get_boot_mode())
	{
		if (tpd_dts_data.use_tpd_button)
			tpd_button(x, y, 1);
	}
}

void tpd_up(int id, int x, int y) 
{
	print_info("------tpd_up------ \n");

	input_report_key(tpd->dev, BTN_TOUCH, 0);
	input_mt_sync(tpd->dev);

	if (FACTORY_BOOT == get_boot_mode()|| RECOVERY_BOOT == get_boot_mode())
	{
		if (tpd_dts_data.use_tpd_button)
			tpd_button(x, y, 0);
	}
}

static void report_data_handle(void)
{
	char touch_data[MAX_FINGERS * 4 + 4] = {0};
	char buf[4] = {0};
	unsigned char id = 0;
	char point_num = 0;
	unsigned int x, y, temp_a, temp_b, i,tmp1;

#ifdef GSL_NOID_VERSION
	struct gsl_touch_info cinfo;
#endif

// added by heliang for tp ps start <<<
#ifdef CONFIG_TP_PROXIMITY_SUPPORT
	//int err;
	struct hwm_sensor_data sensor_data;

	if (tpd_proximity_flag == 1)
	{
		i2c_smbus_read_i2c_block_data(i2c_client,0xac,4,buf);
		print_info("gslX680   buf[0] = %d buf[1] = %d,  buf[2] = %d  buf[3] = %d \n",buf[0],buf[1],buf[2],buf[3]);

		if (buf[0] == 1 && buf[1] == 0 && buf[2] == 0 && buf[3] == 0)
		{
			tpd_proximity_detect = 0;
		}
		else
		{
			tpd_proximity_detect = 1;
		}
		print_info("gslX680    ps change   tpd_proximity_detect = %d  \n",tpd_proximity_detect);
		//map and store data to hwm_sensor_data
		sensor_data.values[0] = tpd_get_ps_value();
		sensor_data.value_divide = 1;
		sensor_data.status = SENSOR_STATUS_ACCURACY_MEDIUM;
		//let up layer to know
		if(ps_report_interrupt_data(tpd_get_ps_value()))
		{
			print_info("call hwmsen_get_interrupt_data fail \n");
		}
		
	}
#endif
// added end >>>

#ifdef GSL_MONITOR
	if(i2c_lock_flag != 0)
		return;
	else
		i2c_lock_flag = 1;
#endif	

#ifdef TPD_PROC_DEBUG
    if(gsl_proc_flag == 1)
        return;
#endif

	gsl_read_interface(i2c_client, 0x80, 4, &touch_data[0]);
	point_num = touch_data[0];
	if(point_num > 0)
		gsl_read_interface(i2c_client, 0x84, 8, &touch_data[4]);
	if(point_num > 2)
		gsl_read_interface(i2c_client, 0x8c, 8, &touch_data[12]);
	if(point_num > 4)
		gsl_read_interface(i2c_client, 0x94, 8, &touch_data[20]);
	if(point_num > 6)
		gsl_read_interface(i2c_client, 0x9c, 8, &touch_data[28]);
	if(point_num > 8)
		gsl_read_interface(i2c_client, 0xa4, 8, &touch_data[36]);
	
#ifdef GSL_NOID_VERSION
	cinfo.finger_num = point_num;
	//print_info("tp-gsl  finger_num = %d\n",cinfo.finger_num);
	for(i = 0; i < (point_num < MAX_CONTACTS ? point_num : MAX_CONTACTS); i ++)
	{
		temp_a = touch_data[(i + 1) * 4 + 3] & 0x0f;
		temp_b = touch_data[(i + 1) * 4 + 2];
		cinfo.x[i] = temp_a << 8 |temp_b;
		temp_a = touch_data[(i + 1) * 4 + 1];
		temp_b = touch_data[(i + 1) * 4 + 0];
		cinfo.y[i] = temp_a << 8 |temp_b;		
		//print_info("tp-gsl  x = %d y = %d \n",cinfo.x[i],cinfo.y[i]);
	}
	cinfo.finger_num = (touch_data[3]<<24)|(touch_data[2]<<16)|
		(touch_data[1]<<8)|touch_data[0];
	gsl_alg_id_main(&cinfo);
	tmp1=gsl_mask_tiaoping();
	print_info("[tp-gsl] tmp1=%x\n",tmp1);
	if(tmp1>0&&tmp1<0xffffffff)
	{
		buf[0]=0xa;buf[1]=0;buf[2]=0;buf[3]=0;
		gsl_write_interface(i2c_client,0xf0,4,buf);
		buf[0]=(u8)(tmp1 & 0xff);
		buf[1]=(u8)((tmp1>>8) & 0xff);
		buf[2]=(u8)((tmp1>>16) & 0xff);
		buf[3]=(u8)((tmp1>>24) & 0xff);
		print_info("tmp1=%08x,buf[0]=%02x,buf[1]=%02x,buf[2]=%02x,buf[3]=%02x\n",
			tmp1,buf[0],buf[1],buf[2],buf[3]);
		gsl_write_interface(i2c_client,0x8,4,buf);
	}
	point_num = cinfo.finger_num;
#endif

	for(i = 1 ;i <= MAX_CONTACTS; i ++)
	{
		if(point_num == 0)
			id_sign[i] = 0;	
		id_state_flag[i] = 0;
	}
	for(i = 0; i < (point_num < MAX_FINGERS ? point_num : MAX_FINGERS); i ++)
	{
	#ifdef GSL_NOID_VERSION
		id = cinfo.id[i];
		x =  cinfo.x[i];
		y =  cinfo.y[i];
	#else
		id = touch_data[(i + 1) * 4 + 3] >> 4;
		temp_a = touch_data[(i + 1) * 4 + 3] & 0x0f;
		temp_b = touch_data[(i + 1) * 4 + 2];
		x = temp_a << 8 |temp_b;
		temp_a = touch_data[(i + 1) * 4 + 1];
		temp_b = touch_data[(i + 1) * 4 + 0];
		y = temp_a << 8 |temp_b;	
	#endif
	
		if(1 <= id && id <= MAX_CONTACTS)
		{
		#ifdef FILTER_POINT
			filter_point(x, y ,id);
		#else
			record_point(x, y , id);
		#endif
			tpd_down(id, x_new, y_new, 10);
			id_state_flag[id] = 1;
		}
	}
	for(i = 1; i <= MAX_CONTACTS; i ++)
	{	
		if( (0 == point_num) || ((0 != id_state_old_flag[i]) && (0 == id_state_flag[i])) )
		{
			id_sign[i]=0;
		}
		id_state_old_flag[i] = id_state_flag[i];
	}			
	if(0 == point_num)
	{
		tpd_up(id, x_new, y_new);
	}
	input_sync(tpd->dev);
#ifdef GSL_MONITOR
	i2c_lock_flag = 0;
#endif
}

#ifdef GSL_MONITOR
static void gsl_monitor_worker(struct work_struct *data)
{
//	u8 write_buf[4] = {0};
	u8 read_buf[4]  = {0};
	u8 init_chip_flag = 0;
	
	print_info("----------------gsl_monitor_worker-----------------\n");	

	if(i2c_lock_flag != 0)
		goto queue_monitor_work;
	else
		i2c_lock_flag = 1;
	
	gsl_read_interface(i2c_client, 0xb0, 4, read_buf);
	if(read_buf[3] != 0x5a || read_buf[2] != 0x5a || read_buf[1] != 0x5a || read_buf[0] != 0x5a)
		b0_counter ++;
	else
		b0_counter = 0;

	if(b0_counter > 1)
	{
		printk("======read 0xb0: %x %x %x %x ======\n",read_buf[3], read_buf[2], read_buf[1], read_buf[0]);
		init_chip_flag = 1;
		b0_counter = 0;
	}

	gsl_read_interface(i2c_client, 0xb4, 4, read_buf);	
	
	int_2nd[3] = int_1st[3];
	int_2nd[2] = int_1st[2];
	int_2nd[1] = int_1st[1];
	int_2nd[0] = int_1st[0];
	int_1st[3] = read_buf[3];
	int_1st[2] = read_buf[2];
	int_1st[1] = read_buf[1];
	int_1st[0] = read_buf[0];

	if (int_1st[3] == int_2nd[3] && int_1st[2] == int_2nd[2] &&int_1st[1] == int_2nd[1] && int_1st[0] == int_2nd[0]) 
	{
		printk("======int_1st: %x %x %x %x , int_2nd: %x %x %x %x ======\n",int_1st[3], int_1st[2], int_1st[1], int_1st[0], int_2nd[3], int_2nd[2],int_2nd[1],int_2nd[0]);
		init_chip_flag = 1;
	}

	gsl_read_interface(i2c_client, 0xbc, 4, read_buf);
	if(read_buf[3] != 0 || read_buf[2] != 0 || read_buf[1] != 0 || read_buf[0] != 0)
		bc_counter ++;
	else
		bc_counter = 0;

	if(bc_counter > 1)
	{
		printk("======read 0xbc: %x %x %x %x ======\n",read_buf[3], read_buf[2], read_buf[1], read_buf[0]);
		init_chip_flag = 1;
		bc_counter = 0;
	}
	
	if(init_chip_flag)
		init_chip(i2c_client);
	
	i2c_lock_flag = 0;
	
queue_monitor_work:	
	queue_delayed_work(gsl_monitor_workqueue, &gsl_monitor_work, 100);
}
#endif

static irqreturn_t tpd_eint_interrupt_handler(int irq, void *dev_id)
{
	TPD_DEBUG_PRINT_INT;

	eint_flag = 1;
	tpd_flag=1; 
	wake_up_interruptible(&waiter);
	return IRQ_HANDLED;
}

static int get_tp_resolution(struct i2c_client *client, u16 reg)
{
	char write_buffer[4] = {0};
	char read_buffer[4] = {0};
	int ret;

	write_buffer[0] = 0x06;
	write_buffer[1] = 0x00;
	write_buffer[2] = 0x00;
	write_buffer[3] = 0x00;

	ret = gsl_write_interface(client, 0xf0, 4, write_buffer);
	if(ret < 0 ){
		 printk("write register to get tp width or height failed\n");
	}

	msleep(10);
	gsl_read_interface(client, reg, 4, read_buffer);
	printk("read_buffer = %d %d %d %d\n", read_buffer[0], read_buffer[1], read_buffer[2], read_buffer[3]);
	ret = (*(int*)read_buffer);

	return ret;
}

static void gsl_post_init(void)
{
	struct device_node *node;
	int ret = 0;

	ret = init_chip(i2c_client);
	if(ret < 0)
		printk("gsl init_chip failed!\n");

	check_mem_data(i2c_client);
	cfg_adjust(i2c_client);
	
#ifdef PEN_Adjust_Freq
//	spin_lock_init(&spin_locked);
//	mutex_init(&pen_mute); 
//	GSL_Adjust_Freq(client);
	GSL_Adjust_Freq(i2c_client);
#endif

	elink_tp_misc_operation(i2c_client, get_tp_resolution);

#ifdef GSL_MONITOR
	printk( "gsl_post_init () : queue gsl_monitor_workqueue\n");

	INIT_DELAYED_WORK(&gsl_monitor_work, gsl_monitor_worker);
	gsl_monitor_workqueue = create_singlethread_workqueue("gsl_monitor_workqueue");
	queue_delayed_work(gsl_monitor_workqueue, &gsl_monitor_work, 1000);
#endif

	node = of_find_matching_node(NULL, tpd_of_match);
	if (node) {
		TOUCH_IRQ = irq_of_parse_and_map(node, 0);
		ret = request_irq(TOUCH_IRQ,
				  (irq_handler_t)tpd_eint_interrupt_handler,
				  !int_type ? IRQF_TRIGGER_RISING :
				  IRQF_TRIGGER_FALLING,
				  "TOUCH_PANEL-eint", NULL);
		if (ret > 0) {
			ret = -1;
			TPD_DMESG("tpd request_irq IRQ LINE NOT AVAILABLE!.");
		}
	}

#ifdef TPD_PROC_DEBUG
#if 0
    gsl_config_proc = create_proc_entry(GSL_CONFIG_PROC_FILE, 0666, NULL);
    printk("[tp-gsl] [%s] gsl_config_proc = %x \n",__func__,gsl_config_proc);
    if (gsl_config_proc == NULL)
    {
        print_info("create_proc_entry %s failed\n", GSL_CONFIG_PROC_FILE);
    }
    else
    {
        gsl_config_proc->read_proc = gsl_config_read_proc;
        gsl_config_proc->write_proc = gsl_config_write_proc;
    }
#else
    proc_create(GSL_CONFIG_PROC_FILE,0666,NULL,&gsl_seq_fops);
#endif
    gsl_proc_flag = 0;
#endif
	
#if 0
	init_timer(&led_timer);
	led_timer.expires	= jiffies + 1/(1000/HZ);
	led_timer.function	= timer_func;
#endif
}

static int touch_event_handler(void *unused)
{
	struct sched_param param = { .sched_priority = 4 };
	sched_setscheduler(current, SCHED_RR, &param);
	
	gsl_post_init();

	do
	{
		set_current_state(TASK_INTERRUPTIBLE);
		// modified by elink_phil@20170616 start <<<
		if (tpd_flag == 0)
		{
			DEFINE_WAIT(wait);
			prepare_to_wait(&waiter, &wait, TASK_INTERRUPTIBLE);
			schedule();
			finish_wait(&waiter, &wait);
		}
		// wait_event_interruptible(waiter, tpd_flag != 0);
		// modified end >>>
		tpd_flag = 0;
		TPD_DEBUG_SET_TIME;
		set_current_state(TASK_RUNNING);
		//print_info("===touch_event_handler, task running===\n");

		eint_flag = 0;
		report_data_handle();
		
	} while (!kthread_should_stop());
	
	return 0;
}

static int tpd_i2c_detect(struct i2c_client *client, struct i2c_board_info *info) {
	strcpy(info->type, TPD_DEVICE);
	return 0;
}

static int tpd_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int err = 0;

// added by heliang for tp ps start <<<
#ifdef CONFIG_TP_PROXIMITY_SUPPORT
	struct gsl_priv *obj;
	//struct hwmsen_object obj_ps;
	//struct als_control_path als_ctl = { 0 };
	//struct als_data_path als_data = { 0 };
	struct ps_control_path ps_ctl = { 0 };
	struct ps_data_path    ps_data = { 0 };
	obj = kzalloc(sizeof(*obj), GFP_KERNEL);
	gsl_obj = obj;
#endif
// added end >>>

	printk("%s\n", __func__);

	i2c_client = client;

	tpd_gpio_output(GTP_RST_PORT, 0);
	msleep(100);

	tpd_gpio_output(GTP_RST_PORT, 1);

	tpd_gpio_output(GTP_INT_PORT, 0);
	tpd_gpio_as_int(GTP_INT_PORT);
	msleep(50);

	mutex_init(&gsl_i2c_lock);
	
	if(test_i2c(client) < 0)
	{
		printk("%s: i2c test error, so quit!!!\n", __func__);
		return -ENODEV;
	}

// added by heliang for tp ps start <<<
#ifdef CONFIG_TP_PROXIMITY_SUPPORT
    gsl_obj = kzalloc(sizeof(*gsl_obj), GFP_KERNEL);

	//wake_lock_init(&ps_lock, WAKE_LOCK_SUSPEND, "ps wakelock");  
	ps_ctl.is_use_common_factory = false;
	ps_ctl.open_report_data = ps_open_report_data;
	ps_ctl.enable_nodata = ps_enable_nodata;
	ps_ctl.set_delay = ps_set_delay;
	ps_ctl.batch = ps_batch;
	ps_ctl.flush = ps_flush;
	ps_ctl.is_report_input_direct = false;
	ps_ctl.is_support_batch = false;
	err = ps_register_control_path(&ps_ctl);
	if (err) {
		printk("register fail = %d\n", err);
		//goto exit_sensor_obj_attach_fail;
	}

	ps_data.get_data = ps_get_data;
	ps_data.vender_div = 100;
	err = ps_register_data_path(&ps_data);
	if (err) {
		printk("tregister fail = %d\n", err);
		//goto exit_sensor_obj_attach_fail;
	}
	//tpd_enable_ps(1);
#endif
// added end >>>

	thread = kthread_run(touch_event_handler, 0, TPD_DEVICE);
	if (IS_ERR(thread)) {
		err = PTR_ERR(thread);
		TPD_DMESG(TPD_DEVICE " failed to create kernel thread: %d\n", err);
	}

	printk("%s successfully\n", __func__);

	tpd_load_status = 1;

	return 0;
}

static int tpd_i2c_remove(struct i2c_client *client)
{
	printk("==tpd_i2c_remove==\n");
	
	return 0;
}


static const struct i2c_device_id tpd_i2c_id[] = {{TPD_DEVICE,0},{}};

struct i2c_driver tpd_i2c_driver = {
	.driver = {
		.name = TPD_DEVICE, 
		.owner = THIS_MODULE,
		.of_match_table = tpd_of_match,
	},
	.probe = tpd_i2c_probe,
	.remove = tpd_i2c_remove,
	.id_table = tpd_i2c_id,
	.detect = tpd_i2c_detect,
};

int tpd_local_init(void)
{
	printk("%s\n", __func__);

	init_tp_power(tpd->tpd_dev, TP_POWER);
	tp_power_on(1);

	if(i2c_add_driver(&tpd_i2c_driver) != 0) {
		printk("%s() unable to add i2c driver.\n", __func__);
		return -1;
	}

	if (tpd_load_status == 0) {
		printk("add error touch panel driver.\n");
		i2c_del_driver(&tpd_i2c_driver);	
		return -1;
	}

#if (defined(TPD_WARP_START) && defined(TPD_WARP_END))
	TPD_DO_WARP = 1;
	memcpy(tpd_wb_start, tpd_wb_start_local, TPD_WARP_CNT*4);
	memcpy(tpd_wb_end, tpd_wb_start_local, TPD_WARP_CNT*4);
#endif

#if (defined(TPD_HAVE_CALIBRATION) && !defined(TPD_CUSTOM_CALIBRATION))
	memcpy(tpd_calmat, tpd_calmat_local, 8*4);
	memcpy(tpd_def_calmat, tpd_def_calmat_local, 8*4);
#endif
	tpd_type_cap = 1;

	printk("%s successfully\n", __func__);
	return 0;
}

/* Function to manage low power suspend */
static void tpd_suspend(struct device *h)
{
	printk("==tpd_suspend==\n");
#ifdef CONFIG_TP_PROXIMITY_SUPPORT
	if (tpd_proximity_flag == 1){
        return ;
    }
#endif
#ifdef TPD_PROC_DEBUG
	if(gsl_proc_flag == 1){
		return;
	}
#endif

	tpd_halt = 1;
	//mt_eint_mask(CUST_EINT_TOUCH_PANEL_NUM);
	disable_irq(TOUCH_IRQ);
#ifdef GSL_MONITOR
	printk( "gsl_ts_suspend () : cancel gsl_monitor_work\n");
	cancel_delayed_work_sync(&gsl_monitor_work);
#endif

	tpd_gpio_output(GTP_RST_PORT, 0);
}

/* Function to manage power-on resume */
static void tpd_resume(struct device *h)
{
	printk("==tpd_resume==\n");
#ifdef CONFIG_TP_PROXIMITY_SUPPORT
	if ((tpd_proximity_flag == 1) && (tpd_halt == 0)){
        tpd_enable_ps(1);
        return;
    }
#endif

#ifdef TPD_PROC_DEBUG
    if(gsl_proc_flag == 1){
        return;
    }
#endif

	tpd_gpio_output(GTP_RST_PORT, 1);
	msleep(30);	
	reset_chip(i2c_client);
	startup_chip(i2c_client);
	check_mem_data(i2c_client);

#ifdef CONFIG_TP_PROXIMITY_SUPPORT
	if ((tpd_proximity_flag == 1) && (tpd_halt == 1)){
        tpd_enable_ps(1);
    }
#endif

#ifdef GSL_MONITOR
	printk( "gsl_ts_resume () : queue gsl_monitor_work\n");
	queue_delayed_work(gsl_monitor_workqueue, &gsl_monitor_work, 300);
#endif	

	enable_irq(TOUCH_IRQ);
	tpd_halt = 0;
}

static struct tpd_driver_t tpd_device_driver = {
	.tpd_device_name = GSLX680_NAME,
	.tpd_local_init = tpd_local_init,
	.suspend = tpd_suspend,
	.resume = tpd_resume,
};

#ifdef CONFIG_TP_PROXIMITY_SUPPORT
void tp_alsps_release(struct kobject *kobject);
ssize_t tp_alsps_show(struct kobject *kobject, struct attribute *attr,char *buf);
ssize_t tp_alsps_store(struct kobject *kobject,struct attribute *attr,const char *buf, size_t count);
 
struct attribute tp_alsps_attr = {
	.name = "tp_alsps_en",
	.mode = S_IRWXUGO,
};
 
static struct attribute *def_attrs[] = {
	&tp_alsps_attr,
	NULL,
};
 
 
struct sysfs_ops tp_alsps_sysops =
{
	.show = tp_alsps_show,
	.store = tp_alsps_store,
};
 
struct kobj_type ktype =
{
	.release = tp_alsps_release,
	.sysfs_ops=&tp_alsps_sysops,
	.default_attrs=def_attrs,
};
 
void tp_alsps_release(struct kobject *kobject)
{
	printk("tp_alsps_release.\n");
}

ssize_t tp_alsps_show(struct kobject *kobject, struct attribute *attr,char *buf)
{
    printk("attrname:%s.\n", attr->name);
    sprintf(buf,"%s\n",attr->name);
    return strlen(attr->name)+2;
}

ssize_t tp_alsps_store(struct kobject *kobject,struct attribute *attr,const char *buf, size_t count)
{
	if(buf[0]=='e' && buf[1]=='n'){
		TPD_PROC_DISABLE=0;
	}else{
		TPD_PROC_DISABLE=1;
	}
	printk("TPD_PROC_DISABLE=%d\n",TPD_PROC_DISABLE);
	return count;
}
 
struct kobject kobj;
static int tp_alsps_init(void)
{
    if( kobject_init_and_add(&kobj,&ktype,NULL,"elink") == 0 ) {
    	printk("[%s] Return OK, I guess. \n",__func__);
   		return 0;
    } else {
    	printk("[%s] Return Fail, I guess. \n",__func__);
   		return -1;
    }

}
 
static int tp_alsps_exit(void)
{
        kobject_del(&kobj);
        return 0;
}
#endif
/* called when loaded into kernel */
static int __init tpd_driver_init(void) {
	printk("Sileadinc gslX680 touch panel driver init\n");
	
	// added by elink_phil@20161021 for when in power off charging mode, don't load tp driver start >>>
	if (KERNEL_POWER_OFF_CHARGING_BOOT == get_boot_mode()\
		|| LOW_POWER_OFF_CHARGING_BOOT == get_boot_mode()\
		|| RECOVERY_BOOT == get_boot_mode())
	{
		printk("CHARGING_BOOT!!! don't need tp driver\n");
		return -EPERM;
	}
	// added end <<<

	tpd_get_dts_info();

	if(tpd_driver_add(&tpd_device_driver) < 0)
		printk("add gslX680 driver failed\n");
	
#ifdef CONFIG_TP_PROXIMITY_SUPPORT
	tp_alsps_init();//by lai
#endif
	return 0;
}

/* should never be called */
static void __exit tpd_driver_exit(void) {
	printk("Sileadinc gslX680 touch panel driver exit\n");

	tpd_driver_remove(&tpd_device_driver);
	
#ifdef CONFIG_TP_PROXIMITY_SUPPORT
	tp_alsps_exit();//by lai
#endif
}

module_init(tpd_driver_init);
module_exit(tpd_driver_exit);
