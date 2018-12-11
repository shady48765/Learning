#include "tpd.h"
#include "./include/tpd_gt9271_common.h"
#include <linux/proc_fs.h>	/*proc */
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <linux/atomic.h>
#ifdef CONFIG_GTP_SUPPORT_I2C_DMA
    #include <linux/dma-mapping.h>
#endif

#define TOUCH_DEV_NAME      "gt9xx"


#ifdef ELINK_TP_SUUPORT
    #include <elink_tp.h>
    extern void elink_tp_misc_operation(void (*get_tp_resolution)(ELINK_RESOLUTION *tp));
    extern void elink_tp_remap(int *x, int *y);
#endif

/**----------------------- global parameter declaration start ----------------------*/
#define TOUCH_DEV_NAME      "gt9271"

#define SWAP(T, x, y) do { T *p = &(x), *q = &(y), z = *p; *p = *q; *q = z; } while(0)

static DECLARE_WAIT_QUEUE_HEAD(waiter);
static DEFINE_MUTEX(i2c_access);
static unsigned int touch_irq;

struct gt9271_information {
    struct mutex        gt9271_mutex;
    struct spinlock_t   gt9271_spinlock;
    struct client       gt9271_client;
    struct i2c_client   *gt9271_i2c_client;
    int driver_load_status;
};
struct gt9271_information *gt9271_info;

/**----------------------- global parameter declaration end ----------------------*/
/**----------------------- function declaration start ----------------------*/
int get_dts_info(struct device *dev);
/**----------------------- function declaration end ----------------------*/

static const struct i2c_device_id gt9271_i2c_id[] = {{TOUCH_DEV_NAME, 0}, {} };




#ifdef CONFIG_OF
static of_device_id gt9271_match[] = {
    {.compatible = TOUCH_DEV_NAME,},
    {/* keep this */},
};
MODULE_DEVICE_TABLE(of, gt9271_match);
#endif

static struct i2c_driver gt9271_driver = {
    .driver = {
        .name = TOUCH_DEV_NAME,
        .owner = THIS_MODULE,
#ifdef CONFIG_OF
        .of_match_table = of_match_ptr(gt9271_match);
#endif
    },
    .probe = gt9271_probe,
    .remove = gt9271_remove,
    .id_table = gt9271_i2c_id,
    
};












static int tpd_local_init(void)
{
    // get touch panel param configuration
    tpd_get_dts_info();
    
}


int get_dts_info(struct device *dev)
{
    
}

static void tpd_suspend(struct device *h)
{
    
}

static void tpd_resume(struct device *h)
{
    
}

static struct tpd_driver_t tpd_device_driver = {
    .tpd_device_name    = TOUCH_DEV_NAME,
    .tpd_local_init     = tpd_local_init,
    .suspend            = tpd_suspend,
    .resume             = tpd_resume,
};
static int __init tpd_init(void)
{
    GTP_INFO("GT9271 driver init protocal");
    gt9271_info = kmalloc(sizeof(struct gt9271_information), GFP_KERNEL);
    if(tpd_driver_add(&tpd_device_driver) < 0) {
        GTP_ERROR("GT9271 driver init failed");
        return -ERRerror;
    }
    return 0;
}


static void __exit tpd_exit(void)
{
    GTP_INFO("GT9271 driver exit");
    kfree(gt9271_info);
    tpd_driver_remove(&tpd_device_driver);
}




module_init(tpd_init);
module_exit(tpd_exit);
MODULE_LICENSE(GTP v2);
MODULE_DESCRIPTION("GT9271 touch panel driver for mtk android system.");













































#pragma pack(1)
struct st_cmd_head {
	u8 wr;			/* write read flag，0:R1:W2:PID 3: */
	u8 flag;		/* 0:no need flag/int 1: need flag2:need int */
	u8 flag_addr[2];	/* flag address */
	u8 flag_val;		/* flag val */
	u8 flag_relation;	/* flag_val:flag 0:not equal 1:equal 2:> 3:< */
	u16 circle;		/* polling cycle */
	u8 times;		/* plling times */
	u8 retry;		/* I2C retry times */
	u16 delay;		/* delay before read or after write */
	u16 data_len;		/* data length */
	u8 addr_len;		/* address length */
	u8 addr[2];		/* address */
	u8 res[3];		/* reserved */
	u8 *data;		/* data pointer */
};
#pragma pack()
static struct st_cmd_head cmd_head, cmd_head2;

#define DATA_LENGTH_UINT	512
#define CMD_HEAD_LENGTH (sizeof(struct st_cmd_head) - sizeof(u8 *))
static char procname[20] = { 0 };

static struct i2c_client *gt_client;

static struct proc_dir_entry *goodix_proc_entry;

static ssize_t goodix_tool_write(struct file *, const char __user *, size_t, loff_t *);
static ssize_t goodix_tool_read(struct file *, char __user *, size_t, loff_t *);
static s32 (*tool_i2c_read)(u8*, u16);
static s32 (*tool_i2c_write)(u8*, u16);

static const struct file_operations tool_ops = {
	.owner = THIS_MODULE,
	.read = goodix_tool_read,
	.write = goodix_tool_write,
};

static int hotknot_open(struct inode *node, struct file *flip);
static int hotknot_release(struct inode *node, struct file *flip);
static ssize_t hotknot_write(struct file *, const char __user *, size_t, loff_t *);
static ssize_t hotknot_read(struct file *, char __user *, size_t, loff_t *);

static s32 DATA_LENGTH = 0;
static s8 IC_TYPE[16] = "GT9XX";

#ifdef CONFIG_HOTKNOT_BLOCK_RW
DECLARE_WAIT_QUEUE_HEAD(bp_waiter);
u8 got_hotknot_state = 0;
u8 got_hotknot_extra_state = 0;
u8 wait_hotknot_state = 0;
u8 force_wake_flag = 0;
#endif

#define HOTKNOTNAME "hotknot"
u8 gtp_hotknot_enabled = 0;

static const struct file_operations hotknot_fops = {
/* .owner = THIS_MODULE, */
	.open = hotknot_open,
	.release = hotknot_release,
	.read = hotknot_read,
	.write = hotknot_write,
};

static struct miscdevice hotknot_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = HOTKNOTNAME,
	.fops = &hotknot_fops,
};

static void tool_set_proc_name(char *procname)
{
	sprintf(procname, "gmnode");
}

static s32 tool_i2c_read_no_extra(u8 *buf, u16 len)
{
	s32 ret = -1;

	ret = gtp_i2c_read(gt_client, buf, len + GTP_ADDR_LENGTH);
	return ret;
}

static s32 tool_i2c_write_no_extra(u8 *buf, u16 len)
{
	s32 ret = -1;

	ret = gtp_i2c_write(gt_client, buf, len);
	return ret;
}

static s32 tool_i2c_read_with_extra(u8 *buf, u16 len)
{
	s32 ret = -1;
	u8 pre[2] = { 0x0f, 0xff };
	u8 end[2] = { 0x80, 0x00 };

	tool_i2c_write_no_extra(pre, 2);
	ret = tool_i2c_read_no_extra(buf, len);
	tool_i2c_write_no_extra(end, 2);

	return ret;
}

static s32 tool_i2c_write_with_extra(u8 *buf, u16 len)
{
	s32 ret = -1;
	u8 pre[2] = { 0x0f, 0xff };
	u8 end[2] = { 0x80, 0x00 };

	tool_i2c_write_no_extra(pre, 2);
	ret = tool_i2c_write_no_extra(buf, len);
	tool_i2c_write_no_extra(end, 2);

	return ret;
}

static void register_i2c_func(void)
{
/* if (!strncmp(IC_TYPE, "GT818", 5) || !strncmp(IC_TYPE, "GT816", 5) */
/* || !strncmp(IC_TYPE, "GT811", 5) || !strncmp(IC_TYPE, "GT818F", 6) */
/* || !strncmp(IC_TYPE, "GT827", 5) || !strncmp(IC_TYPE,"GT828", 5) */
/* || !strncmp(IC_TYPE, "GT813", 5)) */
	if (strncmp(IC_TYPE, "GT8110", 6) && strncmp(IC_TYPE, "GT8105", 6)
	    && strncmp(IC_TYPE, "GT801", 5) && strncmp(IC_TYPE, "GT800", 5)
	    && strncmp(IC_TYPE, "GT801PLUS", 9) && strncmp(IC_TYPE, "GT811", 5)
	    && strncmp(IC_TYPE, "GTxxx", 5) && strncmp(IC_TYPE, "GT9XX", 5)
        && strncmp(IC_TYPE, "GT9271", 6)) {
		tool_i2c_read = tool_i2c_read_with_extra;
		tool_i2c_write = tool_i2c_write_with_extra;
		GTP_DEBUG("I2C function: with pre and end cmd!");
	} else {
		tool_i2c_read = tool_i2c_read_no_extra;
		tool_i2c_write = tool_i2c_write_no_extra;
		GTP_INFO("I2C function: without pre and end cmd!");
	}
}

static void unregister_i2c_func(void)
{
	tool_i2c_read = NULL;
	tool_i2c_write = NULL;
	GTP_INFO("I2C function: unregister i2c transfer function!");
}

s32 init_wr_node(struct i2c_client *client)
{
	s32 i;

	gt_client = i2c_client_point;

	memset(&cmd_head, 0, sizeof(cmd_head));
	cmd_head.data = NULL;

	i = 5;

	while ((!cmd_head.data) && i) {
		cmd_head.data = kzalloc(i * DATA_LENGTH_UINT, GFP_KERNEL);

		if (NULL != cmd_head.data)
			break;

		i--;
	}

	if (i) {
		DATA_LENGTH = i * DATA_LENGTH_UINT + GTP_ADDR_LENGTH;
		GTP_INFO("Applied memory size:%d.", DATA_LENGTH);
	} else {
		GTP_ERROR("Apply for memory failed.");
		return FAIL;
	}

	cmd_head.addr_len = 2;
	cmd_head.retry = 5;

	memset(&cmd_head2, 0, sizeof(cmd_head2));
	cmd_head2.data = NULL;

	i = 5;

	while ((!cmd_head2.data) && i) {
		cmd_head2.data = kzalloc(i * DATA_LENGTH_UINT, GFP_KERNEL);

		if (NULL != cmd_head2.data)
			break;

		i--;
	}

	if (i) {
		DATA_LENGTH = i * DATA_LENGTH_UINT + GTP_ADDR_LENGTH;
		GTP_INFO("Applied memory size:%d.", DATA_LENGTH);
	} else {
		GTP_ERROR("Apply for memory failed.");
		return FAIL;
	}

	cmd_head2.addr_len = 2;
	cmd_head2.retry = 5;

	register_i2c_func();

	tool_set_proc_name(procname);
	goodix_proc_entry = proc_create(procname, 0666, NULL, &tool_ops);

	if (misc_register(&hotknot_misc_device)) {
		GTP_ERROR("mtk_tpd: hotknot_device register failed\n");
		return FAIL;
	}

	if (goodix_proc_entry == NULL) {
		GTP_ERROR("Couldn't create proc entry!");
		return FAIL;
	}
	GTP_INFO("Create proc entry success!");

	return SUCCESS;
}

void uninit_wr_node(void)
{
	kfree(cmd_head.data);
	cmd_head.data = NULL;
	kfree(cmd_head2.data);
	cmd_head2.data = NULL;
	unregister_i2c_func();
	remove_proc_entry(procname, NULL);
}

static u8 relation(u8 src, u8 dst, u8 rlt)
{
	u8 ret = 0;

	switch (rlt) {
	case 0:
		ret = (src != dst) ? true : false;
		break;

	case 1:
		ret = (src == dst) ? true : false;
		GTP_DEBUG("equal:src:0x%02x dst:0x%02x ret:%d.", src, dst, (s32) ret);
		break;

	case 2:
		ret = (src > dst) ? true : false;
		break;

	case 3:
		ret = (src < dst) ? true : false;
		break;

	case 4:
		ret = (src & dst) ? true : false;
		break;

	case 5:
		ret = (!(src | dst)) ? true : false;
		break;

	default:
		ret = false;
		break;
	}

	return ret;
}

/*******************************************************
Function:
Comfirm function.
Input:
None.
Output:
Return write length.
********************************************************/
static u8 comfirm(void)
{
	s32 i = 0;
	u8 buf[32];

	/* memcpy(&buf[GTP_ADDR_LENGTH - cmd_head.addr_len], &cmd_head.flag_addr, cmd_head.addr_len); */
	/* memcpy(buf, &cmd_head.flag_addr, cmd_head.addr_len);//Modified by Scott, 2012-02-17 */
	memcpy(buf, cmd_head.flag_addr, cmd_head.addr_len);

	for (i = 0; i < cmd_head.times; i++) {
		if (tool_i2c_read(buf, 1) <= 0) {
			GTP_ERROR("Read flag data failed!");
			return FAIL;
		}

		if (true == relation(buf[GTP_ADDR_LENGTH], cmd_head.flag_val, cmd_head.flag_relation)) {
			GTP_DEBUG("value at flag addr:0x%02x.", buf[GTP_ADDR_LENGTH]);
			GTP_DEBUG("flag value:0x%02x.", cmd_head.flag_val);
			break;
		}

		msleep(cmd_head.circle);
	}

	if (i >= cmd_head.times) {
		GTP_ERROR("Didn't get the flag to continue!");
		return FAIL;
	}

	return SUCCESS;
}

