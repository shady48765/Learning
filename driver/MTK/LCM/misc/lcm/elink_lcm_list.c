/*
 * Copyright (C) 2015 MediaTek Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

//#include "elink_lcm_list.h"
#include <lcm_drv.h>
#ifdef BUILD_LK
#include <platform/disp_drv_platform.h>
#else
#include <linux/delay.h>
/* #include <mach/mt_gpio.h> */
#endif
LCM_DSI_MODE_CON lcm_dsi_mode;

/* used to identify float ID PIN status */
#define LCD_HW_ID_STATUS_LOW      0
#define LCD_HW_ID_STATUS_HIGH     1
#define LCD_HW_ID_STATUS_FLOAT 0x02
#define LCD_HW_ID_STATUS_ERROR  0x03

#ifdef BUILD_LK
#define LCD_DEBUG(fmt)  dprintf(CRITICAL, fmt)
#else
#define LCD_DEBUG(fmt, args...)  pr_debug("[KERNEL/LCM]"fmt, ##args)
#endif

#if defined(MTK_LCM_DEVICE_TREE_SUPPORT)
	extern LCM_DRIVER lcm_common_drv;
#else
	extern LCM_DRIVER tf070mc124_dsi_lcm_drv;
	extern LCM_DRIVER tf070mc124_dsi_none_lcm_drv;
	extern LCM_DRIVER tf070mc124_ips_dsi_lcm_drv;
	extern LCM_DRIVER otm1284_hd720_dsi_vdo_tm_lcm_drv;
	extern LCM_DRIVER otm1287_hd720_dsi_vdo_ivo_lcm_drv;
	extern LCM_DRIVER hx8394d_hd720_dsi_vdo_lcm_drv;
	extern LCM_DRIVER s6d7aa0x01_boe_wxga_dsi_vdo_lcm_drv;
	extern LCM_DRIVER hx8260_boe_wxga_dsi_vdo_lcm_drv;
	extern LCM_DRIVER hx8389c_hsd_qhd_dsi_vdo_lcm_drv;
	extern LCM_DRIVER hx8389c_ivo_qhd_dsi_vdo_lcm_drv;
	extern LCM_DRIVER otm1283_hd720_dsi_vdo_tm_lcm_drv;
	extern LCM_DRIVER hx8394d_hd720_dsi_cmd_lcm_drv;
	extern LCM_DRIVER kd079d1_35na_a1_lcm_drv;
	extern LCM_DRIVER mr798_hx8394c_boe_ips_dis_vdo_lcm_drv;
	extern LCM_DRIVER hx8667g_hd720_dsi_vdo_lcm_drv;
	extern LCM_DRIVER samsung_08s_dsi_vdo_lcm_drv;
	extern LCM_DRIVER jd9366_boe_ips_dsi_lcm_drv;
	extern LCM_DRIVER b116xan03_dsi_vdo_lcm_drv;
	extern LCM_DRIVER otm9605_qhd_dsi_vdo_tm_lcm_drv;
	extern LCM_DRIVER hx8389c_ivo_qhd_dsi_vdo_sl_lcm_drv;
	extern LCM_DRIVER hc101_hd720_dsi_vdo_lcm_drv;
	extern LCM_DRIVER otm1284_hd720_dsi_vdo_tm_lcm_drv;
	extern LCM_DRIVER ili6136s_wxga_boe_dsi_vdo_lcm_drv;
	extern LCM_DRIVER hs070b03a_boe_dsi_lcm_drv;
	extern LCM_DRIVER hx8389b_qhd_dsi_vdo_lcm_drv;
	extern LCM_DRIVER hx8389b_qhd_dsi_vdo_tianma_lcm_drv;
	extern LCM_DRIVER hx8389b_qhd_dsi_vdo_tianma055xdhp_lcm_drv;
	extern LCM_DRIVER hx8389b_qhd_lt_dsi_vdo_lcm_drv;
	extern LCM_DRIVER hx8389b_LTqhd_dsi_vdo_lcm_drv;
	extern LCM_DRIVER hx101_dsi_vdo_lcm_drv;
	extern LCM_DRIVER hx101_wxga_dsi_vdo_lcm_drv;
	extern LCM_DRIVER jd9366_cu800_ips_dsi_lcm_drv;
	extern LCM_DRIVER nt35521_wxga_dsi_vdo_lcm_drv;
	extern LCM_DRIVER hx8394d_boe_ips_dis_vdo_lcm_drv;
	extern LCM_DRIVER jd9367_wxga_ips_dsi_vdo_lcm_drv;
	extern LCM_DRIVER jd9362_wxga_dsi_vdo_lcm_drv;
	extern LCM_DRIVER jd9366_ivo_ips_dsi_lcm_drv;
	extern LCM_DRIVER hx8394a_ips_qc_wxga_dsi_vdo_lcm_drv;
	extern LCM_DRIVER hx8394d_wxga_ips_dsi_vdo_lcm_drv;
	extern LCM_DRIVER samsung_boe_dsi_vdo_lcm_drv;
	extern LCM_DRIVER jd9367_ips_qc_wxga_dsi_vdo_lcm_drv;
	extern LCM_DRIVER nt35521_cpt_wxga_dsi_vdo_lcm_drv;
	extern LCM_DRIVER jd9365_ips_dsi_lcm_drv;
	extern LCM_DRIVER nt35521_wxga_cpt_dsi_vdo_lcm_drv;
	extern LCM_DRIVER jd9366_ivo_ips_dsi_vdo_lcm_drv;
	extern LCM_DRIVER hx8394c_wxga_ivo_vdo_dsi_lcm_drv;
	extern LCM_DRIVER nt51017_wxga_dsi_vdo_lcm_drv;
	extern LCM_DRIVER tv096wxm_ns0_lcm_drv;
	extern LCM_DRIVER jd9364_inx_wxga_hsx101_lcm_drv;	
	extern LCM_DRIVER ek79007_ivo_tn10_lcm_drv;	
	extern LCM_DRIVER EK79029_cpt_wxga_lcm_drv;	
	extern LCM_DRIVER cpt101_ili9881c_dsi_ips_lcm_drv;
	extern LCM_DRIVER jd9366_hx0082102a_ivo_vdo_dsi_lcm_drv;//add by heliang 20160913
	extern LCM_DRIVER jd9364_hsx101n31am27a_dsi_vdo_lcm_drv;//add by heliang 20160918
	extern LCM_DRIVER tf070mc124_boe7_tn_97w_dsi_lcm_drv;//add by heliang 20160918
	extern LCM_DRIVER otm1287a_kd101n55_31na_a3_wxganl_dsi_lcm_drv;//add by heliang 20160919
	extern LCM_DRIVER jd9367_sl007pn18d1244_a00_wxga_dsi_lcm_drv;//add by heliang 20161008
	extern LCM_DRIVER jd9366_h101iv027004_31n_ivo_wxga_dsi_lcm_drv;//add by heliang 20161012
	extern LCM_DRIVER ili9881c_20810700180665_hd720_cpt_lcm_drv;//add by heliang 20161012
	extern LCM_DRIVER hx8394d_hc695_wsvga_boe_dsi_lcm_drv;//add by heliang 20161021
	extern LCM_DRIVER nt35521s_sl101pn27d0993_wxga_inx_lcm_drv;//add by heliang 20161028
	extern LCM_DRIVER samsung_boe_wxga_joyplus_lcm_drv;
	extern LCM_DRIVER ek79029_sl008pc21d1138_a00_wxga_cpt_lcm_drv;//add by heliang 20161012
	extern LCM_DRIVER jd9366_cpt_wjhd069009a_lcm_drv;
	extern LCM_DRIVER nt35521s_n070ice_g02_inx_wxga_vdo_lcm_drv;//add by heliang 20161116
	extern LCM_DRIVER otm1287_sl101pa27d1249_wxga_auo_lcm_drv;//add by heliang 20161122
	extern LCM_DRIVER jd9366_dy698b30imh18_j01_boe_hd720_lcm_drv;  //add by heliang 20161126
	extern LCM_DRIVER jd9366_p101nwwbp_01g_wxga_dsi_lcm_drv;//add by heliang 20161128
	extern LCM_DRIVER nt35523b_wjwx080030a_wxga_boe_lcm_drv;//add by heliang 20161130
	extern LCM_DRIVER jd9366_20810700200051_boe7_wxga_lcm_drv;//add by heliang 20161202
	extern LCM_DRIVER jd9365_k695_bm2b602a_wsvga_boe_lcm_drv;  //add by heliang 20161206
	extern LCM_DRIVER jd9366_auo8_wxga_vdo_lcm_drv;  //add by heliang 20161207
	extern LCM_DRIVER hx8394c_zs080bh3107e3h7ii_boe8_wxga_lcm_drv;//add by heliang 20161212
	extern LCM_DRIVER ek79029_wjwx080016a_wxga_boe8_lcm_drv;//add by heliang 20161219
	extern LCM_DRIVER otm1287a_auo101_wxga_dsi_lcm_drv;//add by heliang 20161222
	extern LCM_DRIVER jd9366_cpt8_wxga_vdo_dsi_lcm_drv;//add by heliang 20161223
	extern LCM_DRIVER jd9367_kd080d34_30ni_wxga_inx8_lcm_drv;//add by heliang 20161227
	extern LCM_DRIVER jd9366_k101_im2aa02a_wxga_auo101_lcm_drv;//add by heliang 20161229
	extern LCM_DRIVER jd9364_k101_im2oa02a_wxga_auo101_lcm_drv;//add by heliang 20170104
	extern LCM_DRIVER yx070lips30_wxga_dsi_vdo_lcm_drv;//add by heliang 20170107	
	extern LCM_DRIVER otm1287a_hsx101u40am27a_wxga_auo101_lcm_drv;//add by heliang 20170109
	extern LCM_DRIVER ek79029_zs080be3005e3h7ii_wxga_boe8_lcm_drv;//add by heliang 20170119
	extern LCM_DRIVER jd9366_20810800210255_wxga_cpt_vdo_lcm_drv;	//add by heliang 20161011
	extern LCM_DRIVER jd9365aa_k101_im2ca02_a_wxga_auo101_lcm_drv;//add by heliang 20170306
	extern LCM_DRIVER jd9365aa_k101_im2ca02_b_wxga_auo101_lcm_drv;//add by heliang 20170306
	extern LCM_DRIVER nt35521s_kd101n66_40ni_b2_wxga_inx101_lcm_drv;//add by heliang 20170308
	extern LCM_DRIVER tf070mc124_ips_cpt7_dsi_lcm_drv;//add by heliang 20170313	
	extern LCM_DRIVER ota7290b_20810700160048_wsvga_auo695_lcm_drv;//add by heliang 20170327
	extern LCM_DRIVER tf070mc124_ips_boe7_dsi_lcm_drv;//add by heliang 20170328
	extern LCM_DRIVER jd9367_wxga_qc_dsi_vdo_lcm_drv;//add by heliang 20170418
	extern LCM_DRIVER jd9367_hnh_inx8_wxga_lcm_drv;//add by heliang 20170418
	extern LCM_DRIVER jd9365_p101nwwbp_01g_r9m_wxga_auo101_lcm_drv;//add by heliang 20170418
	extern LCM_DRIVER rm68200_auo_wxga_cc80xy_lcm_drv;//add by heliang 20170324
	extern LCM_DRIVER jd9366_sm080_142a1_ivo_wxga8_lcm_drv;//add by heliang 20170410
	extern LCM_DRIVER tf070mc124_ips_hsd7_dsi_lcm_drv;//add by heliang 20170411
	extern LCM_DRIVER rm72014_auo8_wxga_dsi_lcm_drv;//add by heliang 20170426
	extern LCM_DRIVER jd9366_ph080nb20a_wxga_cpt8_lcm_drv;//add by heliang 201700508
	extern LCM_DRIVER nt35521_wxga_cpt_dsi_vdo_rst33_lcm_drv;//add by heliang 20170519
	extern LCM_DRIVER jd9365_wjwx101027b_wxga_hsd101_lcm_drv;//add by heliang 20170626
	extern LCM_DRIVER jd9366_hsx101zi_31mi27g_wxga_ivo101_lcm_drv;//add by heliang 20170629
	extern LCM_DRIVER jd9366_k080_im2h801_a_wxga_hsd8_lcm_drv;//add by heliang 20170609
	extern LCM_DRIVER jd9365_wjwx080056a_wxga_hsd8_lcm_drv;//add by heliang 20170630
	extern LCM_DRIVER ek79029_hsx080mli21a_mipi31_wxga_ivo8_lcm_drv;//add by heliang 20170705
	extern LCM_DRIVER samsung_20810700200057_wxga_boe7_lcm_drv;//add by heliang 20170714
	extern LCM_DRIVER jd9366_cpt_wjhd069009a_fp7721_lcm_drv;//add by heliang 20170719
	extern LCM_DRIVER ek79029_xpp080v190_wxga_ivo8_lcm_drv;//add by heliang 20170803
	extern LCM_DRIVER nt35521s_n070icn_pb1_inx_wxga_vdo_lcm_drv;//add by heliang 20170823
	extern LCM_DRIVER hx8394d_wxga_boe101_dsi_lcm_drv;//add by heliang 20170831
	extern LCM_DRIVER jd9367_yf070l01_wxga_inx7_lcm_drv;//add by heliang 20170919
	extern LCM_DRIVER ek79029_ips_cpt_wxga_fy80_lcm_drv;
	extern LCM_DRIVER hx8394d_wxga_dsi_vdo_lcm_drv;
	extern LCM_DRIVER hx8394c_boe_wxga_dsi_vdo_lcm_drv;
	extern LCM_DRIVER hx8394f_hsd_hd720_dsi_vdo_lcm_drv;
	extern LCM_DRIVER jd9364_wjwx101070a_lcm_drv;//add by heliang 20170220
	extern LCM_DRIVER nt51021_wuxga_sl008pn24d1092_e00_lcm_drv;//add by heliang 20170713
	extern LCM_DRIVER nt51021_wuxga_qc_sl_dsi_vdo_lcm_drv;   //add by zhou 20170410
	extern LCM_DRIVER nt51021_sl101pn28d1127_wuxga_dsi_lcm_drv;
	extern LCM_DRIVER hx8394d_pbt025_wsvga_boe_dsi_lcm_drv;//add by heliang 20170927
	extern LCM_DRIVER ili6180_kd070d5040nia20_wsvga_inx695_lcm_drv;//add by heliang 20170927
	extern LCM_DRIVER jd9366_wxga_dsi_cpt8_lcm_drv;//add by heliang 20171009
	extern LCM_DRIVER jd9366_jf70056w20ch1_wxga_boe7_lcm_drv;//add by heliang 20171128
	extern LCM_DRIVER jd9366_sl008pb21d1439_a00_wxga_boe8_lcm_drv;//add by heliang 20171128
	extern LCM_DRIVER jd9366_cpt101_ips_dsi_lcm_drv;//add by heliang 20171129	
	extern LCM_DRIVER jd9365_k101_im2ba02_c_wxga_boe101_lcm_drv;//add by heliang 20171129
	extern LCM_DRIVER nt51021_wsvga_inx695_dsi_lcm_drv;//add by heliang 20171202
	extern LCM_DRIVER jd9366_pbt080wxb30i057_wxga_boe8_lcm_drv;//add by heliang 20171207
	extern LCM_DRIVER jd9365_pbt080wxh30i062_wxga_hsd8_lcm_drv;//add by heliang 20171218
	extern LCM_DRIVER otm1287a_wjwx101092b_wxga_auo101_lcm_drv;//add by heliang 20180122
	extern LCM_DRIVER ili9881c_20811010320060_wxga_cpt_vdo_lcm_drv;	//add by heliang 20180206
	extern LCM_DRIVER jd9366_k080_im2c801_a_wxga_cpt8_lcm_drv;//add by heliang 20180412
	
	extern LCM_DRIVER ota7290b_k101mm2ba02_c_wuxga_boe101_lcm_drv;
	extern LCM_DRIVER jd9366_wjwx070138a_wxga_boe7_lcm_drv;//add by heliang 20180629
	extern LCM_DRIVER ek79029_wjwx080063a_wxga_boe8_lcm_drv;//add by heliang 20180907
	extern LCM_DRIVER hx8394d_hsd8_wxga_dsi_lcm_drv;//add by heliang 20180925
	extern LCM_DRIVER nt51021_wuxga_sl_boe101_lcm_drv; //add by heliang 20180929
	extern LCM_DRIVER it6151_lp079qx1_edp_dsi_video_lcm_drv;

	extern LCM_DRIVER tv080x0m_ns_xganl_boe_lcm_drv;
