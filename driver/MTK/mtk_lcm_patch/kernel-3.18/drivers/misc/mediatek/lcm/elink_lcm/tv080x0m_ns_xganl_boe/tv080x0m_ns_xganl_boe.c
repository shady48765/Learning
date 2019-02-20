/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
#include <linux/string.h>
#include <linux/wait.h>
#include <linux/platform_device.h>
#include <linux/pinctrl/consumer.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <asm-generic/gpio.h>


//#include <mach/mt_pm_ldo.h>
//#include <mach/upmu_common.h>
	
#include "lcm_drv.h"
#include "ddp_irq.h"

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/wakelock.h>
#include <mach/gpio_const.h>

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (768)
#define FRAME_HEIGHT (1024)

#define REGFLAG_DELAY             							0xFE
#define REGFLAG_END_OF_TABLE      							0xFF   // END OF REGISTERS MARKER

#define LCM_DSI_CMD_MODE									0


// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))



// #define LCM_OFFSET_POWER     (GPIO123 | 0x80000000)
#define LCM_OFFSET_POWER     GPIO123
// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define dsi_set_cmdq_V3(ppara, size, force_update)	        lcm_util.dsi_set_cmdq_V3(ppara, size, force_update)

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)										lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   			lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)    
#ifndef ASSERT
#define ASSERT(expr)					\
	do {						\
		if (expr)				\
			break;				\
		pr_debug("DDP ASSERT FAILED %s, %d\n",	\
		       __FILE__, __LINE__);		\
		BUG();					\
	} while (0)
#endif


struct LCM_setting_table {
    unsigned char cmd;
    unsigned char count;
    unsigned char para_list[64];
};

static struct LCM_setting_table lcm_initialization_setting[] = {

	//{0x01, 0,  {0x00}},
	{0xf0, 2,  {0x5a, 0x5a}},
	{0xf1, 2,  {0x5a, 0x5a}},
	{0xfc, 2,  {0xa5, 0xa5}},
	{0xd0, 2,  {0x00, 0x10}},
	{0xee, 8,  {0x0a, 0x00, 0x28, 0x00, 0x0a, 0x00, 0x28, 0x00}},
	{0x11, 0,  {0x00}},
	{REGFLAG_DELAY, 130, {}},
	{0x29, 0, {0x00}},
	{REGFLAG_DELAY, 20, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}},

};



static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
	unsigned int i;

    for(i = 0; i < count; i++) {

        unsigned cmd;
        cmd = table[i].cmd;

        switch (cmd) {
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
            case REGFLAG_END_OF_TABLE :
                break;
            default:
				// dsi_set_cmdq_V3(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(lcm_initialization_setting[0]), 1);
				dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
       	}
    }

}

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}

extern void DSI_clk_HS_mode(unsigned char enter);

static void lcm_get_params(LCM_PARAMS *params)
{
	memset(params, 0, sizeof(LCM_PARAMS));

	params->type            = LCM_TYPE_DSI;

	params->width           = FRAME_WIDTH;
	params->height          = FRAME_HEIGHT;

	params->physical_height = 163;
	params->physical_width  = 122;

	// enable tearing-free ****
	params->dbi.te_mode 				= LCM_DBI_TE_MODE_DISABLED; //LCM_DBI_TE_MODE_VSYNC_OR_HSYNC;//LCM_DBI_TE_MODE_DISABLED;//LCM_DBI_TE_MODE_VSYNC_ONLY;
	params->dbi.te_edge_polarity		= LCM_POLARITY_FALLING;

#if (LCM_DSI_CMD_MODE)
	params->dsi.mode   = CMD_MODE;
#else
	params->dsi.mode   = BURST_VDO_MODE; //SYNC_PULSE_VDO_MODE;//BURST_VDO_MODE;
#endif

	// DSI
	/* Command mode setting */
	params->dsi.LANE_NUM				= LCM_FOUR_LANE;
	//The following defined the fomat for data coming from LCD engine.
	params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
	params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

	// Highly depends on LCD driver capability.
	// Not support in MT6573
	params->dsi.packet_size             = 256;

	// Video mode setting
	params->dsi.intermediat_buffer_num  = 2; //because DSI/DPI HW design change, this parameters should be 0 when video mode in MT658X; or memory leakage
	params->dsi.PS                      = LCM_PACKED_PS_24BIT_RGB888;
	params->dsi.word_count              = FRAME_WIDTH*3;
	params->dsi.vertical_sync_active    = 4; //4;
	params->dsi.vertical_backporch      = 12; //4;
	params->dsi.vertical_frontporch     = 16;  //8;
	params->dsi.vertical_active_line    = FRAME_HEIGHT;//1322

	params->dsi.horizontal_sync_active  = 16; //20;
	params->dsi.horizontal_backporch    = 48; //20;
	params->dsi.horizontal_frontporch   = 16; //32;
	params->dsi.horizontal_active_pixel = FRAME_WIDTH;//854

	params->dsi.PLL_CLOCK               = 180; // MIPI CLK
	// params->dsi.cont_clock           = 1;
	// params->dsi.ssc_disable          = 1;
}