/*******************************************************
Function:
Goodix tool write function.
Input:
standard proc write function param.
Output:
Return write length.
********************************************************/
static ssize_t goodix_tool_write(struct file *filp, const char __user *buff, size_t len, loff_t *off)
{
	s32 ret = 0;

	GTP_DEBUG_FUNC();
	GTP_DEBUG_ARRAY((u8 *) buff, len);

	if (gtp_resetting == 1) {
		/* GTP_ERROR("[Write]tpd_halt =1 fail!"); */
		return FAIL;
	}

	ret = copy_from_user(&cmd_head, buff, CMD_HEAD_LENGTH);

	if (ret)
		GTP_ERROR("copy_from_user failed.");

	GTP_DEBUG("wr:0x%02x.", cmd_head.wr);

	if (1 == cmd_head.wr) {
		/* copy_from_user(&cmd_head.data[cmd_head.addr_len], &buff[CMD_HEAD_LENGTH], cmd_head.data_len); */
		ret = copy_from_user(&cmd_head.data[GTP_ADDR_LENGTH], &buff[CMD_HEAD_LENGTH], cmd_head.data_len);
		if (ret)
			GTP_ERROR("copy_from_user failed.");

		memcpy(&cmd_head.data[GTP_ADDR_LENGTH - cmd_head.addr_len], cmd_head.addr, cmd_head.addr_len);

		GTP_DEBUG_ARRAY(cmd_head.data, cmd_head.data_len + cmd_head.addr_len);
		GTP_DEBUG_ARRAY((u8 *)&buff[CMD_HEAD_LENGTH], cmd_head.data_len);

		if (1 == cmd_head.flag) {
			if (FAIL == comfirm()) {
				GTP_ERROR("[WRITE]Comfirm fail!");
				return FAIL;
			}
		} else if (2 == cmd_head.flag) {
			/* Need interrupt! */
		}

		if (tool_i2c_write(&cmd_head.data[GTP_ADDR_LENGTH - cmd_head.addr_len],
				   cmd_head.data_len + cmd_head.addr_len) <= 0) {
			GTP_ERROR("[WRITE]Write data failed!");
			return FAIL;
		}

		GTP_DEBUG_ARRAY(&cmd_head.data[GTP_ADDR_LENGTH - cmd_head.addr_len],
				cmd_head.data_len + cmd_head.addr_len);

		if (cmd_head.delay)
			msleep(cmd_head.delay);

		return cmd_head.data_len + CMD_HEAD_LENGTH;
	} else if (3 == cmd_head.wr) {	/* Write ic type */
		memcpy(IC_TYPE, cmd_head.data, cmd_head.data_len);
		register_i2c_func();

		return cmd_head.data_len + CMD_HEAD_LENGTH;
	} else if (5 == cmd_head.wr) {
		/* memcpy(IC_TYPE, cmd_head.data, cmd_head.data_len); */

		return cmd_head.data_len + CMD_HEAD_LENGTH;
	} else if (7 == cmd_head.wr) {	/* disable irq! */
		gtp_irq_disable();
#ifdef CONFIG_GTP_ESD_PROTECT
		gtp_esd_switch(i2c_client_point, SWITCH_OFF);
#endif
		return CMD_HEAD_LENGTH;
	} else if (9 == cmd_head.wr) {	/* enable irq! */
		gtp_irq_enable();
#ifdef CONFIG_GTP_ESD_PROTECT
		gtp_esd_switch(i2c_client_point, SWITCH_ON);
#endif
		return CMD_HEAD_LENGTH;
	} else if (17 == cmd_head.wr) {
		ret = copy_from_user(&cmd_head.data[GTP_ADDR_LENGTH], &buff[CMD_HEAD_LENGTH], cmd_head.data_len);
		if (ret)
			GTP_DEBUG("copy_from_user failed.");

		if (cmd_head.data[GTP_ADDR_LENGTH]) {
			GTP_DEBUG("gtp enter rawdiff.");
			gtp_rawdiff_mode = true;
		} else {
			gtp_rawdiff_mode = false;
			GTP_DEBUG("gtp leave rawdiff.");
		}

		return CMD_HEAD_LENGTH;
	}
#ifdef UPDATE_FUNCTIONS
	else if (11 == cmd_head.wr) {	/* Enter update mode! */
		if (FAIL == gup_enter_update_mode(gt_client))
			return FAIL;
	} else if (13 == cmd_head.wr) {	/* Leave update mode! */
		gup_leave_update_mode();
	} else if (15 == cmd_head.wr) {	/* Update firmware! */
		show_len = 0;
		total_len = 0;
		memset(cmd_head.data, 0, cmd_head.data_len + 1);
		memcpy(cmd_head.data, &buff[CMD_HEAD_LENGTH], cmd_head.data_len);
		GTP_DEBUG("update firmware, filename: %s", cmd_head.data);
		if (FAIL == gup_update_proc((void *)cmd_head.data))
			return FAIL;

	} 
#endif
#ifdef CONFIG_GTP_HOTKNOT
	else if (19 == cmd_head.wr) {	/* load subsystem */
		ret = copy_from_user(&cmd_head.data[0], &buff[CMD_HEAD_LENGTH], cmd_head.data_len);
		if (0 == cmd_head.data[0]) {
			if (FAIL == gup_load_hotknot_fw())
				return FAIL;

		} else if (1 == cmd_head.data[0]) {
			if (FAIL == gup_load_authorization_fw())
				return FAIL;

		} else if (2 == cmd_head.data[0]) {
			if (FAIL == gup_recovery_touch())
				return FAIL;

		} else if (3 == cmd_head.data[0]) {
			if (FAIL == gup_load_touch_fw(NULL))
				return FAIL;

		}
	}
#endif
#ifdef CONFIG_HOTKNOT_BLOCK_RW
	else if (21 == cmd_head.wr) {
		u16 wait_hotknot_timeout = 0;

		u8 rqst_hotknot_state;

		ret = copy_from_user(&cmd_head.data[GTP_ADDR_LENGTH], &buff[CMD_HEAD_LENGTH], cmd_head.data_len);
		if (ret)
			GTP_ERROR("copy_from_user failed.");

		rqst_hotknot_state = cmd_head.data[GTP_ADDR_LENGTH];
		wait_hotknot_state |= rqst_hotknot_state;
		wait_hotknot_timeout = (cmd_head.data[GTP_ADDR_LENGTH + 1] << 8) + cmd_head.data[GTP_ADDR_LENGTH + 2];
		GTP_DEBUG("Goodix tool received wait polling state:0x%x,timeout:%d, all wait state:0x%x",
			  rqst_hotknot_state, wait_hotknot_timeout, wait_hotknot_state);
		got_hotknot_state &= (~rqst_hotknot_state);
		/* got_hotknot_extra_state = 0; */
		switch (rqst_hotknot_state) {
			set_current_state(TASK_INTERRUPTIBLE);
		case HN_DEVICE_PAIRED:
			hotknot_paired_flag = 0;
			wait_event_interruptible(bp_waiter, force_wake_flag ||
						 rqst_hotknot_state == (got_hotknot_state & rqst_hotknot_state));
			wait_hotknot_state &= (~rqst_hotknot_state);
			if (rqst_hotknot_state != (got_hotknot_state & rqst_hotknot_state)) {
				GTP_ERROR("Wait 0x%x block polling waiter failed.", rqst_hotknot_state);
				force_wake_flag = 0;
				return FAIL;
			}
			break;
		case HN_MASTER_SEND:
		case HN_SLAVE_RECEIVED:
			wait_event_interruptible_timeout(bp_waiter, force_wake_flag ||
							 rqst_hotknot_state == (got_hotknot_state & rqst_hotknot_state),
							 wait_hotknot_timeout);
			wait_hotknot_state &= (~rqst_hotknot_state);
			if (rqst_hotknot_state == (got_hotknot_state & rqst_hotknot_state))
				return got_hotknot_extra_state;

			GTP_ERROR("Wait 0x%x block polling waiter timeout.", rqst_hotknot_state);
			force_wake_flag = 0;
			return FAIL;
		case HN_MASTER_DEPARTED:
		case HN_SLAVE_DEPARTED:
			wait_event_interruptible_timeout(bp_waiter, force_wake_flag ||
							 rqst_hotknot_state == (got_hotknot_state & rqst_hotknot_state),
							 wait_hotknot_timeout);
			wait_hotknot_state &= (~rqst_hotknot_state);
			if (rqst_hotknot_state != (got_hotknot_state & rqst_hotknot_state)) {
				GTP_ERROR("Wait 0x%x block polling waitor timeout.", rqst_hotknot_state);
				force_wake_flag = 0;
				return FAIL;
			}
			break;
		default:
			GTP_ERROR("Invalid rqst_hotknot_state in goodix_tool.");
			break;
		}
		force_wake_flag = 0;
	} else if (23 == cmd_head.wr) {
		GTP_DEBUG("Manual wakeup all block polling waiter!");
		got_hotknot_state = 0;
		wait_hotknot_state = 0;
		force_wake_flag = 1;
		hotknot_paired_flag = 0;
		wake_up_interruptible(&bp_waiter);
	}
#endif
	return CMD_HEAD_LENGTH;
}

/*******************************************************
Function:
Goodix tool read function.
Input:
standard proc read function param.
Output:
Return read length.
********************************************************/
static ssize_t goodix_tool_read(struct file *flie, char __user *page, size_t size, loff_t *ppos)
{
	s32 ret;

	GTP_DEBUG_FUNC();

	if (gtp_resetting == 1)
		return FAIL;

	if (*ppos) {
		*ppos = 0;
		return 0;
	}

	if (cmd_head.wr % 2) {
		GTP_ERROR("[READ] invaild operator fail!");
		return FAIL;
	} else if (!cmd_head.wr) {
		u16 len = 0;
		s16 data_len = 0;
		u16 loc = 0;

		if (1 == cmd_head.flag) {
			if (FAIL == comfirm()) {
				GTP_ERROR("[READ]Comfirm fail!");
				return FAIL;
			}
		} else if (2 == cmd_head.flag) {
			/* Need interrupt! */
		}

		memcpy(cmd_head.data, cmd_head.addr, cmd_head.addr_len);

		GTP_DEBUG("[CMD HEAD DATA] ADDR:0x%02x%02x.", cmd_head.data[0], cmd_head.data[1]);
		GTP_DEBUG("[CMD HEAD ADDR] ADDR:0x%02x%02x.", cmd_head.addr[0], cmd_head.addr[1]);

		if (cmd_head.delay)
			msleep(cmd_head.delay);

		data_len = cmd_head.data_len;

		while (data_len > 0) {
			if (data_len > DATA_LENGTH)
				len = DATA_LENGTH;
			else
				len = data_len;

			data_len -= len;

			if (tool_i2c_read(cmd_head.data, len) <= 0) {
				GTP_ERROR("[READ]Read data failed!");
				return FAIL;
			}

			/* memcpy(&page[loc], &cmd_head.data[GTP_ADDR_LENGTH], len); */
			ret = simple_read_from_buffer(&page[loc], size, ppos, &cmd_head.data[GTP_ADDR_LENGTH], len);
			if (ret < 0)
				return ret;

			loc += len;

			GTP_DEBUG_ARRAY(&cmd_head.data[GTP_ADDR_LENGTH], len);
			GTP_DEBUG_ARRAY(page, len);
		}
		return cmd_head.data_len;
	} else if (2 == cmd_head.wr) {
		ret = simple_read_from_buffer(page, size, ppos, IC_TYPE, sizeof(IC_TYPE));
		return ret;
	} else if (4 == cmd_head.wr) {
		u8 progress_buf[4];

		progress_buf[0] = show_len >> 8;
		progress_buf[1] = show_len & 0xff;
		progress_buf[2] = total_len >> 8;
		progress_buf[3] = total_len & 0xff;

		ret = simple_read_from_buffer(page, size, ppos, progress_buf, 4);
		return ret;
	} else if (6 == cmd_head.wr) {
		/* Read error code! */
	} else if (8 == cmd_head.wr) {	/* Read driver version */
		ret = simple_read_from_buffer(page, size, ppos, GTP_DRIVER_VERSION, strlen(GTP_DRIVER_VERSION));
		return ret;
	}
	return -EPERM;
}

static int hotknot_open(struct inode *node, struct file *flip)
{
	GTP_DEBUG("Hotknot is enable.");
	gtp_hotknot_enabled = 1;
	return 0;
}

static int hotknot_release(struct inode *node, struct file *filp)
{
	GTP_DEBUG("Hotknot is disable.");
	gtp_hotknot_enabled = 0;
	return 0;
}