#endif	







LCM_DRIVER *lcm_driver_list[] = {

#if defined(MTK_LCM_DEVICE_TREE_SUPPORT)
	&lcm_common_drv,
#else// End defined(MTK_LCM_DEVICE_TREE_SUPPORT)


#if defined(TF070MC124_DSI)  //Elink Nicholas Zheng Add 
	&tf070mc124_dsi_lcm_drv,
#endif

#if defined(TF070MC124_DSI_NONE)  //Elink Nicholas Zheng Add 
	&tf070mc124_dsi_none_lcm_drv,
#endif

#if defined(TF070MC124_IPS_DSI)
	&tf070mc124_ips_dsi_lcm_drv,
#endif

#if defined(OTM1284_HD720_DSI_VDO_TM)
	&otm1284_hd720_dsi_vdo_tm_lcm_drv,
#endif
	
#if defined(OTM1287_HD720_DSI_VDO_IVO)
	&otm1287_hd720_dsi_vdo_ivo_lcm_drv, 
#endif

#if defined(HX8394D_HD720_DSI_VDO) 
	&hx8394d_hd720_dsi_vdo_lcm_drv, 
#endif

#if defined(S6D7AA0X01_BOE_WXGA_DSI_VDO)
	&s6d7aa0x01_boe_wxga_dsi_vdo_lcm_drv, 
#endif
	
#if defined(HX8260_BOE_WXGA_DSI_VDO)
	&hx8260_boe_wxga_dsi_vdo_lcm_drv,
#endif
	
#if defined(HX8389C_HSD_QHD_DSI_VDO)
	&hx8389c_hsd_qhd_dsi_vdo_lcm_drv, 
#endif
	
#if defined(HX8389C_IVO_QHD_DSI_VDO)	
	&hx8389c_ivo_qhd_dsi_vdo_lcm_drv,
#endif

#if defined(OTM1283_HD720_DSI_VDO)	
	&otm1283_hd720_dsi_vdo_tm_lcm_drv, 
#endif
	
#if defined(HX8394D_HD720_DSI_CMD)	
	&hx8394d_hd720_dsi_cmd_lcm_drv, 
#endif

#if defined(KD079D1_35NA_A1)	
	&kd079d1_35na_a1_lcm_drv,  
#endif
	
#if defined(MR798_HX8394C_BOE_IPS_DIS)	
	&mr798_hx8394c_boe_ips_dis_vdo_lcm_drv, 
#endif
	
#if defined(HX8667G_HD720_DSI_VDO)	
	&hx8667g_hd720_dsi_vdo_lcm_drv,  
#endif
	
#if defined(SAMSUNG_08S_DSI_VDO)	
	&samsung_08s_dsi_vdo_lcm_drv,  
#endif
	
#if defined(JD9366_BOE_IPS_DSI)	
	&jd9366_boe_ips_dsi_lcm_drv,  
#endif
	
#if defined(B116XAN03_DSI_VDO)	
	&b116xan03_dsi_vdo_lcm_drv,  
#endif
	
#if defined(OTM9605_QHD_DSI_VDO_TM)	
	&otm9605_qhd_dsi_vdo_tm_lcm_drv,  
#endif
	
#if defined(HX8389C_IVO_QHD_DSI_VDO_SL)	
	&hx8389c_ivo_qhd_dsi_vdo_sl_lcm_drv,  
#endif
	
#if defined(HC101_HD720_DSI_VDO)	
	&hc101_hd720_dsi_vdo_lcm_drv,  
#endif
	
#if defined(OTM1284_HD720_DSI_VDO_TM)	
	&otm1284_hd720_dsi_vdo_tm_lcm_drv,  
#endif
	
#if defined(ILI6136S_WXGA_BOE_DSI_VDO)	
	&ili6136s_wxga_boe_dsi_vdo_lcm_drv,  
#endif
	
#if defined(HS070B03A_BOE_DSI)	
	&hs070b03a_boe_dsi_lcm_drv,  
#endif
	
#if defined(HX8389B_QHD_DSI_VDO)	
	&hx8389b_qhd_dsi_vdo_lcm_drv,  
#endif
	
#if defined(HX8389B_QHD_DSI_VDO_TIANMA)	
	&hx8389b_qhd_dsi_vdo_tianma_lcm_drv, 
#endif
	
#if defined(HX8389B_QHD_DSI_VDO_TIANMA055XDHP)	
	&hx8389b_qhd_dsi_vdo_tianma055xdhp_lcm_drv,  
#endif
	
#if defined(HX8389B_QHD_LT_DSI_VDO)	
	&hx8389b_qhd_lt_dsi_vdo_lcm_drv,  
#endif
	
#if defined(HX8389B_LTQHD_DSI_VDO)	
	&hx8389b_LTqhd_dsi_vdo_lcm_drv,  
#endif
	
#if defined(HX101_DSI_VDO)	
	&hx101_dsi_vdo_lcm_drv,  
#endif
	
#if defined(HX101_WXGA_DSI_VDO)	
	&hx101_wxga_dsi_vdo_lcm_drv,  
#endif
	
#if defined(JD9366_CU800_IPS_DSI)	
	&jd9366_cu800_ips_dsi_lcm_drv, 
#endif
	
#if defined(NT35521_WXGA_DSI_VDO)	
	&nt35521_wxga_dsi_vdo_lcm_drv, 
#endif
	
#if defined(HX8394D_BOE_IPS_DIS_VDO)	
	&hx8394d_boe_ips_dis_vdo_lcm_drv, 
#endif
	
#if defined(JD9367_WXGA_IPS_DSI_VDO)	
	&jd9367_wxga_ips_dsi_vdo_lcm_drv,  
#endif
	
#if defined(JD9362_WXGA_DSI_VDO)	
	&jd9362_wxga_dsi_vdo_lcm_drv,  
#endif
	
#if defined(JD9366_IVO_IPS_DSI)	
	&jd9366_ivo_ips_dsi_lcm_drv,  
#endif
	
#if defined(HX8394A_IPS_QC_WXGA_DSI_VDO)	
	&hx8394a_ips_qc_wxga_dsi_vdo_lcm_drv,  
#endif
	
#if defined(HX8394D_WXGA_IPS_DSI_VDO)	
	&hx8394d_wxga_ips_dsi_vdo_lcm_drv, 
#endif	

#if defined(SAMSUNG_BOE_DSI_VDO)	
	&samsung_boe_dsi_vdo_lcm_drv, 
#endif	

#if defined(JD9367_IPS_QC_WXGA_DSI_VDO)	
	&jd9367_ips_qc_wxga_dsi_vdo_lcm_drv, 
#endif

#if defined(NT35521_CPT_WXGA_DSI_VDO)	
	&nt35521_cpt_wxga_dsi_vdo_lcm_drv, 
#endif	

#if defined(JD9365_IPS_DSI)
	&jd9365_ips_dsi_lcm_drv,
#endif		

#if defined(NT35521_WXGA_CPT_DSI_VDO)
	&nt35521_wxga_cpt_dsi_vdo_lcm_drv, 
#endif	

#if defined(JD9366_IVO_IPS_DSI_VDO)
	&jd9366_ivo_ips_dsi_vdo_lcm_drv, 
#endif	

#if defined(HX8394C_WXGA_IVO_VDO_DSI)
	&hx8394c_wxga_ivo_vdo_dsi_lcm_drv, 
#endif	

#if defined(NT51017_WXGA_DSI_VDO)
	&nt51017_wxga_dsi_vdo_lcm_drv, 
#endif	

#if defined(TV096WXM_NS0)
	&tv096wxm_ns0_lcm_drv, 
#endif	

#if defined(JD9364_INX_WXGA_HSX101)
	&jd9364_inx_wxga_hsx101_lcm_drv, 
#endif	

#if defined(EK79007_IVO_TN10)
	&ek79007_ivo_tn10_lcm_drv, 
#endif

#if defined(EK79029_CPT_WXGA_DRV)
	&EK79029_cpt_wxga_lcm_drv, 
#endif

#if defined(CPT101_ILI9881C_DSI_IPS)
	&cpt101_ili9881c_dsi_ips_lcm_drv, 
#endif

#if defined(JD9366_HX0082102A_IVO_VDO_DSI)  
	&jd9366_hx0082102a_ivo_vdo_dsi_lcm_drv,
#endif

#if defined(JD9364_HSX101N31AM27A_DSI_VDO)  
	&jd9364_hsx101n31am27a_dsi_vdo_lcm_drv,
#endif

#if defined(TF070MC124_BOE7_TN_97W_DSI)  
	&tf070mc124_boe7_tn_97w_dsi_lcm_drv,
#endif

#if defined(OTM1287A_KD101N55_31NA_A3_WXGANL_DSI)  
	&otm1287a_kd101n55_31na_a3_wxganl_dsi_lcm_drv,
#endif

#if defined(JD9367_SL007PN18D1244_A00_WXGA_DSI)  
	&jd9367_sl007pn18d1244_a00_wxga_dsi_lcm_drv,
#endif

#if defined(JD9366_H101IV027004_31N_IVO_WXGA_DSI)  
	&jd9366_h101iv027004_31n_ivo_wxga_dsi_lcm_drv,
#endif

#if defined(ILI9881C_20810700180665_HD720_CPT)  
	&ili9881c_20810700180665_hd720_cpt_lcm_drv,
#endif

#if defined(SAMSUNG_BOE_WXGA_JOYPLUS)
	&samsung_boe_wxga_joyplus_lcm_drv, 
#endif

#if defined(EK79029_SL008PC21D1138_A00_WXGA_CPT)  
	&ek79029_sl008pc21d1138_a00_wxga_cpt_lcm_drv,
#endif

#if defined(HX8394D_HC695_WSVGA_BOE_DSI)  
	&hx8394d_hc695_wsvga_boe_dsi_lcm_drv,
#endif

#if defined(NT35521S_SL101PN27D0993_WXGA_INX)  
	&nt35521s_sl101pn27d0993_wxga_inx_lcm_drv,
#endif

#if defined(JD9366_CPT_WJHD069009A)
	&jd9366_cpt_wjhd069009a_lcm_drv, 
#endif

#if defined(NT35521S_N070ICE_G02_INX_WXGA_VDO)  
	&nt35521s_n070ice_g02_inx_wxga_vdo_lcm_drv,
#endif

#if defined(OTM1287_SL101PA27D1249_WXGA_AUO)  
	&otm1287_sl101pa27d1249_wxga_auo_lcm_drv,
#endif

#if defined(JD9366_DY698B30IMH18_J01_BOE_HD720)
	&jd9366_dy698b30imh18_j01_boe_hd720_lcm_drv,
#endif

#if defined(JD9366_P101NWWBP_01G_WXGA_DSI)
	&jd9366_p101nwwbp_01g_wxga_dsi_lcm_drv,
#endif

#if defined(NT35523B_WJWX080030A_WXGA_BOE)  
	&nt35523b_wjwx080030a_wxga_boe_lcm_drv,
#endif

#if defined(JD9366_20810700200051_BOE7_WXGA)  
	&jd9366_20810700200051_boe7_wxga_lcm_drv,
#endif

#if defined(JD9365_K695_BM2B602A_WSVGA_BOE)
	&jd9365_k695_bm2b602a_wsvga_boe_lcm_drv,
#endif

#if defined(JD9366_AUO8_WXGA_VDO)
	&jd9366_auo8_wxga_vdo_lcm_drv,
#endif

#if defined(HX8394C_ZS080BH3107E3H7II_BOE8_WXGA)
	&hx8394c_zs080bh3107e3h7ii_boe8_wxga_lcm_drv,
#endif

#if defined(EK79029_WJWX080016A_WXGA_BOE8)
	&ek79029_wjwx080016a_wxga_boe8_lcm_drv,
#endif

#if defined(OTM1287A_AUO101_WXGA_DSI)
	&otm1287a_auo101_wxga_dsi_lcm_drv,
#endif

#if defined(JD9366_CPT8_WXGA_VDO_DSI)
	&jd9366_cpt8_wxga_vdo_dsi_lcm_drv,
#endif

#if defined(JD9367_KD080D34_30NI_WXGA_INX8)
	&jd9367_kd080d34_30ni_wxga_inx8_lcm_drv,
#endif

#if defined(JD9366_K101_IM2AA02A_WXGA_AUO101)
	&jd9366_k101_im2aa02a_wxga_auo101_lcm_drv,
#endif

#if defined(JD9364_K101_IM2OA02A_WXGA_AUO101)
	&jd9364_k101_im2oa02a_wxga_auo101_lcm_drv,
#endif

#if defined(YX070lIPS30_WXGA_DSI_VDO)
	&yx070lips30_wxga_dsi_vdo_lcm_drv,
#endif

#if defined(OTM1287A_HSX101U40AM27A_WXGA_AUO101)
	&otm1287a_hsx101u40am27a_wxga_auo101_lcm_drv,
#endif

#if defined(EK79029_ZS080BE3005E3H7II_WXGA_BOE8)
	&ek79029_zs080be3005e3h7ii_wxga_boe8_lcm_drv,
#endif

#if defined(JD9366_20810800210255_WXGA_CPT_VDO)
	&jd9366_20810800210255_wxga_cpt_vdo_lcm_drv,
#endif

#if defined(JD9365AA_K101_IM2CA02_A_WXGA_AUO101)
	&jd9365aa_k101_im2ca02_a_wxga_auo101_lcm_drv,
#endif

#if defined(JD9365AA_K101_IM2CA02_B_WXGA_AUO101)
	&jd9365aa_k101_im2ca02_b_wxga_auo101_lcm_drv,
#endif

#if defined(NT35521S_KD101N66_40NI_B2_WXGA_INX101)
	&nt35521s_kd101n66_40ni_b2_wxga_inx101_lcm_drv,
#endif

#if defined(TF070MC124_IPS_CPT7_DSI)
	&tf070mc124_ips_cpt7_dsi_lcm_drv,
#endif

#if defined(OTA7290B_20810700160048_WSVGA_AUO695)
	&ota7290b_20810700160048_wsvga_auo695_lcm_drv,
#endif

#if defined(TF070MC124_IPS_BOE7_DSI)
	&tf070mc124_ips_boe7_dsi_lcm_drv,
#endif

#if defined(JD9367_WXGA_QC_DSI_VDO)
	&jd9367_wxga_qc_dsi_vdo_lcm_drv,
#endif

#if defined(JD9367_HNH_INX8_WXGA)
	&jd9367_hnh_inx8_wxga_lcm_drv,
#endif

#if defined(JD9365_P101NWWBP_01G_R9M_WXGA_AUO101)
	&jd9365_p101nwwbp_01g_r9m_wxga_auo101_lcm_drv,
#endif

#if defined(RM68200_AUO_WXGA_CC80XY)   
	&rm68200_auo_wxga_cc80xy_lcm_drv,
#endif

#if defined(JD9366_SM080_142A1_IVO_WXGA8)   
	&jd9366_sm080_142a1_ivo_wxga8_lcm_drv,
#endif

#if defined(TF070MC124_IPS_HSD7_DSI)   
	&tf070mc124_ips_hsd7_dsi_lcm_drv,
#endif

#if defined(RM72014_AUO8_WXGA_DSI)
	&rm72014_auo8_wxga_dsi_lcm_drv,
#endif

#if defined(JD9366_PH080NB20A_WXGA_CPT8)
	&jd9366_ph080nb20a_wxga_cpt8_lcm_drv,
#endif

#if defined(NT35521_WXGA_CPT_DSI_VDO_RST33)
	&nt35521_wxga_cpt_dsi_vdo_rst33_lcm_drv, 
#endif	

#if defined(JD9365_WJWX101027B_WXGA_HSD101)
	&jd9365_wjwx101027b_wxga_hsd101_lcm_drv, 
#endif	

#if defined(JD9366_HSX101ZI_31MI27G_WXGA_IVO101)
	&jd9366_hsx101zi_31mi27g_wxga_ivo101_lcm_drv,
#endif

#if defined(JD9366_K080_IM2H801_A_WXGA_HSD8)
	&jd9366_k080_im2h801_a_wxga_hsd8_lcm_drv,
#endif

#if defined(JD9365_WJWX080056A_WXGA_HSD8)
	&jd9365_wjwx080056a_wxga_hsd8_lcm_drv,
#endif

#if defined(EK79029_HSX080MLI21A_MIPI31_WXGA_IVO8)
	&ek79029_hsx080mli21a_mipi31_wxga_ivo8_lcm_drv,
#endif


#if defined(SAMSUNG_20810700200057_WXGA_BOE7)
	&samsung_20810700200057_wxga_boe7_lcm_drv,
#endif

#if defined(JD9366_CPT_WJHD069009A_FP7721)
	&jd9366_cpt_wjhd069009a_fp7721_lcm_drv,
#endif

#if defined(EK79029_XPP080V190_WXGA_IVO8)
	&ek79029_xpp080v190_wxga_ivo8_lcm_drv,
#endif

#if defined(NT35521S_N070ICN_PB1_INX_WXGA_VDO)  
	&nt35521s_n070icn_pb1_inx_wxga_vdo_lcm_drv,
#endif

#if defined(HX8394D_WXGA_BOE101_DSI)  
	&hx8394d_wxga_boe101_dsi_lcm_drv,
#endif

#if defined(JD9367_YF070L01_WXGA_INX7)  
	&jd9367_yf070l01_wxga_inx7_lcm_drv,
#endif

#if defined(EK79029_IPS_CPT_WXGA_FY80)
	&ek79029_ips_cpt_wxga_fy80_lcm_drv,
#endif

#if defined(HX8394D_WXGA_DSI_VDO)
	&hx8394d_wxga_dsi_vdo_lcm_drv,
#endif

#if defined(HX8394C_BOE_WXGA_DSI_VDO)
	&hx8394c_boe_wxga_dsi_vdo_lcm_drv,
#endif

#if defined(HX8394F_HSD_HD720_DSI_VDO)
	&hx8394f_hsd_hd720_dsi_vdo_lcm_drv,
#endif


#if defined(JD9364_WJWX101070A)
    &jd9364_wjwx101070a_lcm_drv,
#endif

#if defined(NT51021_WUXGA_SL008PN24D1092_E00)
    &nt51021_wuxga_sl008pn24d1092_e00_lcm_drv,
#endif

#if defined(NT51021_WUXGA_QC_SL_DSI_VDO)
    &nt51021_wuxga_qc_sl_dsi_vdo_lcm_drv,
#endif

#if defined(NT51021_SL101PN28D1127_WUXGA_DSI)
    &nt51021_sl101pn28d1127_wuxga_dsi_lcm_drv,
#endif

#if defined(HX8394D_PBT025_WSVGA_BOE_DSI)
	&hx8394d_pbt025_wsvga_boe_dsi_lcm_drv,
#endif

#if defined(ILI6180_KD070D5040NIA20_WSVGA_INX695)
	&ili6180_kd070d5040nia20_wsvga_inx695_lcm_drv,
#endif

#if defined(JD9366_WXGA_DSI_CPT8)
	&jd9366_wxga_dsi_cpt8_lcm_drv,
#endif

#if defined(JD9366_JF70056W20CH1_WXGA_BOE7)
	&jd9366_jf70056w20ch1_wxga_boe7_lcm_drv,
#endif

#if defined(JD9366_SL008PB21D1439_A00_WXGA_BOE8)
    &jd9366_sl008pb21d1439_a00_wxga_boe8_lcm_drv,
#endif

#if defined(JD9366_CPT101_IPS_DSI)	
	&jd9366_cpt101_ips_dsi_lcm_drv, 
#endif

#if defined(JD9365_K101_IM2BA02_C_WXGA_BOE101)  
	&jd9365_k101_im2ba02_c_wxga_boe101_lcm_drv,
#endif

#if defined(NT51021_WSVGA_INX695_DSI)
	&nt51021_wsvga_inx695_dsi_lcm_drv,
#endif

#if defined(JD9366_PBT080WXB30I057_WXGA_BOE8)	
	&jd9366_pbt080wxb30i057_wxga_boe8_lcm_drv, 
#endif

#if defined(JD9365_PBT080WXH30I062_WXGA_HSD8)	
	&jd9365_pbt080wxh30i062_wxga_hsd8_lcm_drv, 
#endif

#if defined(OTM1287A_WJWX101092B_WXGA_AUO101)
	&otm1287a_wjwx101092b_wxga_auo101_lcm_drv,
#endif

#if defined(ILI9881C_20811010320060_WXGA_CPT_VDO)
	&ili9881c_20811010320060_wxga_cpt_vdo_lcm_drv,
#endif

#if defined(JD9366_K080_IM2C801_A_WXGA_CPT8)
    &jd9366_k080_im2c801_a_wxga_cpt8_lcm_drv,
#endif

#if defined(OTA7290B_K101MM2BA02_C_WUXGA_BOE101)
	&ota7290b_k101mm2ba02_c_wuxga_boe101_lcm_drv,
#endif

#if defined(JD9366_WJWX070138A_WXGA_BOE7)
	&jd9366_wjwx070138a_wxga_boe7_lcm_drv,
#endif

#if defined(EK79029_WJWX080063A_WXGA_BOE8)
	&ek79029_wjwx080063a_wxga_boe8_lcm_drv,
#endif

#if defined(HX8394D_HSD8_WXGA_DSI)
	&hx8394d_hsd8_wxga_dsi_lcm_drv,
#endif

#if defined(NT51021_WUXGA_SL_BOE101)  
	&nt51021_wuxga_sl_boe101_lcm_drv,
#endif

#if defined(IT6151_LP079QX1_EDP_DSI_VIDEO)
	&it6151_lp079qx1_edp_dsi_video_lcm_drv,
#endif

#if defined(TV080X0M_NS_XGANL_BOE)
	&tv080x0m_ns_xganl_boe_lcm_drv,
#endif


#endif // End not defined(MTK_LCM_DEVICE_TREE_SUPPORT)
};


