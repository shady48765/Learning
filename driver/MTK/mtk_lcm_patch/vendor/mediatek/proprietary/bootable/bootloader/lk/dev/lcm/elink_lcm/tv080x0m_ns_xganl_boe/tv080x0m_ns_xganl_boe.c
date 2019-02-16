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
#include <platform/mt_gpio.h>
#include <platform/mt_pmic.h>
#include <platform/upmu_common.h>
#include <platform/mt_i2c.h>

#include "lcm_drv.h"


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

static LCM_UTIL_FUNCS lcm_util = {
    .set_gpio_out = NULL,
};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))



#define GPIO_LCD_VD     ((unsigned int)123)

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define dsi_set_cmdq_V3(ppara, size, force_update)	        lcm_util.dsi_set_cmdq_V3(ppara, size, force_update)

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg											lcm_util.dsi_read_reg()
#define read_reg_v2(cmd, buffer, buffer_size)   			lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)


static struct LCM_setting_table {
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


    mt_set_gpio_mode(GPIO_LCD_VD, GPIO_MODE_00);
    mt_set_gpio_dir(GPIO_LCD_VD, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_LCD_VD, GPIO_OUT_ONE);


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
#ifdef BUILD_LK
	printf("[Elink_LCM_Drv  ] %s, LK \n", __func__);
#else
	printk("[Elink_LCM_Drv ] %s, kernel", __func__);
#endif

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

#if 0
static struct LCM_setting_table lcm_set_window[] = {
	{0x2A,	4,	{0x00, 0x00, (FRAME_WIDTH>>8), (FRAME_WIDTH&0xFF)}},
	{0x2B,	4,	{0x00, 0x00, (FRAME_HEIGHT>>8), (FRAME_HEIGHT&0xFF)}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_out_setting[] = {
    // Sleep Out
	{0x11, 0, {0x00}},
    {REGFLAG_DELAY, 120, {}},

    // Display ON
	{0x29, 0, {0x00}},
	{REGFLAG_DELAY, 10, {}},

	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_sleep_in_setting[] = {
	// Display off sequence
	{0x28, 0, {0x00}},

    // Sleep Mode On
	{0x10, 0, {0x00}},

	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static unsigned int lcm_compare_id(void)
{
	unsigned int id = 0;
	unsigned char buffer[3];

	unsigned int data_array[16];

	/* NOTE:should reset LCM firstly */
	elink_lcm_reset(1);
	MDELAY(30);
	elink_lcm_reset(0);
	MDELAY(30);
	elink_lcm_reset(1);

	MDELAY(30);


	data_array[0] = 0x00033700;
	dsi_set_cmdq(data_array, 1, 1);

	read_reg_v2(0x04, buffer, 3);
	id = (buffer[0] << 8) | buffer[1]; //we only need ID

#ifdef BUILD_LK
	printf("Read JD9366 ID, buf:0x%02x ,0x%02x,0x%02x, ID=0x%X\n", buffer[0], buffer[1], buffer[2],id);
#else
	printk("Read JD9366 ID, buf:0x%02x ,0x%02x,0x%02x, ID=0x%X\n", buffer[0], buffer[1], buffer[2],id);
#endif

	return (LCM_ID_JD9366 == id) ? 1 : 0;
}

#endif

#if 0
	{0xFF,4,{0xAA,0x55,0xA5,0x80}},//========== Internal setting ==========
	{0x6F,2,{0x11,0x00}},
	{0xF7,2,{0x20,0x00}},
	{0x6F,1,{0x06}},
	{0xF7,1,{0xA0}},
	{0x6F,1,{0x19}},
	{0xF7,1,{0x12}},
	{0xF4,1,{0x03}},
	// new Vcom floating
	{0x6F,1,{0x08}},
	{0xFA,1,{0x40}},
	{0x6F,1,{0x11}},
	{0xF3,1,{0x01}},
	// for1, abnormal power off
	{0x6F,1,{0x06}},
	{0xFC,1,{0x03}},
	{0xF0,5,{0x55,0xAA,0x52,0x08,0x00}},//========== page0 relative ==========
	{0xB1,2,{0x68,0x01}},
	{0xB6,1,{0x08}},
	{0x6F,1,{0x02}},
	{0xB8,1,{0x08}},
	{0xBB,2,{0x54,0x44}},
	{0xBC,2,{0x05,0x05}},
	{0xC7,1,{0x01}},
	{0xBD,5,{0x02,0xB0,0x1E,0x1E,0x00}},
	{0xC5,2,{0x01,0x07}},
	{0xC8,1,{0x80}},
	{0xF0,5,{0x55,0xAA,0x52,0x08,0x01}},//========== page1 relative ==========
	{0xB0,2,{0x05,0x05}},
	{0xB1,2,{0x05,0x05}},
	//new}1,
	{0xB2,2,{0x00,0x00}},
	//{0xB8,2,{0x03,0x03}},
	{0xBC,2,{0x90,0x01}},
	{0xBD,2,{0x90,0x01}},
	{0xCA,1,{0x00}},
	{0xC0,1,{0x04}},
	{0xBE,1,{0x29}},
	{0xB3,2,{0x28,0x28}},
	{0xB4,2,{0x12,0x12}},
	{0xB9,2,{0x35,0x35}},
	{0xBA,2,{0x25,0x25}},
	{0xF0,5,{0x55,0xAA,0x52,0x08,0x02}},//========== page2 relative ==========
	{0xEE,1,{0x01}},
	{0xEF,4,{0x09,0x06,0x15,0x18}},
	{0xB0,6,{0x00,0x00,0x00,0x24,0x00,0x55}},
	{0x6F,1,{0x06}},
	{0xB0,6,{0x00,0x77,0x00,0x94,0x00,0xC0}},
	{0x6F,1,{0x0C}},
	{0xB0,4,{0x00,0xE3,0x01,0x1A}},
	{0xB1,6,{0x01,0x46,0x01,0x88,0x01,0xBC}},
	{0x6F,1,{0x06}},
	{0xB1,6,{0x02,0x0B,0x02,0x4B,0x02,0x4D}},
	{0x6F,1,{0x0C}},
	{0xB1,4,{0x02,0x88,0x02,0xC9}},
	{0xB2,6,{0x02,0xF3,0x03,0x29,0x03,0x4E}},
	{0x6F,1,{0x06}},
	{0xB2,6,{0x03,0x7D,0x03,0x9B,0x03,0xBE}},
	{0x6F,1,{0x0C}},
	{0xB2,4,{0x03,0xD3,0x03,0xE9}},
	{0xB3,4,{0x03,0xFB,0x03,0xFF}},
	{0xF0,5,{0x55,0xAA,0x52,0x08,0x06}},//========== GOA relative ==========
	{0xB0,2,{0x0B,0x2E}},
	{0xB1,2,{0x2E,0x2E}},
	{0xB2,2,{0x2E,0x09}},
	{0xB3,2,{0x2A,0x29}},
	{0xB4,2,{0x1B,0x19}},
	{0xB5,2,{0x17,0x15}},
	{0xB6,2,{0x13,0x11}},
	{0xB7,2,{0x01,0x2E}},
	{0xB8,2,{0x2E,0x2E}},
	{0xB9,2,{0x2E,0x2E}},
	{0xBA,2,{0x2E,0x2E}},
	{0xBB,2,{0x2E,0x2E}},
	{0xBC,2,{0x2E,0x00}},
	{0xBD,2,{0x10,0x12}},
	{0xBE,2,{0x14,0x16}},
	{0xBF,2,{0x18,0x1A}},
	{0xC0,2,{0x29,0x2A}},
	{0xC1,2,{0x08,0x2E}},
	{0xC2,2,{0x2E,0x2E}},
	{0xC3,2,{0x2E,0x0A}},
	{0xE5,2,{0x2E,0x2E}},
	{0xC4,2,{0x0A,0x2E}},
	{0xC5,2,{0x2E,0x2E}},
	{0xC6,2,{0x2E,0x00}},
	{0xC7,2,{0x2A,0x29}},
	{0xC8,2,{0x10,0x12}},
	{0xC9,2,{0x14,0x16}},
	{0xCA,2,{0x18,0x1A}},
	{0xCB,2,{0x08,0x2E}},
	{0xCC,2,{0x2E,0x2E}},
	{0xCD,2,{0x2E,0x2E}},
	{0xCE,2,{0x2E,0x2E}},
	{0xCF,2,{0x2E,0x2E}},
	{0xD0,2,{0x2E,0x09}},
	{0xD1,2,{0x1B,0x19}},
	{0xD2,2,{0x17,0x15}},
	{0xD3,2,{0x13,0x11}},
	{0xD4,2,{0x29,0x2A}},
	{0xD5,2,{0x01,0x2E}},
	{0xD6,2,{0x2E,0x2E}},
	{0xD7,2,{0x2E,0x0B}},
	{0xE6,2,{0x2E,0x2E}},
	{0xD8,5,{0x00,0x00,0x00,0x00,0x00}},
	{0xD9,5,{0x00,0x00,0x00,0x00,0x00}},
	{0xE7,1,{0x00}},
	{0xF0,5,{0x55,0xAA,0x52,0x08,0x03}},// PAGE3 :
	{0xB0,2,{0x20,0x00}},
	{0xB1,2,{0x20,0x00}},
	{0xB2,5,{0x05,0x00,0x00,0x00,0x00}},
	{0xB6,5,{0x05,0x00,0x00,0x00,0x00}},
	{0xB7,5,{0x05,0x00,0x00,0x00,0x00}},
	{0xBA,5,{0x57,0x00,0x00,0x00,0x00}},
	{0xBB,5,{0x57,0x00,0x00,0x00,0x00}},
	{0xC0,4,{0x00,0x00,0x00,0x00}},
	{0xC1,4,{0x00,0x00,0x00,0x00}},
	{0xC4,1,{0x60}},
	{0xC5,1,{0x40}},
	{0xF0,5,{0x55,0xAA,0x52,0x08,0x05}},// PAGE5 :
	{0xBD,5,{0x03,0x01,0x03,0x03,0x03}},
	{0xB0,2,{0x17,0x06}},
	{0xB1,2,{0x17,0x06}},
	{0xB2,2,{0x17,0x06}},
	{0xB3,2,{0x17,0x06}},
	{0xB4,2,{0x17,0x06}},
	{0xB5,2,{0x17,0x06}},
	{0xB8,1,{0x00}},
	{0xB9,1,{0x00}},
	{0xBA,1,{0x00}},
	{0xBB,1,{0x02}},
	{0xBC,1,{0x00}},
	{0xC0,1,{0x07}},
	{0xC4,1,{0x80}},
	{0xC5,1,{0xA4}},
	{0xC8,2,{0x05,0x30}},
	{0xC9,2,{0x01,0x31}},
	{0xCC,3,{0x00,0x00,0x3C}},
	{0xCD,3,{0x00,0x00,0x3C}},
	{0xD1,5,{0x00,0x05,0x09,0x07,0x10}},
	{0xD2,5,{0x00,0x05,0x0E,0x07,0x10}},
	{0xE5,1,{0x06}},
	{0xE6,1,{0x06}},
	{0xE7,1,{0x06}},
	{0xE8,1,{0x06}},
	{0xE9,1,{0x06}},
	{0xEA,1,{0x06}},
	{0xED,1,{0x30}},
	{0x6F,1,{0x11}},
	{0xF3,1,{0x01}},//reload setting
	{0x35,1,{0x00}},

	{0x11, 0, {0x00}},// Normal Display
	{REGFLAG_DELAY, 120, {}},

	{0x29, 0, {0x00}},// Display ON
	{REGFLAG_DELAY, 100, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
#endif