/*******************************************************
Function:
Goodix tool write function.
Input:
standard proc write function param.
Output:
Return write length.
********************************************************/
static ssize_t hotknot_write(struct file *filp, const char __user *buff, size_t len, loff_t *ppos)
{
	s32 ret = 0;
	int cnt = 30;
	
	GTP_DEBUG_FUNC();
	GTP_DEBUG_ARRAY((u8 *) buff, len);
	
	while (cnt-- && gtp_updating_fw) {
	        ssleep(1);
	}
	
	if (gtp_resetting == 1) {
		GTP_DEBUG("[Write]tpd_halt =1 fail!");
		return FAIL;
	}

	ret = copy_from_user(&cmd_head2, buff, CMD_HEAD_LENGTH);
	if (ret)
		GTP_ERROR("copy_from_user failed.");

	GTP_DEBUG("wr:0x%02x.", cmd_head2.wr);
	/*
	   GTP_DEBUG("flag:0x%02x.", cmd_head2.flag);
	   GTP_DEBUG("flag addr:0x%02x%02x.", cmd_head2.flag_addr[0], cmd_head2.flag_addr[1]);
	   GTP_DEBUG("flag val:0x%02x.", cmd_head2.flag_val);
	   GTP_DEBUG("flag rel:0x%02x.", cmd_head2.flag_relation);
	   GTP_DEBUG("circle:%d.", (s32)cmd_head2.circle);
	   GTP_DEBUG("times :%d.", (s32)cmd_head2.times);
	   GTP_DEBUG("retry :%d.", (s32)cmd_head2.retry);
	   GTP_DEBUG("delay :%d.", (s32)cmd_head2.delay);
	   GTP_DEBUG("data len:%d.", (s32)cmd_head2.data_len);
	   GTP_DEBUG("addr len:%d.", (s32)cmd_head2.addr_len);
	   GTP_DEBUG("addr:0x%02x%02x.", cmd_head2.addr[0], cmd_head2.addr[1]);
	   GTP_DEBUG("len:%d.", (s32)len);
	   GTP_DEBUG("buf[20]:0x%02x.", buff[CMD_HEAD_LENGTH]);
	 */

	if (1 == cmd_head2.wr) {
		/* copy_from_user(&cmd_head2.data[cmd_head2.addr_len], &buff[CMD_HEAD_LENGTH], cmd_head2.data_len); */
		ret = copy_from_user(&cmd_head2.data[GTP_ADDR_LENGTH], &buff[CMD_HEAD_LENGTH], cmd_head2.data_len);

		if (ret)
			GTP_ERROR("copy_from_user failed.");

		memcpy(&cmd_head2.data[GTP_ADDR_LENGTH - cmd_head2.addr_len], cmd_head2.addr, cmd_head2.addr_len);

		GTP_DEBUG_ARRAY(cmd_head2.data, cmd_head2.data_len + cmd_head2.addr_len);
		GTP_DEBUG_ARRAY((u8 *)&buff[CMD_HEAD_LENGTH], cmd_head2.data_len);

		if (1 == cmd_head2.flag) {
			if (FAIL == comfirm()) {
				GTP_ERROR("[WRITE]Comfirm fail!");
				return FAIL;
			}
		} else if (2 == cmd_head2.flag) {
			/* Need interrupt! */
		}

		if (tool_i2c_write(&cmd_head2.data[GTP_ADDR_LENGTH - cmd_head2.addr_len],
				   cmd_head2.data_len + cmd_head2.addr_len) <= 0) {
			GTP_ERROR("[WRITE]Write data failed!");
			return FAIL;
		}

		GTP_DEBUG_ARRAY(&cmd_head2.data[GTP_ADDR_LENGTH - cmd_head2.addr_len],
				cmd_head2.data_len + cmd_head2.addr_len);

		if (cmd_head2.delay)
			msleep(cmd_head2.delay);

		return cmd_head2.data_len + CMD_HEAD_LENGTH;
	} else if (3 == cmd_head2.wr) {	/* Write ic type */
		memcpy(IC_TYPE, cmd_head2.data, cmd_head2.data_len);
		register_i2c_func();

		return cmd_head2.data_len + CMD_HEAD_LENGTH;
	} else if (5 == cmd_head2.wr) {
		/* memcpy(IC_TYPE, cmd_head2.data, cmd_head2.data_len); */

		return cmd_head2.data_len + CMD_HEAD_LENGTH;
	} else if (7 == cmd_head2.wr) {	/* disable irq! */
		gtp_irq_disable();
#ifdef CONFIG_GTP_ESD_PROTECT
		gtp_esd_switch(i2c_client_point, SWITCH_OFF);
#endif
		return CMD_HEAD_LENGTH;
	} else if (9 == cmd_head2.wr) {	/* enable irq! */
		gtp_irq_enable();
#ifdef CONFIG_GTP_ESD_PROTECT
		gtp_esd_switch(i2c_client_point, SWITCH_ON);
#endif
		return CMD_HEAD_LENGTH;
	} else if (17 == cmd_head2.wr) {
		ret = copy_from_user(&cmd_head2.data[GTP_ADDR_LENGTH], &buff[CMD_HEAD_LENGTH], cmd_head2.data_len);
		if (ret)
			GTP_DEBUG("copy_from_user failed.");

		if (cmd_head2.data[GTP_ADDR_LENGTH]) {
			GTP_DEBUG("gtp enter rawdiff.");
			gtp_rawdiff_mode = true;
		} else {
			gtp_rawdiff_mode = false;
			GTP_DEBUG("gtp leave rawdiff.");
		}

		return CMD_HEAD_LENGTH;
	}
#ifdef UPDATE_FUNCTIONS
	else if (11 == cmd_head2.wr) {	/* Enter update mode! */
		if (FAIL == gup_enter_update_mode(gt_client))
			return FAIL;
	} else if (13 == cmd_head2.wr) {	/* Leave update mode! */
		gup_leave_update_mode();
	} else if (15 == cmd_head2.wr) {	/* Update firmware! */
		show_len = 0;
		total_len = 0;
		memset(cmd_head2.data, 0, cmd_head2.data_len + 1);
		memcpy(cmd_head2.data, &buff[CMD_HEAD_LENGTH], cmd_head2.data_len);
		GTP_DEBUG("update firmware, filename: %s", cmd_head2.data);
		if (FAIL == gup_update_proc((void *)cmd_head2.data))
			return FAIL;

	}
#endif
#ifdef CONFIG_GTP_HOTKNOT
	else if (19 == cmd_head2.wr) {	/* load subsystem */
		ret = copy_from_user(&cmd_head2.data[0], &buff[CMD_HEAD_LENGTH], cmd_head2.data_len);
		if (0 == cmd_head2.data[0]) {
			if (FAIL == gup_load_hotknot_fw())
				return FAIL;

		} else if (1 == cmd_head2.data[0]) {
			if (FAIL == gup_load_authorization_fw())
				return FAIL;

		} else if (2 == cmd_head2.data[0]) {
			if (FAIL == gup_recovery_touch())
				return FAIL;

		} else if (3 == cmd_head2.data[0]) {
			if (FAIL == gup_load_touch_fw(NULL))
				return FAIL;

		}
	}
#endif
#ifdef CONFIG_HOTKNOT_BLOCK_RW
	else if (21 == cmd_head2.wr) {
		u16 wait_hotknot_timeout = 0;
		u8 rqst_hotknot_state;

		ret = copy_from_user(&cmd_head2.data[GTP_ADDR_LENGTH], &buff[CMD_HEAD_LENGTH], cmd_head2.data_len);

		if (ret)
			GTP_ERROR("copy_from_user failed.");

		rqst_hotknot_state = cmd_head2.data[GTP_ADDR_LENGTH];
		wait_hotknot_state |= rqst_hotknot_state;
		wait_hotknot_timeout = (cmd_head2.data[GTP_ADDR_LENGTH + 1] << 8) + cmd_head2.data[GTP_ADDR_LENGTH + 2];
		GTP_DEBUG("Goodix tool received wait polling state:0x%x,timeout:%d, all wait state:0x%x",
			  rqst_hotknot_state, wait_hotknot_timeout, wait_hotknot_state);
		got_hotknot_state &= (~rqst_hotknot_state);
		/* got_hotknot_extra_state = 0; */
		switch (rqst_hotknot_state) {
			set_current_state(TASK_INTERRUPTIBLE);
		case HN_DEVICE_PAIRED:
			hotknot_paired_flag = 0;
			wait_event_interruptible(bp_waiter, force_wake_flag ||
						 rqst_hotknot_state == (got_hotknot_state & rqst_hotknot_state));
			wait_hotknot_state &= (~rqst_hotknot_state);
			if (rqst_hotknot_state != (got_hotknot_state & rqst_hotknot_state)) {
				GTP_ERROR("Wait 0x%x block polling waiter failed.", rqst_hotknot_state);
				force_wake_flag = 0;
				return FAIL;
			}
			break;
		case HN_MASTER_SEND:
		case HN_SLAVE_RECEIVED:
			wait_event_interruptible_timeout(bp_waiter, force_wake_flag ||
							 rqst_hotknot_state == (got_hotknot_state & rqst_hotknot_state),
							 wait_hotknot_timeout);
			wait_hotknot_state &= (~rqst_hotknot_state);
			if (rqst_hotknot_state == (got_hotknot_state & rqst_hotknot_state))
				return got_hotknot_extra_state;

			GTP_ERROR("Wait 0x%x block polling waiter timeout.", rqst_hotknot_state);
			force_wake_flag = 0;
			return FAIL;
		case HN_MASTER_DEPARTED:
		case HN_SLAVE_DEPARTED:
			wait_event_interruptible_timeout(bp_waiter, force_wake_flag ||
							 rqst_hotknot_state == (got_hotknot_state & rqst_hotknot_state),
							 wait_hotknot_timeout);
			wait_hotknot_state &= (~rqst_hotknot_state);
			if (rqst_hotknot_state != (got_hotknot_state & rqst_hotknot_state)) {
				GTP_ERROR("Wait 0x%x block polling waitor timeout.", rqst_hotknot_state);
				force_wake_flag = 0;
				return FAIL;
			}
			break;
		default:
			GTP_ERROR("Invalid rqst_hotknot_state in goodix_tool.");
			break;
		}
		force_wake_flag = 0;
	} else if (23 == cmd_head2.wr) {
		GTP_DEBUG("Manual wakeup all block polling waiter!");
		got_hotknot_state = 0;
		wait_hotknot_state = 0;
		force_wake_flag = 1;
		hotknot_paired_flag = 0;
		wake_up_interruptible(&bp_waiter);
	}
#endif
	return CMD_HEAD_LENGTH;
}

/*******************************************************
Function:
Goodix tool read function.
Input:
standard proc read function param.
Output:
Return read length.
********************************************************/
static ssize_t _hotknot_read(struct file *file, char __user *page, size_t size, loff_t *ppos)
{
	int ret;
	GTP_DEBUG_FUNC();

	if (gtp_resetting == 1) {
		GTP_DEBUG("[READ]tpd_halt =1 fail!");
		return FAIL;
	}
	if (*ppos) {
		*ppos = 0;
		return 0;
	}
	if (cmd_head2.wr % 2) {
		GTP_ERROR("[READ] invaild operator fail!");
		return FAIL;
	} else if (!cmd_head2.wr) {
		u16 len = 0;
		s16 data_len = 0;
		u16 loc = 0;

		if (1 == cmd_head2.flag) {
			if (FAIL == comfirm()) {
				GTP_ERROR("[READ]Comfirm fail!");
				return FAIL;
			}
		} else if (2 == cmd_head2.flag) {
			/* Need interrupt! */
		}

		memcpy(cmd_head2.data, cmd_head2.addr, cmd_head2.addr_len);

		GTP_DEBUG("[CMD HEAD DATA] ADDR:0x%02x%02x.", cmd_head2.data[0], cmd_head2.data[1]);
		GTP_DEBUG("[CMD HEAD ADDR] ADDR:0x%02x%02x.", cmd_head2.addr[0], cmd_head2.addr[1]);

		if (cmd_head2.delay)
			msleep(cmd_head2.delay);

		data_len = cmd_head2.data_len;

		while (data_len > 0) {
			if (data_len > DATA_LENGTH)
				len = DATA_LENGTH;
			else
				len = data_len;

			data_len -= DATA_LENGTH;

			if (tool_i2c_read(cmd_head2.data, len) <= 0) {
				GTP_ERROR("[READ]Read data failed!");
				return FAIL;
			}

			ret = copy_to_user(&page[loc], &cmd_head2.data[GTP_ADDR_LENGTH], len);
			loc += len;

			GTP_DEBUG_ARRAY(&cmd_head2.data[GTP_ADDR_LENGTH], len);
			GTP_DEBUG_ARRAY(page, len);
		}
	} else if (2 == cmd_head2.wr) {
		GTP_DEBUG("Return ic type:%s len:%d.", page, (s32) cmd_head2.data_len);
		*ppos += cmd_head2.data_len;
		return cmd_head2.data_len;
	} else if (4 == cmd_head2.wr) {
		page[0] = show_len >> 8;
		page[1] = show_len & 0xff;
		page[2] = total_len >> 8;
		page[3] = total_len & 0xff;
		*ppos += cmd_head2.data_len;
		return cmd_head2.data_len;
	} else if (6 == cmd_head2.wr) {
		/* Read error code! */
	} else if (8 == cmd_head2.wr) {	/* Read driver version */
		s32 tmp_len = strlen(GTP_DRIVER_VERSION);

		ret = copy_to_user(page, GTP_DRIVER_VERSION, tmp_len);
		page[tmp_len] = 0;
	}
	*ppos += cmd_head2.data_len;
	return cmd_head2.data_len;
}

static ssize_t hotknot_read(struct file *file, char __user *page, size_t size, loff_t *ppos)
{
	*ppos = 0;
	return _hotknot_read(file, page, size, ppos);
}
		Executive outcomes.0--success,non-0--fail.
*******************************************************/
static s32 gtp_send_cfg(struct i2c_client *client)
{
		s32 ret = 1;
#ifdef CONFIG_GTP_DRIVER_SEND_CFG
		s32 retry = 0;
        GTP_INFO("------> reay to send cfg");
		if (pnl_init_error) {
			GTP_INFO("Error occurred in init_panel, no config sent!");
			return 0;
		}

		GTP_DEBUG("Driver Send Config");
		for (retry = 0; retry < 5; retry++) {
			ret = gtp_i2c_write(client, config, GTP_CONFIG_MAX_LENGTH + GTP_ADDR_LENGTH);
			if (ret > 0) {
                GTP_INFO("------>send client (%x) config error", client->addr);
				break;
            }
		}
#endif
		return ret;
}

#ifdef CONFIG_GTP_CHARGER_DETECT
static int gtp_send_chr_cfg(struct i2c_client *client)
{
	s32 ret = 1;
#ifdef CONFIG_GTP_DRIVER_SEND_CFG
		s32 retry = 0;

	if (pnl_init_error) {
		GTP_INFO("Error occurred in init_panel, no config sent!");
		return 0;
	}

	GTP_INFO("Driver Send Config");
	for (retry = 0; retry < 5; retry++) {
		ret = gtp_i2c_write(client, gtp_charger_config, GTP_CONFIG_MAX_LENGTH + GTP_ADDR_LENGTH);
		if (ret > 0)
			break;

	}
#endif
	return ret;
}
#endif
/*******************************************************
Function:
		Read goodix touchscreen version function.
Input:
		client: i2c client struct.
		version:address to store version info
Output:
		Executive outcomes.0---succeed.
*******************************************************/
s32 gtp_read_version(struct i2c_client *client, u16 *version)
{
		s32 ret = -1;
		s32 i;
		u8 buf[8] = {GTP_REG_VERSION >> 8, GTP_REG_VERSION & 0xff};

		GTP_DEBUG_FUNC();
		ret = gtp_i2c_read(client, buf, sizeof(buf));
		if (ret < 0) {
			GTP_ERROR("GTP read version failed");
			return ret;
		}

		if (version)
			*version = (buf[7] << 8) | buf[6];

		tpd_info.vid = *version;
		tpd_info.pid = 0x00;

		for (i = 0; i < 4; i++) {
			if (buf[i + 2] < 0x30)
				break;

			tpd_info.pid |= ((buf[i + 2] - 0x30) << ((3 - i) * 4));
		}

		if (buf[5] == 0x00) {
			GTP_INFO("IC VERSION: %c%c%c_%02x%02x",
			buf[2], buf[3], buf[4], buf[7], buf[6]);
		} else {
			if (buf[5] == 'S' || buf[5] == 's')
				chip_gt9xxs = 1;

			GTP_INFO("IC VERSION:%c%c%c%c_%02x%02x",
			buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);
		}
		return ret;
}