unsigned char lcm_name_list[][128] = {
#if defined(HX8392A_DSI_CMD)
	"hx8392a_dsi_cmd",
#endif

#if defined(HX8392A_DSI_VDO)
	"hx8392a_vdo_cmd",
#endif

#if defined(HX8392A_DSI_CMD_FWVGA)
	"hx8392a_dsi_cmd_fwvga",
#endif

#if defined(OTM9608_QHD_DSI_CMD)
	"otm9608a_qhd_dsi_cmd",
#endif

#if defined(OTM9608_QHD_DSI_VDO)
	"otm9608a_qhd_dsi_vdo",
#endif
};

#define LCM_COMPILE_ASSERT(condition) LCM_COMPILE_ASSERT_X(condition, __LINE__)
#define LCM_COMPILE_ASSERT_X(condition, line) LCM_COMPILE_ASSERT_XX(condition, line)
#define LCM_COMPILE_ASSERT_XX(condition, line) char assertion_failed_at_line_##line[(condition) ? 1 : -1]

unsigned int lcm_count = sizeof(lcm_driver_list) / sizeof(LCM_DRIVER *);
LCM_COMPILE_ASSERT(0 != sizeof(lcm_driver_list) / sizeof(LCM_DRIVER *));
#if defined(NT35520_HD720_DSI_CMD_TM) | defined(NT35520_HD720_DSI_CMD_BOE) | \
	defined(NT35521_HD720_DSI_VDO_BOE) | defined(NT35521_HD720_DSI_VIDEO_TM)
