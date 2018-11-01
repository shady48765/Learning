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




struct 
{
	
};