/*******************************************************
Function:
		GTP initialize function.

Input:
		client: i2c client private struct.

Output:
		Executive outcomes.0---succeed.
*******************************************************/
s32 gtp_init_panel(struct i2c_client *client)
{
	s32 ret = 0;

#ifdef CONFIG_GTP_DRIVER_SEND_CFG
	s32 i;
	u8 check_sum = 0;
	u8 opr_buf[16];
	u8 sensor_id = 0;
	u8 retry = 0;
	u8 flash_cfg_version = 0;
	u8 drv_cfg_version = 0;

	u8 cfg_info_group0[] = CTP_CFG_GROUP0;
	u8 cfg_info_group1[] = CTP_CFG_GROUP1;
	u8 cfg_info_group2[] = CTP_CFG_GROUP2;
	u8 cfg_info_group3[] = CTP_CFG_GROUP3;
	u8 cfg_info_group4[] = CTP_CFG_GROUP4;
	u8 cfg_info_group5[] = CTP_CFG_GROUP5;
	u8 *send_cfg_buf[] = {cfg_info_group0, cfg_info_group1, cfg_info_group2,
		cfg_info_group3, cfg_info_group4, cfg_info_group5};
	u8 cfg_info_len[] = { CFG_GROUP_LEN(cfg_info_group0),
			CFG_GROUP_LEN(cfg_info_group1),
			CFG_GROUP_LEN(cfg_info_group2),
			CFG_GROUP_LEN(cfg_info_group3),
			CFG_GROUP_LEN(cfg_info_group4),
			CFG_GROUP_LEN(cfg_info_group5)};

#ifdef CONFIG_GTP_CHARGER_DETECT
	const u8 cfg_grp0_charger[] = GTP_CFG_GROUP0_CHARGER;
	const u8 cfg_grp1_charger[] = GTP_CFG_GROUP1_CHARGER;
	const u8 cfg_grp2_charger[] = GTP_CFG_GROUP2_CHARGER;
	const u8 cfg_grp3_charger[] = GTP_CFG_GROUP3_CHARGER;
	const u8 cfg_grp4_charger[] = GTP_CFG_GROUP4_CHARGER;
	const u8 cfg_grp5_charger[] = GTP_CFG_GROUP5_CHARGER;
	const u8 *cfgs_charger[] = {
		cfg_grp0_charger, cfg_grp1_charger, cfg_grp2_charger,
		cfg_grp3_charger, cfg_grp4_charger, cfg_grp5_charger
	};
	u8 cfg_lens_charger[] = {
						CFG_GROUP_LEN(cfg_grp0_charger),
						CFG_GROUP_LEN(cfg_grp1_charger),
						CFG_GROUP_LEN(cfg_grp2_charger),
						CFG_GROUP_LEN(cfg_grp3_charger),
						CFG_GROUP_LEN(cfg_grp4_charger),
						CFG_GROUP_LEN(cfg_grp5_charger)};
#endif

	GTP_DEBUG("Config Groups\' Lengths: %d, %d, %d, %d, %d, %d",
	cfg_info_len[0], cfg_info_len[1], cfg_info_len[2], cfg_info_len[3],
	cfg_info_len[4], cfg_info_len[5]);

	pnl_init_error = 0;
	if ((!cfg_info_len[1]) && (!cfg_info_len[2]) &&
	(!cfg_info_len[3]) && (!cfg_info_len[4]) && (!cfg_info_len[5])) {
		sensor_id = 0;
	} else {
#ifdef CONFIG_GTP_COMPATIBLE_MODE
		if (CHIP_TYPE_GT9F == gtp_chip_type)
			msleep(50);
#endif
		ret = gt9xx_double_readcheck(client, GTP_REG_SENSOR_ID, &sensor_id, 1);
		if (SUCCESS == ret) {

			while ((sensor_id == 0xff) && (retry++ < 3)) {
				msleep(100);
				ret = gt9xx_double_readcheck(client, GTP_REG_SENSOR_ID, &sensor_id, 1);
				GTP_ERROR("GTP sensor_ID read failed time %d.", retry);
			}

			if (sensor_id >= 0x06) {
				GTP_ERROR("Invalid sensor_id(0x%02X), No Config Sent!", sensor_id);
				pnl_init_error = 1;
				return -1;
			}
		} else {
			GTP_ERROR("Failed to get sensor_id, No config sent!");
			pnl_init_error = 1;
			return -1;
		}
		GTP_INFO("Sensor_ID: %d", sensor_id);
	}

		cfg_len = cfg_info_len[sensor_id];

		GTP_INFO("CTP_CONFIG_GROUP%d used, config length: %d", sensor_id, cfg_len);

		if (cfg_len < GTP_CONFIG_MIN_LENGTH) {
			GTP_ERROR("CTP_CONFIG_GROUP%d is INVALID CONFIG GROUP! NO Config Sent!", sensor_id);
			GTP_ERROR(" You need to check you header file CFG_GROUP section!");
			pnl_init_error = 1;
			return -1;
		}

#ifdef CONFIG_GTP_COMPATIBLE_MODE
	if (gtp_chip_type != CHIP_TYPE_GT9F) {
#else
	{
#endif
		ret = gt9xx_double_readcheck(client, GTP_REG_CONFIG_DATA, &opr_buf[0], 1);
		if (ret == SUCCESS) {
			GTP_DEBUG("CFG_CONFIG_GROUP%d Config Version: %d, 0x%02X; IC Config Version: %d, 0x%02X",
			sensor_id, send_cfg_buf[sensor_id][0], send_cfg_buf[sensor_id][0], opr_buf[0], opr_buf[0]);

			flash_cfg_version = opr_buf[0];
			drv_cfg_version = send_cfg_buf[sensor_id][0];/* backup	config version */
			if (flash_cfg_version < 90 && flash_cfg_version > drv_cfg_version) {
				send_cfg_buf[sensor_id][0] = 0x00;
                GTP_INFO("------> flash_cfg_verison = %d, drv_cfg_version = %d", flash_cfg_version, drv_cfg_version);
             }

		} else {
			GTP_ERROR("Failed to get ic config version!No config sent!");
			return -1;
		}
	}

		memset(&config[GTP_ADDR_LENGTH], 0, GTP_CONFIG_MAX_LENGTH);
		memcpy(&config[GTP_ADDR_LENGTH], send_cfg_buf[sensor_id], cfg_len);

#ifdef CONFIG_GTP_CUSTOM_CFG
		config[RESOLUTION_LOC]		 = (u8)GTP_MAX_WIDTH;
		config[RESOLUTION_LOC + 1] = (u8)(GTP_MAX_WIDTH>>8);
		config[RESOLUTION_LOC + 2] = (u8)GTP_MAX_HEIGHT;
		config[RESOLUTION_LOC + 3] = (u8)(GTP_MAX_HEIGHT>>8);

		if (GTP_INT_TRIGGER == 0)	/* RISING */
			config[TRIGGER_LOC] &= 0xfe;
		else if (GTP_INT_TRIGGER == 1)	/* FALLING */
			config[TRIGGER_LOC] |= 0x01;

#endif	/* CONFIG_GTP_CUSTOM_CFG */

		check_sum = 0;
		for (i = GTP_ADDR_LENGTH; i < cfg_len; i++)
			check_sum += config[i];

		config[cfg_len] = (~check_sum) + 1;

#ifdef CONFIG_GTP_CHARGER_DETECT
	GTP_DEBUG("Charger Config Groups Length: %d, %d, %d, %d, %d, %d", cfg_lens_charger[0],
	cfg_lens_charger[1], cfg_lens_charger[2], cfg_lens_charger[3], cfg_lens_charger[4], cfg_lens_charger[5]);

	memset(&gtp_charger_config[GTP_ADDR_LENGTH], 0, GTP_CONFIG_MAX_LENGTH);
	if (cfg_lens_charger[sensor_id] == cfg_len)
		memcpy(&gtp_charger_config[GTP_ADDR_LENGTH], cfgs_charger[sensor_id], cfg_len);

#ifdef CONFIG_GTP_CUSTOM_CFG
	gtp_charger_config[RESOLUTION_LOC] = (u8) GTP_MAX_WIDTH;
	gtp_charger_config[RESOLUTION_LOC + 1] = (u8) (GTP_MAX_WIDTH >> 8);
	gtp_charger_config[RESOLUTION_LOC + 2] = (u8) GTP_MAX_HEIGHT;
	gtp_charger_config[RESOLUTION_LOC + 3] = (u8) (GTP_MAX_HEIGHT >> 8);

	if (GTP_INT_TRIGGER == 0)	/* RISING	*/
		gtp_charger_config[TRIGGER_LOC] &= 0xfe;
	else if (GTP_INT_TRIGGER == 1) /* FALLING */
		gtp_charger_config[TRIGGER_LOC] |= 0x01;
#endif /* END CONFIG_GTP_CUSTOM_CFG */
	if (cfg_lens_charger[sensor_id] != cfg_len)
		memset(&gtp_charger_config[GTP_ADDR_LENGTH], 0, GTP_CONFIG_MAX_LENGTH);

	check_sum = 0;
	for (i = GTP_ADDR_LENGTH; i < cfg_len; i++)
		check_sum += gtp_charger_config[i];

	gtp_charger_config[cfg_len] = (~check_sum) + 1;

#endif /* END CONFIG_GTP_CHARGER_DETECT */

#else /* DRIVER NOT SEND CONFIG */
	cfg_len = GTP_CONFIG_MAX_LENGTH;
	ret = gtp_i2c_read(client, config, cfg_len + GTP_ADDR_LENGTH);
	if (ret < 0) {
		GTP_ERROR("Read Config Failed, Using DEFAULT Resolution & INT Trigger!");
		abs_x_max = GTP_MAX_WIDTH;
		abs_y_max = GTP_MAX_HEIGHT;
		int_type = GTP_INT_TRIGGER;
	}
#endif /* CONFIG_GTP_DRIVER_SEND_CFG */

	GTP_DEBUG_FUNC();
	if ((abs_x_max == 0) && (abs_y_max == 0)) {
		abs_x_max = (config[RESOLUTION_LOC + 1] << 8) + config[RESOLUTION_LOC];
		abs_y_max = (config[RESOLUTION_LOC + 3] << 8) + config[RESOLUTION_LOC + 2];
		int_type = (config[TRIGGER_LOC]) & 0x03;
	}

#ifdef CONFIG_GTP_COMPATIBLE_MODE
	if (CHIP_TYPE_GT9F == gtp_chip_type) {
		u8 have_key = 0;

		if (!memcmp(&gtp_touch_fw[4], "950", 3)) {
			driver_num = config[GTP_REG_MATRIX_DRVNUM - GTP_REG_CONFIG_DATA + 2];
			sensor_num = config[GTP_REG_MATRIX_SENNUM - GTP_REG_CONFIG_DATA + 2];
		} else {
			driver_num = (config[CFG_LOC_DRVA_NUM]&0x1F) + (config[CFG_LOC_DRVB_NUM]&0x1F);
			sensor_num = (config[CFG_LOC_SENS_NUM]&0x0F) + ((config[CFG_LOC_SENS_NUM]>>4)&0x0F);
		}

		have_key = config[GTP_REG_HAVE_KEY - GTP_REG_CONFIG_DATA + 2] & 0x01;	/* have key or not */
		if (1 == have_key)
			driver_num--;

		GTP_INFO("Driver * Sensor: %d * %d(Key: %d), X_MAX = %d, Y_MAX = %d, TRIGGER = 0x%02x",
		driver_num, sensor_num, have_key, abs_x_max, abs_y_max, int_type);
	} else
#endif
	{
#ifdef CONFIG_GTP_DRIVER_SEND_CFG
		ret = gtp_send_cfg(client);
		if (ret < 0)
			GTP_ERROR("Send config error.");

#ifdef CONFIG_GTP_COMPATIBLE_MODE
		if (gtp_chip_type != CHIP_TYPE_GT9F) {
#else
		{
#endif
			/* for resume to send config */
			if (flash_cfg_version < 90 && flash_cfg_version > drv_cfg_version) {
				config[GTP_ADDR_LENGTH] = drv_cfg_version;
				check_sum = 0;
				for (i = GTP_ADDR_LENGTH; i < cfg_len; i++)
					check_sum += config[i];

				config[cfg_len] = (~check_sum) + 1;
                GTP_INFO("------> flash_cfg_verison = %d, drv_cfg_version = %d", flash_cfg_version, drv_cfg_version);
            
			}
		}
#endif
		GTP_INFO("X_MAX = %d, Y_MAX = %d, TRIGGER = 0x%02x",
		abs_x_max, abs_y_max, int_type);
	}

	msleep(20);
	return 0;
}

static s8 gtp_i2c_test(struct i2c_client *client)
{

		u8 retry = 0;
		s8 ret = -1;
		u32 hw_info = 0;

		GTP_DEBUG_FUNC();

		while (retry++ < 5) {
			ret = i2c_read_bytes(client, GTP_REG_HW_INFO, (u8 *)&hw_info, sizeof(hw_info));

			if ((!ret) && (hw_info == 0x00900600))/* 20121212 */
				return ret;

			GTP_ERROR("GTP_REG_HW_INFO : %08X", hw_info);
			GTP_ERROR("GTP i2c test failed time %d.", retry);
			msleep(20);
		}

		return -1;
}

/*******************************************************
Function:
		Set INT pin	as input for FW sync.
Note:
	If the INT is high, It means there is pull up resistor attached on the INT pin.
	Pull low the INT pin manaully for FW sync.
*******************************************************/
void gtp_int_sync(s32 ms)
{
#if 0	
	tpd_gpio_output(GTP_IRQ_GPIO, 1);//0
	msleep(ms);
	gtp_gpio_input(GTP_IRQ_GPIO);
#else
	tpd_gpio_output(GTP_INT_PORT, 1);
	tpd_gpio_as_int(GTP_INT_PORT);
	msleep(50);
#endif	
}

void gtp_reset_guitar(struct i2c_client *client, s32 ms)
{
	GTP_INFO("GTP RESET!\n");
	GTP_INFO("client->addr = 0x%x\n", client->addr);

	if (gtp_resetting)
		return;
	gtp_resetting = 1;

#if 0	
	tpd_gpio_output(GTP_RST_GPIO, 0);
	msleep(ms);

	
	/* select client address */
	tpd_gpio_output(GTP_IRQ_GPIO, client->addr == 0x14);
	msleep(2);

	tpd_gpio_output(GTP_RST_GPIO, 1);
	msleep(6);/* must >= 6ms */
#else
	tpd_gpio_output(GTP_RST_PORT, 0);
	msleep(ms);
	
	/* select client address */
	tpd_gpio_output(GTP_INT_PORT, client->addr == 0x14);
	msleep(2);
	
	tpd_gpio_output(GTP_RST_PORT, 1);
	msleep(6);	
#endif
	
	
#ifdef CONFIG_GTP_COMPATIBLE_MODE
	if (CHIP_TYPE_GT9F == gtp_chip_type) {
		gtp_resetting = 0;
		return;
	}
#endif

	gtp_int_sync(50);
#ifdef CONFIG_GTP_ESD_PROTECT
	gtp_init_ext_watchdog(i2c_client_point);
#endif
	gtp_resetting = 0;
}

static int tpd_power_on(struct i2c_client *client)
{
		int ret = 0;
		int reset_count = 0;

reset_proc:
#if 0
	tpd_gpio_output(GTP_IRQ_GPIO, 0);
	tpd_gpio_output(GTP_RST_GPIO, 0);
	msleep(20);
#else
	tpd_gpio_output(GTP_RST_PORT, 0);
	tpd_gpio_output(GTP_INT_PORT, 0);
	msleep(20);	
#endif	
	/* power on, need confirm with SA */
	GTP_INFO("turn on power reg-vgp2\n");

#if 0	
	ret = regulator_enable(tpd->reg);
	if (ret != 0)
		TPD_DMESG("Failed to enable reg-vgp2: %d\n", ret);
#else
	tp_power_on(1);	
#endif	

	gtp_reset_guitar(client, 20);

#ifdef CONFIG_GTP_COMPATIBLE_MODE
	gtp_get_chip_type(client);

	if (CHIP_TYPE_GT9F == gtp_chip_type) {
		ret = gup_fw_download_proc(NULL, GTP_FL_FW_BURN);
		if (FAIL == ret) {
			GTP_ERROR("[tpd_power_on]Download fw failed.");
			if (reset_count++ < TPD_MAX_RESET_COUNT)
				goto reset_proc;
			else
				return ret;
		}

		ret = gtp_fw_startup(client);
		if (FAIL == ret) {
			GTP_ERROR("[tpd_power_on]Startup fw failed.");
			if (reset_count++ < TPD_MAX_RESET_COUNT)
				goto reset_proc;
			else
				return -1;
		}
	} else
#endif
	{
		ret = gtp_i2c_test(client);
		if (ret < 0) {
			GTP_ERROR("I2C communication ERROR!");

			if (reset_count < TPD_MAX_RESET_COUNT) {
				reset_count++;
				goto reset_proc;
			}
		}
	}

	return ret;
}

#if defined(CONFIG_GTP_COMPATIBLE_MODE) || defined(CONFIG_GTP_HOTKNOT)
u8 gtp_fw_startup(struct i2c_client *client)
{
		u8 wr_buf[4];
		s32 ret = 0;

		/* init sw WDT */
		wr_buf[0] = 0xAA;
		ret = i2c_write_bytes(client, 0x8041, wr_buf, 1);
		if (ret < 0) {
			GTP_ERROR("I2C error to firmware startup.");
			return FAIL;
		}
		/* release SS51 & DSP */
		wr_buf[0] = 0x00;
		i2c_write_bytes(client, 0x4180, wr_buf, 1);

		/* int sync */
		gtp_int_sync(20);

		/* check fw run status */
		i2c_read_bytes(client, 0x8041, wr_buf, 1);
		if (0xAA == wr_buf[0]) {
			GTP_ERROR("IC works abnormally,startup failed.");
			return FAIL;
		}
		GTP_DEBUG("IC works normally,Startup success.");
		wr_buf[0] = 0xAA;
		i2c_write_bytes(client, 0x8041, wr_buf, 1);
		return SUCCESS;
}
#endif

/* **************** For GT9XXF Start *********************/
#ifdef CONFIG_GTP_COMPATIBLE_MODE

void gtp_get_chip_type(struct i2c_client *client)
{
		u8 opr_buf[10] = {0x00};
		s32 ret = 0;

		msleep(20);
		ret = gt9xx_double_readcheck(client, GTP_REG_CHIP_TYPE, opr_buf, 10);
		if (FAIL == ret) {
			GTP_ERROR("Failed to get chip-type, set chip type default: GOODIX_GT9");
			gtp_chip_type = CHIP_TYPE_GT9;
			return;
		}

		if (!memcmp(opr_buf, "GOODIX_GT9", 10)) {
			GTP_INFO("Chip Type: %s", (gtp_chip_type == CHIP_TYPE_GT9) ? "GOODIX_GT9" : "GOODIX_GT9F");
			gtp_chip_type = CHIP_TYPE_GT9;
		} else {/* GT9XXF */
			gtp_chip_type = CHIP_TYPE_GT9F;
			GTP_INFO("Chip Type: %s", (gtp_chip_type == CHIP_TYPE_GT9) ? "GOODIX_GT9" : "GOODIX_GT9F");
		}
		gtp_chip_type = CHIP_TYPE_GT9; /* for test */
		GTP_INFO("Chip Type: %s", (gtp_chip_type == CHIP_TYPE_GT9) ? "GOODIX_GT9" : "GOODIX_GT9F");
}

static u8 gtp_bak_ref_proc(struct i2c_client *client, u8 mode)
{
		s32 i = 0;
		s32 j = 0;
		s32 ret = 0;
		struct file *flp = NULL;
		u8 *refp = NULL;
		u32 ref_len = 0;
		u32 ref_seg_len = 0;
		s32 ref_grps = 0;
		s32 ref_chksum = 0;
		u16 tmp = 0;

		mm_segment_t old_fs;

		GTP_DEBUG("[gtp_bak_ref_proc]Driver:%d,Sensor:%d.", driver_num, sensor_num);
		
		old_fs = get_fs();
		set_fs(KERNEL_DS);

		/* check file-system mounted */
		GTP_DEBUG("[gtp_bak_ref_proc]Waiting for FS %d", gtp_ref_retries);
		if (gup_check_fs_mounted("/data") == FAIL) {
			GTP_DEBUG("[gtp_bak_ref_proc]/data not mounted");
			if (gtp_ref_retries++ < GTP_CHK_FS_MNT_MAX){
				set_fs(old_fs);
				return FAIL;
			}

		} else {
			GTP_DEBUG("[gtp_bak_ref_proc]/data mounted !!!!");
		}

		if (!memcmp(&gtp_touch_fw[4], "950", 3)) {
			ref_seg_len = (driver_num * (sensor_num - 1) + 2) * 2;
			ref_grps = 6;
			ref_len =	ref_seg_len * 6;	/* for GT950, backup-reference for six segments */
		} else {
			ref_len = driver_num*(sensor_num-2)*2 + 4;
			ref_seg_len = ref_len;
			ref_grps = 1;
		}

		refp = kzalloc(ref_len, GFP_KERNEL);
		if (refp == NULL) {
			GTP_ERROR("[gtp_bak_ref_proc]Alloc memory for ref failed.use default ref");
			set_fs(old_fs);
			return FAIL;
		}
				
		memset(refp, 0, ref_len);
		if (gtp_ref_retries >= GTP_CHK_FS_MNT_MAX) {
			for (j = 0; j < ref_grps; ++j)
				refp[ref_seg_len + j * ref_seg_len - 1] = 0x01;

			ret = i2c_write_bytes(client, 0x99D0, refp, ref_len);
			if (-1 == ret) {
				GTP_ERROR("[gtp_bak_ref_proc]Write ref i2c error.");
				ret = FAIL;
			}

			GTP_ERROR("[gtp_bak_ref_proc]Bak file or path is not exist,send default ref.");
			ret = SUCCESS;
			goto exit_ref_proc;
		}



		/* get ref file data */
		flp = filp_open(GTP_BAK_REF_PATH, O_RDWR | O_CREAT, 0666);
		if (IS_ERR(flp)) {
			GTP_ERROR("[gtp_bak_ref_proc]Ref File not found!Creat ref file.");
			/* flp->f_op->llseek(flp, 0, SEEK_SET); */
			/* flp->f_op->write(flp, (char *)refp, ref_len, &flp->f_pos); */
			gtp_ref_retries++;
			ret = FAIL;
			goto exit_ref_proc;
		} else if (GTP_BAK_REF_SEND == mode) {
			flp->f_op->llseek(flp, 0, SEEK_SET);
			ret = flp->f_op->read(flp, (char *)refp, ref_len, &flp->f_pos);
			if (ret < 0) {
				GTP_ERROR("[gtp_bak_ref_proc]Read ref file failed.");
				memset(refp, 0, ref_len);
			}
		}

		if (GTP_BAK_REF_STORE == mode) {
			ret = i2c_read_bytes(client, 0x99D0, refp, ref_len);
			if (-1 == ret) {
				GTP_ERROR("[gtp_bak_ref_proc]Read ref i2c error.");
				ret = FAIL;
				goto exit_ref_proc;
			}
			flp->f_op->llseek(flp, 0, SEEK_SET);
			flp->f_op->write(flp, (char *)refp, ref_len, &flp->f_pos);
		} else {
			/* checksum ref file */
			for (j = 0; j < ref_grps; ++j) {
				ref_chksum = 0;
				for (i = 0; i < ref_seg_len-2; i += 2)
					ref_chksum += ((refp[i + j * ref_seg_len]<<8) + refp[i + 1 + j * ref_seg_len]);

				GTP_DEBUG("[gtp_bak_ref_proc]Calc ref chksum:0x%04X", ref_chksum&0xFF);
				tmp = ref_chksum + (refp[ref_seg_len + j * ref_seg_len - 2]<<8) +
				refp[ref_seg_len + j * ref_seg_len - 1];
				if (1 != tmp) {
					GTP_DEBUG("[gtp_bak_ref_proc]Ref file chksum error,use default ref");
					memset(&refp[j * ref_seg_len], 0, ref_seg_len);
					refp[ref_seg_len - 1 + j * ref_seg_len] = 0x01;
				} else {
					if (j == (ref_grps - 1))
						GTP_DEBUG("[gtp_bak_ref_proc]Ref file chksum success.");
				}
			}

			ret = i2c_write_bytes(client, 0x99D0, refp, ref_len);
			if (-1 == ret) {
				GTP_ERROR("[gtp_bak_ref_proc]Write ref i2c error.");
				ret = FAIL;
				goto exit_ref_proc;
			}
		}

		ret = SUCCESS;

exit_ref_proc:
	kfree(refp);
	set_fs(old_fs);
	if (flp && !IS_ERR(flp))
		filp_close(flp, NULL);
	return ret;
}

static void gtp_recovery_reset(struct i2c_client *client)
{
	mutex_lock(&i2c_access);
	if (tpd_halt == 0) {
#ifdef CONFIG_GTP_ESD_PROTECT
		gtp_esd_switch(client, SWITCH_OFF);
#endif
		force_reset_guitar();
#ifdef CONFIG_GTP_ESD_PROTECT
		gtp_esd_switch(client, SWITCH_ON);
#endif
	}
	mutex_unlock(&i2c_access);
}

static u8 gtp_check_clk_legality(void)
{
	u8 i = 0;
	u8 clk_chksum = gtp_clk_buf[5];

	for (i = 0; i < 5; i++) {
		if ((gtp_clk_buf[i] < 50) || (gtp_clk_buf[i] > 120) ||
		(gtp_clk_buf[i] != gtp_clk_buf[0]))
			break;

		clk_chksum += gtp_clk_buf[i];
	}

	if ((i == 5) && (clk_chksum == 0)) {
		GTP_INFO("Clk ram legality check success");
		return SUCCESS;
	}
	GTP_ERROR("main clock freq in clock buf is wrong");
	return FAIL;
}

static u8 gtp_main_clk_proc(struct i2c_client *client)
{
		s32 ret = 0;
		u8	i = 0;
		u8	clk_cal_result = 0;
		u8	clk_chksum = 0;
		struct file *flp = NULL;

		mm_segment_t old_fs1;
		old_fs1 = get_fs();
		set_fs(KERNEL_DS);
		
		/* check clk legality */
		ret = gtp_check_clk_legality();
		if (SUCCESS == ret)
			goto send_main_clk;

		GTP_DEBUG("[gtp_main_clk_proc]Waiting for FS %d", gtp_ref_retries);
		if (gup_check_fs_mounted("/data") == FAIL) {
			GTP_DEBUG("[gtp_main_clk_proc]/data not mounted");
			if (gtp_clk_retries++ < GTP_CHK_FS_MNT_MAX){
				set_fs(old_fs1);
				return FAIL;
			}
			GTP_ERROR("[gtp_main_clk_proc]Wait for file system timeout,need cal clk");
		} else {
			GTP_DEBUG("[gtp_main_clk_proc]/data mounted !!!!");
			flp = filp_open(GTP_MAIN_CLK_PATH, O_RDWR | O_CREAT, 0666);
			if (!IS_ERR(flp)) {
				flp->f_op->llseek(flp, 0, SEEK_SET);
				ret = flp->f_op->read(flp, (char *)gtp_clk_buf, 6, &flp->f_pos);
				if (ret > 0) {
					ret = gtp_check_clk_legality();
					if (SUCCESS == ret) {
						GTP_DEBUG("[gtp_main_clk_proc]Open & read & check clk file success.");
						goto send_main_clk;
					}
				}
			}
			GTP_ERROR("[gtp_main_clk_proc]Check clk file failed,need cal clk");
		}

		/* cal clk */
#ifdef CONFIG_GTP_ESD_PROTECT
		gtp_esd_switch(client, SWITCH_OFF);
#endif
		clk_cal_result = gup_clk_calibration();
		force_reset_guitar();
		GTP_DEBUG("clk cal result:%d", clk_cal_result);

#ifdef CONFIG_GTP_ESD_PROTECT
		gtp_esd_switch(client, SWITCH_ON);
#endif

		if (clk_cal_result < 50 || clk_cal_result > 120) {
			GTP_ERROR("[gtp_main_clk_proc]cal clk result is illegitimate");
			ret = FAIL;
			goto exit_clk_proc;
		}

		for (i = 0; i < 5; i++) {
			gtp_clk_buf[i] = clk_cal_result;
			clk_chksum += gtp_clk_buf[i];
		}
		gtp_clk_buf[5] = 0 - clk_chksum;

		if (IS_ERR(flp)) {
			flp = filp_open(GTP_MAIN_CLK_PATH, O_RDWR | O_CREAT, 0666);
		} else {
			flp->f_op->llseek(flp, 0, SEEK_SET);
			flp->f_op->write(flp, (char *)gtp_clk_buf, 6, &flp->f_pos);
		}

send_main_clk:

		ret = i2c_write_bytes(client, 0x8020, gtp_clk_buf, 6);
		if (-1 == ret) {
			GTP_ERROR("[gtp_main_clk_proc]send main clk i2c error!");
			ret = FAIL;
			goto exit_clk_proc;
		}

		ret = SUCCESS;

exit_clk_proc:
		set_fs(old_fs1);
		if (flp && !IS_ERR(flp))
			filp_close(flp, NULL);

		return ret;
}

#endif
/* ************* For GT9XXF End ***********************/

static const struct file_operations gt_upgrade_proc_fops = {
		.write = gt91xx_config_write_proc,
		.read = gt91xx_config_read_proc
};
#if 1
static int tpd_irq_registration(void)
{
	struct device_node *node = NULL;
	unsigned long irqf_val = 0;
	int ret = 0;

#if 0	
	node = of_find_compatible_node(NULL, NULL, "mediatek,cap_touch");//0704
	// node = of_find_matching_node(NULL, touch_of_match);
	GTP_ERROR("\n tpd_irq_registration \n");
	if (node) {
		/*touch_irq = gpio_to_irq(tpd_int_gpio);*/
		touch_irq = irq_of_parse_and_map(node, 0);

		irqf_val = !int_type ? IRQF_TRIGGER_RISING : IRQF_TRIGGER_FALLING;

		ret = request_irq(touch_irq, tpd_interrupt_handler,
				IRQF_TRIGGER_FALLING/*irqf_val*/, "TOUCH_PANEL-eint"/*TPD_DEVICE*/, NULL);
		if (ret < 0)
			GTP_ERROR("tpd request_irq IRQ LINE NOT AVAILABLE!.");
		else
			irq_enabled = true;
	} else {
		GTP_ERROR("[%s] tpd request_irq can not find touch eint device node!.", __func__);
	}
#else	
	node = of_find_matching_node(NULL, tpd_of_match);
	GTP_ERROR("\n tpd_irq_registration \n");	
	if (node) {
		touch_irq = irq_of_parse_and_map(node, 0);
		
		irqf_val = !int_type ? IRQF_TRIGGER_RISING : IRQF_TRIGGER_FALLING;
		
		ret = request_irq(touch_irq,
				(irq_handler_t)tpd_interrupt_handler,
				  /*IRQF_TRIGGER_FALLING*/irqf_val, "TOUCH_PANEL-eint"/*TPD_DEVICE*/, NULL);
		if (ret < 0)
			GTP_ERROR("tpd request_irq IRQ LINE NOT AVAILABLE!.");
		else
			irq_enabled = true;
	} else {
		GTP_ERROR("[%s] tpd request_irq can not find touch eint device node!.", __func__);
	}
#endif
	
	return ret;
}
#endif

static s32 tpd_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
		u16 version_info;
#ifdef CONFIG_GTP_HAVE_TOUCH_KEY
		s32 idx = 0;
#endif
		s32 err = 0;
		s32 ret = 0;
#ifdef CONFIG_GTP_PROXIMITY
		struct hwmsen_object obj_ps;
#endif

	client->addr =0x5d;//add by heliang 20180426
/*	tpd_gpio_output(GTP_RST_GPIO, 1);
	tpd_gpio_output(GTP_IRQ_GPIO, 0);*/

	printk("[elink] %s:%d\n", __func__, __LINE__);
	i2c_client_point = client;
	// of_get_gt9xx_platform_data(&client->dev);

	// printk("[elink] %s:%d\n", __func__, __LINE__);
	// ret = gtp_get_gpio_res();
	// if (ret < 0) {
		// GTP_ERROR("Failed to get gpio resources");
		// return ret;
	// }

	printk("[elink] %s:%d\n", __func__, __LINE__);
	ret = tpd_power_on(client);
	if (ret < 0) {
		GTP_ERROR("I2C communication ERROR!");
		goto out;
	}

	printk("[elink] %s:%d\n", __func__, __LINE__);
	ret = gtp_read_version(client, &version_info);
	if (ret < 0) {
		GTP_ERROR("Read version failed.");
		goto out;
	}

	printk("[elink] %s:%d\n", __func__, __LINE__);
	ret = gtp_init_panel(client);
	if (ret < 0) {
		GTP_ERROR("GTP init panel failed.");
	}
	GTP_DEBUG("gtp_init_panel success");
	/* Create proc file system */
	gt91xx_config_proc = proc_create(GT91XX_CONFIG_PROC_FILE, 0660, NULL, &gt_upgrade_proc_fops);
	if (gt91xx_config_proc == NULL) {
		GTP_ERROR("create_proc_entry %s failed", GT91XX_CONFIG_PROC_FILE);
	}

#ifdef CONFIG_GTP_CREATE_WR_NODE
	init_wr_node(client);
#endif

	thread = kthread_run(touch_event_handler, 0, TPD_DEVICE);
	if (IS_ERR(thread)) {
		err = PTR_ERR(thread);
		GTP_ERROR(TPD_DEVICE " failed to create kernel thread: %d", err);
	}

#ifdef CONFIG_GTP_HAVE_TOUCH_KEY
	for (idx = 0; idx < GTP_MAX_KEY_NUM; idx++)
		input_set_capability(tpd->dev, EV_KEY, touch_key_array[idx]);

#endif
#ifdef CONFIG_GTP_GESTURE_WAKEUP
	gtp_extents_init();
	input_set_capability(tpd->dev, EV_KEY, KEY_F2);
	input_set_capability(tpd->dev, EV_KEY, KEY_F3);
#endif

#ifdef CONFIG_GTP_WITH_PEN
	/* pen support */
	__set_bit(BTN_TOOL_PEN, tpd->dev->keybit);
	__set_bit(INPUT_PROP_DIRECT, tpd->dev->propbit);
#endif

	msleep(50);
	tpd_irq_registration();
	// gtp_irq_enable();

#ifdef CONFIG_GTP_ESD_PROTECT
	gtp_esd_switch(client, SWITCH_ON);
#endif

#ifdef CONFIG_GTP_AUTO_UPDATE
	ret = gup_init_update_proc(client);
	if (ret < 0)
		GTP_ERROR("Create update thread error.");
#endif

#ifdef CONFIG_GTP_PROXIMITY
	/* obj_ps.self = cm3623_obj; */
	obj_ps.polling = 0;				 /* 0--interrupt mode;1--polling mode; */
	obj_ps.sensor_operate = tpd_ps_operate;

	err = hwmsen_attach(ID_PROXIMITY, &obj_ps);
	if (err)
		GTP_ERROR("hwmsen attach fail, return:%d.", err);
#endif

	tpd_load_status = 1;
	GTP_INFO("%s, success run Done", __func__);
	printk("[elink] %s:%d\n", __func__, __LINE__);
	return 0;
out:
	gtp_free_gpio_res();
	return -1;
}

static irqreturn_t tpd_interrupt_handler(int irq, void *dev_id)
{
	TPD_DEBUG_PRINT_INT;
	
	if (irq_enabled) {
		irq_enabled = false;
		disable_irq_nosync(touch_irq);
	}
	
	tpd_flag = 1;
	wake_up_interruptible(&waiter);
	
	return IRQ_HANDLED;
}
static int tpd_i2c_remove(struct i2c_client *client)
{
#ifdef CONFIG_GTP_CREATE_WR_NODE
	uninit_wr_node();
#endif

#ifdef CONFIG_GTP_GESTURE_WAKEUP
	gtp_extents_exit();
#endif

#ifdef CONFIG_GTP_ESD_PROTECT
	destroy_workqueue(gtp_workqueue);
#endif

	gtp_free_gpio_res();
	return 0;
}
#if (defined(CONFIG_GTP_ESD_PROTECT) || defined(CONFIG_GTP_COMPATIBLE_MODE))
void force_reset_guitar(void)
{
	s32 i = 0;
	s32 ret = 0;

	if (gtp_resetting || (gtp_loading_fw == 1))
		return;

	GTP_INFO("force_reset_guitar");
	gtp_irq_disable();

#if 0	
	tpd_gpio_output(GTP_RST_GPIO, 0);
	tpd_gpio_output(GTP_IRQ_GPIO, 0);
#else
	tpd_gpio_output(GTP_RST_PORT, 0);
	tpd_gpio_output(GTP_INT_PORT, 0);	
#endif
	
	/* Power off TP */
#if 0	
	ret = regulator_disable(tpd->reg);
	if (ret != 0)
		TPD_DMESG("Failed to disable reg-vgp6: %d\n", ret);

	msleep(30);
	/* Power on TP */
	ret = regulator_enable(tpd->reg);
	if (ret != 0)
		TPD_DMESG("Failed to enable reg-vgp6: %d\n", ret);
	msleep(30);
#else
	tp_power_on(0);
	msleep(30);
	tp_power_on(1);
#endif
	
	for (i = 0; i < 5; i++) {
		gtp_reset_guitar(i2c_client_point, 20);

#ifdef CONFIG_GTP_COMPATIBLE_MODE
		if (CHIP_TYPE_GT9F == gtp_chip_type) {
			/* check code ram */
			ret = gup_fw_download_proc(NULL, GTP_FL_ESD_RECOVERY);
			if (FAIL == ret) {
				GTP_ERROR("[force_reset_guitar]Check & repair fw failed.");
				continue;
			}

			ret = gtp_fw_startup(i2c_client_point);
			if (FAIL == ret) {
				GTP_ERROR("[force_reset_guitar]Startup fw failed.");
				continue;
			}
		} else
#endif
		{
			/* Send config */
			ret = gtp_send_cfg(i2c_client_point);
			if (ret < 0)
				continue;
		}
		break;
	}

	gtp_irq_enable();
}
#endif

#ifdef CONFIG_GTP_ESD_PROTECT
static s32 gtp_init_ext_watchdog(struct i2c_client *client)
{
		u8 opr_buffer[2] = {0xAA};

		GTP_DEBUG("Init external watchdog.");
		return i2c_write_bytes(client, 0x8041, opr_buffer, 1);
}

void gtp_esd_switch(struct i2c_client *client, s32 on)
{
	spin_lock(&esd_lock);
	if (SWITCH_ON == on)		 /* switch on esd */ {
		if (!esd_running) {
			esd_running = 1;
			spin_unlock(&esd_lock);
			GTP_INFO("Esd started");
			queue_delayed_work(gtp_workqueue, &gtp_esd_check_work, clk_tick_cnt);
		} else {
			spin_unlock(&esd_lock);
		}
	} else {		/* switch off esd */
		if (esd_running) {
			esd_running = 0;
			spin_unlock(&esd_lock);
			GTP_INFO("Esd cancelled");
			cancel_delayed_work_sync(&gtp_esd_check_work);
		} else {
			spin_unlock(&esd_lock);
		}
	}
}


static void gtp_esd_check_func(struct work_struct *work)
{
		s32 i = 0;
		s32 ret = -1;
		u8 esd_buf[3] = {0x00};

		if (tpd_halt) {
			GTP_INFO("Esd suspended!");
			return;
		}
		if (1 == gtp_loading_fw) {
			GTP_INFO("Load FW process is running");
			return;
		}
		for (i = 0; i < 3; i++) {
			ret = i2c_read_bytes_non_dma(i2c_client_point, 0x8040, esd_buf, 2);

			GTP_DEBUG("[Esd]0x8040 = 0x%02X, 0x8041 = 0x%02X", esd_buf[0], esd_buf[1]);
			if (ret < 0) {
				/* IIC communication problem */
				continue;
			} else {
				if ((esd_buf[0] == 0xAA) || (esd_buf[1] != 0xAA)) {
					u8 chk_buf[2] = {0x00};

					i2c_read_bytes_non_dma(i2c_client_point, 0x8040, chk_buf, 2);
					GTP_DEBUG("[Check]0x8040 = 0x%02X, 0x8041 = 0x%02X", chk_buf[0], chk_buf[1]);

					if ((chk_buf[0] == 0xAA) || (chk_buf[1] != 0xAA)) {
						i = 3;					/* jump to reset guitar */
						break;
					}
					continue;
				} else {
					/* IC works normally, Write 0x8040 0xAA, feed the watchdog */
					esd_buf[0] = 0xAA;
					i2c_write_bytes_non_dma(i2c_client_point, 0x8040, esd_buf, 1);

					break;
				}
			}
		}

		if (i >= 3) {
#ifdef CONFIG_GTP_COMPATIBLE_MODE
			if ((CHIP_TYPE_GT9F == gtp_chip_type) && (1 == rqst_processing)) {
				GTP_INFO("Request Processing, no reset guitar.");
			} else
#endif
			{
				GTP_INFO("IC works abnormally! Process reset guitar.");
				memset(esd_buf, 0x01, sizeof(esd_buf));
				i2c_write_bytes(i2c_client_point, 0x4226, esd_buf, sizeof(esd_buf));
				msleep(50);
				force_reset_guitar();
			}
		}

		if (!tpd_halt && esd_running)
			queue_delayed_work(gtp_workqueue, &gtp_esd_check_work, clk_tick_cnt);
		else
			GTP_INFO("Esd suspended!");

		return;
}
#endif
static int tpd_history_x = 0, tpd_history_y;
static void tpd_down(s32 x, s32 y, s32 size, s32 id)
{
	if ((!size) && (!id)) {
		input_report_abs(tpd->dev, ABS_MT_PRESSURE, 100);
		input_report_abs(tpd->dev, ABS_MT_TOUCH_MAJOR, 100);
	} else {
		input_report_abs(tpd->dev, ABS_MT_PRESSURE, size);
		input_report_abs(tpd->dev, ABS_MT_TOUCH_MAJOR, size);
		/* track id Start 0 */
		input_report_abs(tpd->dev, ABS_MT_TRACKING_ID, id);
	}

	input_report_key(tpd->dev, BTN_TOUCH, 1);
	input_report_abs(tpd->dev, ABS_MT_POSITION_X, x);
	input_report_abs(tpd->dev, ABS_MT_POSITION_Y, y);
	input_mt_sync(tpd->dev);
	TPD_DEBUG_SET_TIME;
	TPD_EM_PRINT(x, y, x, y, id, 1);
	tpd_history_x = x;
	tpd_history_y = y;

	/* MMProfileLogEx(MMP_TouchPanelEvent, MMProfileFlagPulse, 1, x+y); */
	if (tpd_dts_data.use_tpd_button) {
		if (FACTORY_BOOT == get_boot_mode() || RECOVERY_BOOT == get_boot_mode())
			tpd_button(x, y, 1);
	}
}

static void tpd_up(s32 x, s32 y, s32 id)
{
		/* input_report_abs(tpd->dev, ABS_MT_PRESSURE, 0); */
		input_report_key(tpd->dev, BTN_TOUCH, 0);
		/* input_report_abs(tpd->dev, ABS_MT_TOUCH_MAJOR, 0); */
		input_mt_sync(tpd->dev);
		TPD_DEBUG_SET_TIME;
		TPD_EM_PRINT(tpd_history_x, tpd_history_y, tpd_history_x, tpd_history_y, id, 0);
		tpd_history_x = 0;
		tpd_history_y = 0;
		/* MMProfileLogEx(MMP_TouchPanelEvent, MMProfileFlagPulse, 0, x+y); */

	if (tpd_dts_data.use_tpd_button) {
		if (FACTORY_BOOT == get_boot_mode() || RECOVERY_BOOT == get_boot_mode())
			tpd_button(x, y, 0);
	}
}
#ifdef CONFIG_GTP_CHARGER_DETECT
static void gtp_charger_switch(s32 dir_update)
{
		u32 chr_status = 0;
		u8 chr_cmd[3] = {0x80, 0x40};
		static u8 chr_pluggedin;
		int ret = 0;

#ifdef MT6573
		chr_status = *(u32 *)CHR_CON0;
		chr_status &= (1 << 13);
#else	 /* ( defined(MT6575) || defined(MT6577) || defined(MT6589) ) */
		chr_status = upmu_is_chr_det();
#endif

	if (chr_status) {		 /* charger plugged in */
		if (!chr_pluggedin || dir_update) {
			chr_cmd[2] = 6;
			ret = gtp_i2c_write(i2c_client_point, chr_cmd, 3);
			if (ret > 0) {
				GTP_INFO("Update status for Charger Plugin");
				if (gtp_send_chr_cfg(i2c_client_point) < 0)
					GTP_ERROR("Send charger config failed.");
				else
					GTP_DEBUG("Send charger config.");

			}
			chr_pluggedin = 1;
		}
	} else {						/* charger plugged out */
		if (chr_pluggedin || dir_update) {
			chr_cmd[2] = 7;
			ret = gtp_i2c_write(i2c_client_point, chr_cmd, 3);
			if (ret > 0) {
				GTP_INFO("Update status for Charger Plugout");
				if (gtp_send_cfg(i2c_client_point) < 0)
					GTP_ERROR("Send normal config failed.");
				else
					GTP_DEBUG("Send normal config.");

			}
			chr_pluggedin = 0;
		}
	}
}
#endif

/*static void gtp_charger_check_func(struct work_struct *work)
{
#ifdef CONFIG_GTP_CHARGER_DETECT
	gtp_charger_switch(0);
#endif	
}*/

static int touch_event_handler(void *unused)
{
		struct sched_param param = { .sched_priority = 4 };
		u8	end_cmd[3] = {GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF, 0};
		u8	point_data[2 + 1 + 8 * GTP_MAX_TOUCH + 1] = {
			GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF};
		u8	touch_num = 0, finger = 0, key_value = 0, *coor_data = NULL;
		static u8 pre_touch, pre_key;
#ifdef CONFIG_GTP_WITH_PEN
		static u8 pre_pen;
#endif
		s32 input_x = 0, input_y = 0, input_w = 0;
		s32 id = 0, i = 0, ret = -1;

#ifdef CONFIG_HOTKNOT_BLOCK_RW
		u8 hn_state_buf[10] = {(u8)(GTP_REG_HN_STATE >> 8),
				(u8)(GTP_REG_HN_STATE & 0xFF), 0};
		u8 hn_pxy_state = 0, hn_pxy_state_bak = 0;
		u8 hn_paired_cnt = 0;
#endif

	sched_setscheduler(current, SCHED_RR, &param);
	do {
		set_current_state(TASK_INTERRUPTIBLE);
		if (tpd_eint_mode) {
			wait_event_interruptible(waiter, tpd_flag != 0);
			tpd_flag = 0;
		} else {
			msleep(tpd_polling_time);
		}

		set_current_state(TASK_RUNNING);
		mutex_lock(&i2c_access);
		

#ifdef CONFIG_GTP_GESTURE_WAKEUP
		if (gesture_data.enabled) {
			ret = gesture_event_handler(tpd->dev);
			GTP_DEBUG("Interrupt gesture event handled, ret = %d",ret);
			if (ret >0) { /* event handled */
				gtp_irq_enable();
				mutex_unlock(&i2c_access);
				continue;
			}
		}
#endif

		if (tpd_halt || gtp_resetting || gtp_loading_fw) {
			GTP_DEBUG("Interrupt exit,halt:%d,reset:%d,ld_fw:%d",
				tpd_halt, gtp_resetting, gtp_loading_fw);
			goto exit_unlock;
		}

		ret = gtp_i2c_read(i2c_client_point, point_data, 12);
		if (ret < 0) {
				GTP_ERROR("I2C transfer error. errno:%d\n ", ret);
				goto exit_unlock;
		}
		finger = point_data[GTP_ADDR_LENGTH];

#ifdef CONFIG_GTP_COMPATIBLE_MODE
		if ((finger == 0x00) && (CHIP_TYPE_GT9F == gtp_chip_type)) {
			u8 rqst_data[3] = {(u8)(GTP_REG_RQST >> 8),
							(u8)(GTP_REG_RQST & 0xFF), 0};

			ret = gtp_i2c_read(i2c_client_point, rqst_data, 3);
			if (ret < 0) {
				GTP_ERROR("I2C transfer error. errno:%d\n ", ret);
				goto exit_unlock;
			}

			switch (rqst_data[2]&0x0F) {
			case GTP_RQST_BAK_REF:
				GTP_INFO("Request Ref.");
				ret = gtp_bak_ref_proc(i2c_client_point, GTP_BAK_REF_SEND);
				if (SUCCESS == ret) {
					GTP_INFO("Send ref success.");
					rqst_data[2] = GTP_RQST_RESPONDED;
					gtp_i2c_write(i2c_client_point, rqst_data, 3);
				}
				goto exit_work_func;
			case GTP_RQST_CONFIG:
				GTP_INFO("Request Config.");
				ret = gtp_send_cfg(i2c_client_point);
				if (ret < 0) {
					GTP_ERROR("Send config error.");
				} else {
					GTP_INFO("Send config success.");
					rqst_data[2] = GTP_RQST_RESPONDED;
					gtp_i2c_write(i2c_client_point, rqst_data, 3);
				}
				goto exit_work_func;
			case GTP_RQST_MAIN_CLOCK:
				GTP_INFO("Request main clock.");
				rqst_processing = 1;
				ret = gtp_main_clk_proc(i2c_client_point);
				if (SUCCESS == ret) {
					GTP_INFO("Send main clk success.");
					rqst_data[2] = GTP_RQST_RESPONDED;
					gtp_i2c_write(i2c_client_point, rqst_data, 3);
					rqst_processing = 0;
				}
				goto exit_work_func;
			case GTP_RQST_RESET:
				GTP_INFO("Request Reset.");
				mutex_unlock(&i2c_access);
				gtp_recovery_reset(i2c_client_point);
				goto exit_work_func;
			default:
				break;
			}
		}
#endif

#ifdef CONFIG_GTP_HOTKNOT
		if (finger == 0x00 && gtp_hotknot_enabled) {
			u8 rqst_data[3] = {(u8)(GTP_REG_RQST >> 8),
								(u8)(GTP_REG_RQST & 0xFF), 0};

			ret = gtp_i2c_read(i2c_client_point, rqst_data, 3);
			if (ret < 0) {
				GTP_ERROR("I2C transfer error. errno:%d\n ", ret);
				goto exit_unlock;
			}

			if ((rqst_data[2] & 0x0F) == GTP_RQST_HOTKNOT_CODE) {
				GTP_INFO("Request HotKnot Code.");
				gup_load_hotknot_fw();
				goto exit_unlock;
			}
		}
#endif

		if ((finger & 0x80) == 0) {
#ifdef CONFIG_HOTKNOT_BLOCK_RW
			if (!hotknot_paired_flag) {
#else
			{
#endif
				GTP_ERROR("buffer not ready");
				goto exit_unlock;
			}
		}

#ifdef CONFIG_HOTKNOT_BLOCK_RW
		if (!hotknot_paired_flag && (finger&0x0F)) {
			id = point_data[GTP_ADDR_LENGTH+1];
			hn_pxy_state = point_data[GTP_ADDR_LENGTH+2]&0x80;
			hn_pxy_state_bak = point_data[GTP_ADDR_LENGTH+3]&0x80;
			if ((32 == id) && (0x80 == hn_pxy_state) && (0x80 == hn_pxy_state_bak)) {
#ifdef HN_DBLCFM_PAIRED
				if (hn_paired_cnt++ < 2)
					goto exit_work_func;
#endif
				GTP_DEBUG("HotKnot paired!");
				if (wait_hotknot_state & HN_DEVICE_PAIRED) {
					GTP_DEBUG("INT wakeup HN_DEVICE_PAIRED block polling waiter");
					got_hotknot_state |= HN_DEVICE_PAIRED;
					wake_up_interruptible(&bp_waiter);
				}
				hotknot_paired_flag = 1;
				goto exit_work_func;
			} else {
				got_hotknot_state &= (~HN_DEVICE_PAIRED);
				hn_paired_cnt = 0;
			}
		}

		if (hotknot_paired_flag) {
			ret = gtp_i2c_read(i2c_client_point, hn_state_buf, 6);

			if (ret < 0) {
				GTP_ERROR("I2C transfer error. errno:%d\n ", ret);
				goto exit_unlock;
			}

			got_hotknot_state = 0;

			GTP_DEBUG("[0xAB10~0xAB13]=0x%x,0x%x,0x%x,0x%x", hn_state_buf[GTP_ADDR_LENGTH],
				hn_state_buf[GTP_ADDR_LENGTH+1],
				hn_state_buf[GTP_ADDR_LENGTH+2],
				hn_state_buf[GTP_ADDR_LENGTH+3]);

			if (wait_hotknot_state & HN_MASTER_SEND) {
				if ((0x03 == hn_state_buf[GTP_ADDR_LENGTH]) ||
				(0x04 == hn_state_buf[GTP_ADDR_LENGTH]) ||
				(0x07 == hn_state_buf[GTP_ADDR_LENGTH])) {
					GTP_DEBUG("Wakeup HN_MASTER_SEND block polling waiter");
					got_hotknot_state |= HN_MASTER_SEND;
					got_hotknot_extra_state = hn_state_buf[GTP_ADDR_LENGTH];
					wake_up_interruptible(&bp_waiter);
				}
			} else if (wait_hotknot_state & HN_SLAVE_RECEIVED) {
				if ((0x03 == hn_state_buf[GTP_ADDR_LENGTH+1]) ||
				(0x04 == hn_state_buf[GTP_ADDR_LENGTH+1]) ||
				(0x07 == hn_state_buf[GTP_ADDR_LENGTH+1])) {
					GTP_DEBUG("Wakeup HN_SLAVE_RECEIVED block polling waiter:0x%x",
					hn_state_buf[GTP_ADDR_LENGTH+1]);
					got_hotknot_state |= HN_SLAVE_RECEIVED;
					got_hotknot_extra_state = hn_state_buf[GTP_ADDR_LENGTH+1];
					wake_up_interruptible(&bp_waiter);
				}
			} else if (wait_hotknot_state & HN_MASTER_DEPARTED) {
				if (0x07 == hn_state_buf[GTP_ADDR_LENGTH]) {
					GTP_DEBUG("Wakeup HN_MASTER_DEPARTED block polling waiter");
					got_hotknot_state |= HN_MASTER_DEPARTED;
					wake_up_interruptible(&bp_waiter);
				}
			} else if (wait_hotknot_state & HN_SLAVE_DEPARTED) {
				if (0x07 == hn_state_buf[GTP_ADDR_LENGTH+1]) {
					GTP_DEBUG("Wakeup HN_SLAVE_DEPARTED block polling waiter");
					got_hotknot_state |= HN_SLAVE_DEPARTED;
					wake_up_interruptible(&bp_waiter);
				}
			}
		}

#endif

#ifdef CONFIG_GTP_PROXIMITY
		if (tpd_proximity_flag == 1) {
			struct hwm_sensor_data sensor_data;
			u8 proximity_status = point_data[GTP_ADDR_LENGTH];

			GTP_DEBUG("REG INDEX[0x814E]:0x%02X\n", proximity_status);
			/* proximity or large touch detect,enable hwm_sensor. */
			if (proximity_status & 0x60)
				tpd_proximity_detect = 0;
			else
				tpd_proximity_detect = 1;

			/* get raw data */
			GTP_DEBUG("PS change,PROXIMITY STATUS:0x%02X\n", tpd_proximity_detect);
			/* map and store data to hwm_sensor_data */
			sensor_data.values[0] = tpd_get_ps_value();
			sensor_data.value_divide = 1;
			sensor_data.status = SENSOR_STATUS_ACCURACY_MEDIUM;
			/* report to the up-layer */
			ret = hwmsen_get_interrupt_data(ID_PROXIMITY, &sensor_data);

			if (ret)
				GTP_ERROR("Call hwmsen_get_interrupt_data fail = %d\n", ret);
		}

#endif

		touch_num = finger & 0x0f;
		if (touch_num > GTP_MAX_TOUCH) {
				GTP_ERROR("Bad number of fingers!");
				goto exit_work_func;
		}

		if (touch_num > 1) {
			u8 buf[8 * GTP_MAX_TOUCH] = {(GTP_READ_COOR_ADDR + 10) >> 8,
										 (GTP_READ_COOR_ADDR + 10) & 0xff};

			ret = gtp_i2c_read(i2c_client_point, buf, 2 + 8 * (touch_num - 1));
			if (ret < 0)
				goto exit_unlock;
			memcpy(&point_data[12], &buf[2], 8 * (touch_num - 1));
		}

#ifdef CONFIG_GTP_HAVE_TOUCH_KEY
		key_value = point_data[3 + 8 * touch_num];

		if (key_value || pre_key) {
			for (i = 0; i < TPD_KEY_COUNT; i++) {
				input_report_key(tpd->dev, touch_key_array[i], key_value & (0x01 << i));
			}

			if ((pre_key != 0) && (key_value == 0))
				tpd_up(0, 0, 0);

			touch_num = 0;
			pre_touch = 0;
		}

#endif
		pre_key = key_value;
		GTP_DEBUG("pre_touch:%02x, finger:%02x.", pre_touch, finger);
		if (touch_num) {
			for (i = 0; i < touch_num; i++) {
				coor_data = &point_data[i * 8 + 3];
				if (coor_data[0] == 32)
					goto exit_work_func;

				id = coor_data[0] & 0x0F;
				input_x	= coor_data[1] | coor_data[2] << 8;
				input_y	= coor_data[3] | coor_data[4] << 8;
				input_w	= coor_data[5] | coor_data[6] << 8;

				input_x = TPD_WARP_X(abs_x_max, input_x);
				input_y = TPD_WARP_Y(abs_y_max, input_y);
#ifdef CONFIG_GTP_WITH_PEN
				id = coor_data[0];
				if ((id & 0x80))			/* pen/stylus is activated */ {
					GTP_DEBUG("Pen touch DOWN!");
					input_report_key(tpd->dev, BTN_TOOL_PEN, 1);
					pre_pen = 1;
					id = 0;
				}
#endif
				GTP_DEBUG(" %d)(%d, %d)[%d]", id, input_x, input_y, input_w);
				tpd_down(input_x, input_y, input_w, id);
			}
		} else if (pre_touch) {
#ifdef CONFIG_GTP_WITH_PEN
			if (pre_pen) {
				GTP_DEBUG("Pen touch UP!");
				input_report_key(tpd->dev, BTN_TOOL_PEN, 0);
				pre_pen = 0;
			}
#endif
			GTP_DEBUG("Touch Release!");
			tpd_up(0, 0, 0);
		} else {
			GTP_DEBUG("Additional Eint!");
		}
		pre_touch = touch_num;

		if (tpd != NULL && tpd->dev != NULL)
			input_sync(tpd->dev);

exit_work_func:
		if (!gtp_rawdiff_mode) {
			ret = gtp_i2c_write(i2c_client_point, end_cmd, 3);
			if (ret < 0)
				GTP_INFO("I2C write end_cmd	error!");

		}

exit_unlock:
		gtp_irq_enable();
		mutex_unlock(&i2c_access);

	} while (!kthread_should_stop());

	return 0;
}

static int tpd_local_init(void)
{
#if 0	
	int retval;
#endif	
	
#ifdef CONFIG_GTP_ESD_PROTECT
	gtp_workqueue = create_workqueue("gtp-workqueue");
	clk_tick_cnt = 2 * HZ;	 /* HZ: clock ticks in 1 second generated by system */
	GTP_DEBUG("Clock ticks for an esd cycle: %d", clk_tick_cnt);
	INIT_DELAYED_WORK(&gtp_esd_check_work, gtp_esd_check_func);
	spin_lock_init(&esd_lock);	/* 2.6.39 & later */
#endif

#ifdef CONFIG_GTP_CHARGER_DETECT
	INIT_DELAYED_WORK(&gtp_charger_check_work, gtp_charger_check_func);
#endif

#ifdef CONFIG_GTP_SUPPORT_I2C_DMA
	gpDMABuf_va = (u8 *)dma_alloc_coherent(NULL, GTP_DMA_MAX_TRANSACTION_LENGTH,
			&gpDMABuf_pa, GFP_KERNEL);
	if (!gpDMABuf_va)
		GTP_INFO("[Error] Allocate DMA I2C Buffer failed!\n");

	memset(gpDMABuf_va, 0, GTP_DMA_MAX_TRANSACTION_LENGTH);
#endif
	printk("[elink] %s:%d\n", __func__, __LINE__);

#if 0	
	tpd->reg = regulator_get(tpd->tpd_dev, "vtouch");
	retval = regulator_set_voltage(tpd->reg, 2800000, 2800000);
	if (retval != 0) {
		TPD_DMESG("Failed to set reg-vgp6 voltage: %d\n", retval);
		return -1;
	}
#else
	init_tp_power(tpd->tpd_dev, TP_POWER);
#endif
	
	if (i2c_add_driver(&tpd_i2c_driver) != 0) {
		GTP_INFO("unable to add i2c driver.");
		return -1;
	}

	/*if (tpd_load_status == 0)	{
		GTP_INFO("add error touch panel driver.");
		i2c_del_driver(&tpd_i2c_driver);
		return -1;
	}*/
	input_set_abs_params(tpd->dev, ABS_MT_TRACKING_ID, 0, (GTP_MAX_TOUCH-1), 0, 0);
	if (tpd_dts_data.use_tpd_button) {
		/*initialize tpd button data*/
		tpd_button_setting(tpd_dts_data.tpd_key_num, tpd_dts_data.tpd_key_local,
		tpd_dts_data.tpd_key_dim_local);
	}

#if (defined(TPD_WARP_START) && defined(TPD_WARP_END))
	TPD_DO_WARP = 1;
	memcpy(tpd_wb_start, tpd_wb_start_local, TPD_WARP_CNT * 4);
	memcpy(tpd_wb_end, tpd_wb_start_local, TPD_WARP_CNT * 4);
#endif

#if (defined(TPD_HAVE_CALIBRATION) && !defined(TPD_CUSTOM_CALIBRATION))
	memcpy(tpd_calmat, tpd_def_calmat_local, 8 * 4);
	memcpy(tpd_def_calmat, tpd_def_calmat_local, 8 * 4);
#endif

	/* set vendor string */
	tpd->dev->id.vendor = 0x00;
	tpd->dev->id.product = tpd_info.pid;
	tpd->dev->id.version = tpd_info.vid;

	GTP_INFO("end %s, %d", __func__, __LINE__);
	tpd_type_cap = 1;

	return 0;
}

/*******************************************************
Function:
		Eter sleep function.
Input:
		client:i2c_client.
Output:
		Executive outcomes.0--success,non-0--fail.
*******************************************************/
static s8 gt9xx_enter_sleep(struct i2c_client *client)
{
#ifdef CONFIG_GTP_COMPATIBLE_MODE
	if (CHIP_TYPE_GT9F == gtp_chip_type) {
		u8 i2c_status_buf[3] = {0x80, 0x44, 0x00};
		s32 ret = 0;

		ret = gtp_i2c_read(client, i2c_status_buf, 3);
		if (ret <= 0)
			GTP_ERROR("[gtp_enter_sleep]Read ref status reg error.");

		if (i2c_status_buf[2] & 0x80) {
			/* Store bak ref */
			ret = gtp_bak_ref_proc(client, GTP_BAK_REF_STORE);
			if (FAIL == ret)
				GTP_ERROR("[gtp_enter_sleep]Store bak ref failed.");
		}
	}
#endif

#ifdef CONFIG_GTP_POWER_CTRL_SLEEP
	s32 ret_p = 0;
#if 0	
	tpd_gpio_output(GTP_RST_GPIO, 0);
	msleep(20);

	ret_p = regulator_disable(tpd->reg);
	if (ret_p != 0)
		TPD_DMESG("Failed to disable reg-vgp6: %d\n", ret_p);
#else
	tpd_gpio_output(GTP_RST_PORT, 0);
	msleep(20);
	tp_power_on(0);
#endif
	
	GTP_INFO("GTP enter sleep by poweroff!");
	return 0;
#else
	{
		s8 ret = -1;
		s8 retry = 0;
		u8 i2c_control_buf[3] = {(u8)(GTP_REG_SLEEP >> 8), (u8)GTP_REG_SLEEP, 5};

		tpd_gpio_output(GTP_IRQ_GPIO, 0);
		msleep(20);

		while (retry++ < 5) {
			ret = gtp_i2c_write(client, i2c_control_buf, 3);

			if (ret > 0) {
				GTP_INFO("GTP enter sleep!");
				return ret;
			}
			msleep(20);
		}

		GTP_ERROR("GTP send sleep cmd failed.");
		return ret;
	}
#endif
}

/*******************************************************
Function:
		Wakeup from sleep mode Function.
Input:
		client:i2c_client.
Output:
		Executive outcomes.0--success,non-0--fail.
*******************************************************/
static s8 gtp_wakeup_sleep(struct i2c_client *client)
{
	u8 retry = 0;
	s8 ret = -1;

	GTP_DEBUG("GTP wakeup begin.");

#ifdef CONFIG_GTP_POWER_CTRL_SLEEP

#ifdef CONFIG_GTP_COMPATIBLE_MODE
	if (CHIP_TYPE_GT9F == gtp_chip_type) {
		force_reset_guitar();
		GTP_INFO("Esd recovery wakeup.");
		return 0;
	}
#endif

	while (retry++ < 5) {
		ret = tpd_power_on(client);
		if (ret < 0) {
			GTP_ERROR("I2C Power on ERROR!");
			continue;
		}
		GTP_INFO("Ic wakeup by poweron");
		return 0;
	}
#else

#ifdef CONFIG_GTP_COMPATIBLE_MODE
	if (CHIP_TYPE_GT9F == gtp_chip_type) {
		u8 opr_buf[2] = {0};

		while (retry++ < 10) {
		#if 0	
			tpd_gpio_output(GTP_IRQ_GPIO, 1);
			msleep(20);
		#else
			tpd_gpio_output(GTP_INT_PORT, 1);	
		#endif
			ret = gtp_i2c_test(client);
			if (ret >= 0) {
				/* Hold ss51 & dsp */
				opr_buf[0] = 0x0C;
				ret = i2c_write_bytes(client, 0x4180, opr_buf, 1);
				if (ret < 0) {
					GTP_DEBUG("Hold ss51 & dsp I2C error,retry:%d", retry);
					continue;
				}

				/* Confirm hold */
				opr_buf[0] = 0x00;
				ret = i2c_read_bytes(client, 0x4180, opr_buf, 1);
				if (ret < 0) {
					GTP_DEBUG("confirm ss51 & dsp hold, I2C error,retry:%d", retry);
					continue;
				}
				if (0x0C != opr_buf[0]) {
					GTP_DEBUG("ss51 & dsp not hold, val: %d, retry: %d", opr_buf[0], retry);
					continue;
				}
				GTP_DEBUG("ss51 & dsp has been hold");

				ret = gtp_fw_startup(client);
				if (FAIL == ret) {
					GTP_ERROR("[gtp_wakeup_sleep]Startup fw failed.");
					continue;
				}
				GTP_INFO("flashless wakeup sleep success");
				return ret;
			}
			force_reset_guitar();
			retry = 0;
			break;
		}
		if (retry >= 10) {
			GTP_ERROR("wakeup retry timeout, process esd reset");
			force_reset_guitar();
		}
		GTP_ERROR("GTP wakeup sleep failed.");
		return ret;
	}
#endif
	while (retry++ < 10) {
#ifdef CONFIG_GTP_GESTURE_WAKEUP
		if (gesture_data.enabled) {		
			if (DOZE_WAKEUP != gesture_data.doze_status)
				GTP_INFO("Powerkey wakeup.");
			else
				GTP_INFO("Gesture wakeup.");

			gesture_data.doze_status = DOZE_DISABLED;

			gtp_irq_disable();
			gtp_reset_guitar(client, 20);
			gtp_irq_enable();
		} else {
#else
		{
#endif
		#if 0	
			tpd_gpio_output(GTP_IRQ_GPIO, 1);
			msleep(20);
		#else
			tpd_gpio_output(GTP_INT_PORT, 1);	
		#endif
		}
		
		ret = gtp_i2c_test(client);
		if (ret >= 0) {
			GTP_INFO("GTP wakeup sleep.");
#ifndef CONFIG_GTP_GESTURE_WAKEUP
			gtp_int_sync(25);
#ifdef CONFIG_GTP_ESD_PROTECT
			gtp_init_ext_watchdog(client);
#endif
#endif
			return ret;
		}
		gtp_reset_guitar(client, 20);
	}
#endif
	GTP_ERROR("GTP wakeup sleep failed.");
	return ret;
}

/* Function to manage low power suspend */
static void tpd_suspend(struct device *h)
{
	s32 ret = -1;

	GTP_INFO("System suspend.");
#ifdef CONFIG_GTP_PROXIMITY
	if (tpd_proximity_flag == 1)
		return;
#endif

#ifdef CONFIG_GTP_HOTKNOT
	if (gtp_hotknot_enabled) {
		u8 buf[3] = {0x81, 0xaa, 0};
#ifdef CONFIG_HOTKNOT_BLOCK_RW
		if (hotknot_paired_flag)
			return;
#endif
		/* check hotknot pair state */
		gtp_i2c_read(i2c_client_point, buf, sizeof(buf));
		if (buf[2] == 0x55) {
			GTP_INFO("GTP early suspend	pair success");
			return;
		}
	}
#endif

	tpd_halt = 1;
	mutex_lock(&i2c_access);
	gtp_irq_disable();

#ifdef CONFIG_GTP_ESD_PROTECT
	gtp_esd_switch(i2c_client_point, SWITCH_OFF);
#endif

#ifdef CONFIG_GTP_GESTURE_WAKEUP
	if (gesture_data.enabled) {
		ret = gtp_enter_doze();
		gtp_irq_enable();
		enable_irq_wake(touch_irq);
	} else {
#else
	{
#endif
		ret = gtp_enter_sleep(i2c_client_point);
		if (ret < 0)
			GTP_ERROR("GTP early suspend failed.");
	}

	mutex_unlock(&i2c_access);
	msleep(58);
}

/* Function to manage power-on resume */
static void tpd_resume(struct device *h)
{
	s32 ret = -1;

	GTP_INFO("System resume.");

	if (gtp_loading_fw) {
		GTP_INFO("Loading fw, abort resume");
		return;
	}

	mutex_lock(&i2c_access);

	ret = gtp_wakeup_sleep(i2c_client_point);
	if (ret < 0)
		GTP_ERROR("GTP later resume failed.");



#ifndef CONFIG_GTP_GESTURE_WAKEUP
	gtp_irq_enable();
#endif

#ifdef CONFIG_GTP_ESD_PROTECT
	gtp_esd_switch(i2c_client_point, SWITCH_ON);
#endif

	mutex_unlock(&i2c_access);
	tpd_halt = 0;
}

static struct tpd_driver_t tpd_device_driver = {
		.tpd_device_name = "gt9xx",
		.tpd_local_init = tpd_local_init,
		.suspend = tpd_suspend,
		.resume = tpd_resume,
};

static void tpd_off(void)
{

	int ret;

	ret = regulator_disable(tpd->reg);
	if (ret != 0)
		TPD_DMESG("Failed to disable reg-vgp6: %d\n", ret);

	GTP_INFO("GTP enter sleep!");

	tpd_halt = 1;
	gtp_irq_disable();
}

static void tpd_on(void)
{
	s32 ret = -1, retry = 0;

	while (retry++ < 5) {
		ret = tpd_power_on(i2c_client_point);
		if (ret < 0)
			GTP_ERROR("I2C Power on ERROR!");

			ret = gtp_send_cfg(i2c_client_point);
			if (ret > 0)
				GTP_DEBUG("Wakeup sleep send config success.");
	}
	if (ret < 0)
		GTP_ERROR("GTP later resume failed.");

	gtp_irq_enable();
	tpd_halt = 0;
}

/* called when loaded into kernel */
static int __init tpd_driver_init(void)
{
	GTP_INFO("GT9 series touch panel driver init");
	tpd_get_dts_info();
	if (tpd_driver_add(&tpd_device_driver) < 0)
		GTP_INFO("add generic driver failed");

		return 0;
}

/* should never be called */
static void __exit tpd_driver_exit(void)
{
	GTP_INFO("GT9 series touch panel driver exit");
	tpd_driver_remove(&tpd_device_driver);
}

module_init(tpd_driver_init);
module_exit(tpd_driver_exit);
MODULE_LICENSE(GTP v2);
MODULE_DESCRIPTION("GT9 Series Touch Panel Driver");