static unsigned char lcd_id_pins_value = 0xFF;

/**
 * Function:       which_lcd_module_triple
 * Description:    read LCD ID PIN status,could identify three status:highlowfloat
 * Input:           none
 * Output:         none
 * Return:         LCD ID1|ID0 value
 * Others:
 */
unsigned char which_lcd_module_triple(void)
{
	unsigned char  high_read0 = 0;
	unsigned char  low_read0 = 0;
	unsigned char  high_read1 = 0;
	unsigned char  low_read1 = 0;
	unsigned char  lcd_id0 = 0;
	unsigned char  lcd_id1 = 0;
	unsigned char  lcd_id = 0;
	/*Solve Coverity scan warning : check return value*/
	unsigned int ret = 0;

	/*only recognise once*/
	if (0xFF != lcd_id_pins_value)
		return lcd_id_pins_value;

	/*Solve Coverity scan warning : check return value*/
	ret = mt_set_gpio_mode(GPIO_DISP_ID0_PIN, GPIO_MODE_00);
	if (0 != ret)
		LCD_DEBUG("ID0 mt_set_gpio_mode fail\n");

	ret = mt_set_gpio_dir(GPIO_DISP_ID0_PIN, GPIO_DIR_IN);
	if (0 != ret)
		LCD_DEBUG("ID0 mt_set_gpio_dir fail\n");

	ret = mt_set_gpio_pull_enable(GPIO_DISP_ID0_PIN, GPIO_PULL_ENABLE);
	if (0 != ret)
		LCD_DEBUG("ID0 mt_set_gpio_pull_enable fail\n");

	ret = mt_set_gpio_mode(GPIO_DISP_ID1_PIN, GPIO_MODE_00);
	if (0 != ret)
		LCD_DEBUG("ID1 mt_set_gpio_mode fail\n");

	ret = mt_set_gpio_dir(GPIO_DISP_ID1_PIN, GPIO_DIR_IN);
	if (0 != ret)
		LCD_DEBUG("ID1 mt_set_gpio_dir fail\n");

	ret = mt_set_gpio_pull_enable(GPIO_DISP_ID1_PIN, GPIO_PULL_ENABLE);
	if (0 != ret)
		LCD_DEBUG("ID1 mt_set_gpio_pull_enable fail\n");

	/*pull down ID0 ID1 PIN*/
	ret = mt_set_gpio_pull_select(GPIO_DISP_ID0_PIN, GPIO_PULL_DOWN);
	if (0 != ret)
		LCD_DEBUG("ID0 mt_set_gpio_pull_select->Down fail\n");

	ret = mt_set_gpio_pull_select(GPIO_DISP_ID1_PIN, GPIO_PULL_DOWN);
	if (0 != ret)
		LCD_DEBUG("ID1 mt_set_gpio_pull_select->Down fail\n");

	/* delay 100ms , for discharging capacitance*/
	mdelay(100);
	/* get ID0 ID1 status*/
	low_read0 = mt_get_gpio_in(GPIO_DISP_ID0_PIN);
	low_read1 = mt_get_gpio_in(GPIO_DISP_ID1_PIN);
	/* pull up ID0 ID1 PIN */
	ret = mt_set_gpio_pull_select(GPIO_DISP_ID0_PIN, GPIO_PULL_UP);
	if (0 != ret)
		LCD_DEBUG("ID0 mt_set_gpio_pull_select->UP fail\n");

	ret = mt_set_gpio_pull_select(GPIO_DISP_ID1_PIN, GPIO_PULL_UP);
	if (0 != ret)
		LCD_DEBUG("ID1 mt_set_gpio_pull_select->UP fail\n");

	/* delay 100ms , for charging capacitance */
	mdelay(100);
	/* get ID0 ID1 status */
	high_read0 = mt_get_gpio_in(GPIO_DISP_ID0_PIN);
	high_read1 = mt_get_gpio_in(GPIO_DISP_ID1_PIN);

	if (low_read0 != high_read0) {
		/*float status , pull down ID0 ,to prevent electric leakage*/
		ret = mt_set_gpio_pull_select(GPIO_DISP_ID0_PIN, GPIO_PULL_DOWN);
		if (0 != ret)
			LCD_DEBUG("ID0 mt_set_gpio_pull_select->Down fail\n");

		lcd_id0 = LCD_HW_ID_STATUS_FLOAT;
	} else if ((LCD_HW_ID_STATUS_LOW == low_read0) && (LCD_HW_ID_STATUS_LOW == high_read0)) {
		/*low status , pull down ID0 ,to prevent electric leakage*/
		ret = mt_set_gpio_pull_select(GPIO_DISP_ID0_PIN, GPIO_PULL_DOWN);
		if (0 != ret)
			LCD_DEBUG("ID0 mt_set_gpio_pull_select->Down fail\n");

		lcd_id0 = LCD_HW_ID_STATUS_LOW;
	} else if ((LCD_HW_ID_STATUS_HIGH == low_read0) && (LCD_HW_ID_STATUS_HIGH == high_read0)) {
		/*high status , pull up ID0 ,to prevent electric leakage*/
		ret = mt_set_gpio_pull_select(GPIO_DISP_ID0_PIN, GPIO_PULL_UP);
		if (0 != ret)
			LCD_DEBUG("ID0 mt_set_gpio_pull_select->UP fail\n");

		lcd_id0 = LCD_HW_ID_STATUS_HIGH;
	} else {
		LCD_DEBUG(" Read LCD_id0 error\n");
		ret = mt_set_gpio_pull_select(GPIO_DISP_ID0_PIN, GPIO_PULL_DISABLE);
		if (0 != ret)
			LCD_DEBUG("ID0 mt_set_gpio_pull_select->Disbale fail\n");

		lcd_id0 = LCD_HW_ID_STATUS_ERROR;
	}


	if (low_read1 != high_read1) {
		/*float status , pull down ID1 ,to prevent electric leakage*/
		ret = mt_set_gpio_pull_select(GPIO_DISP_ID1_PIN, GPIO_PULL_DOWN);
		if (0 != ret)
			LCD_DEBUG("ID1 mt_set_gpio_pull_select->Down fail\n");

		lcd_id1 = LCD_HW_ID_STATUS_FLOAT;
	} else if ((LCD_HW_ID_STATUS_LOW == low_read1) && (LCD_HW_ID_STATUS_LOW == high_read1)) {
		/*low status , pull down ID1 ,to prevent electric leakage*/
		ret = mt_set_gpio_pull_select(GPIO_DISP_ID1_PIN, GPIO_PULL_DOWN);
		if (0 != ret)
			LCD_DEBUG("ID1 mt_set_gpio_pull_select->Down fail\n");

		lcd_id1 = LCD_HW_ID_STATUS_LOW;
	} else if ((LCD_HW_ID_STATUS_HIGH == low_read1) && (LCD_HW_ID_STATUS_HIGH == high_read1)) {
		/*high status , pull up ID1 ,to prevent electric leakage*/
		ret = mt_set_gpio_pull_select(GPIO_DISP_ID1_PIN, GPIO_PULL_UP);
		if (0 != ret)
			LCD_DEBUG("ID1 mt_set_gpio_pull_select->UP fail\n");

		lcd_id1 = LCD_HW_ID_STATUS_HIGH;
	} else {

		LCD_DEBUG(" Read LCD_id1 error\n");
		ret = mt_set_gpio_pull_select(GPIO_DISP_ID1_PIN, GPIO_PULL_DISABLE);
		if (0 != ret)
			LCD_DEBUG("ID1 mt_set_gpio_pull_select->Disable fail\n");

		lcd_id1 = LCD_HW_ID_STATUS_ERROR;
	}
#ifdef BUILD_LK
	dprintf(CRITICAL, "which_lcd_module_triple,lcd_id0:%d\n", lcd_id0);
	dprintf(CRITICAL, "which_lcd_module_triple,lcd_id1:%d\n", lcd_id1);
#else
	LCD_DEBUG("which_lcd_module_triple,lcd_id0:%d\n", lcd_id0);
	LCD_DEBUG("which_lcd_module_triple,lcd_id1:%d\n", lcd_id1);
#endif
	lcd_id =  lcd_id0 | (lcd_id1 << 2);

#ifdef BUILD_LK
	dprintf(CRITICAL, "which_lcd_module_triple,lcd_id:%d\n", lcd_id);
#else
	LCD_DEBUG("which_lcd_module_triple,lcd_id:%d\n", lcd_id);
#endif

	lcd_id_pins_value = lcd_id;
	return lcd_id;
}
#endif