static void lcm_power_init(void)
{
    

#ifdef BUILD_LK
	printf("[Elink_LCM_Drv  ] %s, LK \n", __func__);
#else
	printk("[Elink_LCM_Drv  ] %s, kernel", __func__);
#endif


	elink_lcm_dvdd(1);//IOVCC 1.8V
	MDELAY(10);
	elink_lcm_avdd(1);//VCC 3.3V
	MDELAY(30);

	elink_lcm_reset(1);
	MDELAY(30);
	elink_lcm_reset(0);
	MDELAY(30);
	elink_lcm_reset(1);

	MDELAY(200);

#ifdef BUILD_LK
	printf("[elink][LK/LCM] %s() end\n", __func__);
#else
	printk("[elink][Kernel/LCM] %s() end\n", __func__);
#endif
}

static void lcm_init(void)
{
    
    int ret = 0;
    
#ifdef BUILD_LK
    printf("[Elink_LCM_Drv  ] %s, LK \n", __func__);
#else
    printk("[Elink_LCM_Drv ] %s, kernel", __func__);
#endif

	elink_lcm_reset(0);
	elink_lcm_standby(0);
	elink_lcm_avdd(0);
	elink_lcm_dvdd(0);
    MDELAY(30);

	elink_lcm_dvdd(1);//IOVCC 1.8V
	MDELAY(10);
	elink_lcm_avdd(1);//VCC 3.3V
	MDELAY(30);
	elink_lcm_standby(1);
	MDELAY(30);
    
	/* lcm baise power on start --------------------------*/

	ret = gpio_request(LCM_OFFSET_POWER, "lcm_offset_power");
	if (ret)
		pr_err("[KERNEL][LCM] lcm_offset_power gpio_request (%d)fail\n", LCM_OFFSET_POWER);

	ret = gpio_direction_output(LCM_OFFSET_POWER, 0);
	if (ret)
		pr_err("[KERNEL][LCM] lcm_offset_powergpio_direction_output (%d)fail\n", LCM_OFFSET_POWER);

	gpio_set_value(LCM_OFFSET_POWER, 1);
    /* lcm baise power on end --------------------------*/
    
	elink_lcm_reset(1);
	MDELAY(30);
	elink_lcm_reset(0);
	MDELAY(30);
	elink_lcm_reset(1);
	MDELAY(120);



    //init_lcm_registers();

	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);

	// dsi_set_cmdq_V3(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(LCM_setting_table_V3), 1);
}



static void lcm_suspend(void)
{
	unsigned int data_array[16];
	int ret = 0;

#ifdef BUILD_LK
	printf("[elink][LK/LCM] %s() enter\n", __func__);
#else
	printk("[elink][Kernel/LCM] %s() enter\n", __func__);
#endif

	// data_array[0] = 0x00280500;  //display off						  
	// dsi_set_cmdq(data_array, 1, 1);
	// MDELAY(80); 

	data_array[0] = 0x00100500; // Sleep In
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(120); 
	
	elink_lcm_reset(0);
	MDELAY(10);
	
	elink_lcm_standby(0);
	MDELAY(10);
	
	elink_lcm_avdd(0);
	MDELAY(10);
	elink_lcm_dvdd(0);
	MDELAY(10);

	/* lcm baise power on start --------------------------*/

	ret = gpio_request(LCM_OFFSET_POWER, "lcm_offset_power");
	if (ret)
		pr_err("[KERNEL][LCM] lcm_offset_power gpio_request (%d)fail\n", LCM_OFFSET_POWER);

	ret = gpio_direction_output(LCM_OFFSET_POWER, 0);
	if (ret)
		pr_err("[KERNEL][LCM] lcm_offset_powergpio_direction_output (%d)fail\n", LCM_OFFSET_POWER);

	gpio_set_value(LCM_OFFSET_POWER, 0);
	/* lcm baise power on end --------------------------*/
}


static void lcm_resume(void)
{
#ifdef BUILD_LK
	printf("[Elink_LCM_Drv  ] %s, LK \n", __func__);
#else
	printk("[Elink_LCM_Drv ] %s, kernel", __func__);
#endif
	lcm_init();
}

LCM_DRIVER tv080x0m_ns_xganl_boe_lcm_drv =
{
	.name           = "tv080x0m_ns_xganl_boe",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	// .compare_id = lcm_compare_id,
	.init_power = lcm_power_init,
};
