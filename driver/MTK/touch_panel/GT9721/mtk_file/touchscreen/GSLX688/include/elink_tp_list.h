/*
 * Copyright (c) 2018 elinktek
 * Author:	phil
 * Email :	ph@elinktek.com
 * Version:	20180226
 * Desc:	kernel tp_list header file
 */
#ifndef __ELINK_TP_LIST_H__
#define __ELINK_TP_LIST_H__

#include <gsl_common.h>

//--------------------------------------------
//.h file

//590
#include "configs/GSL1688_W590_DZ_OGS.h"
#include "configs/GSL1688_W590_ZHZY_GG.h"
//601
//608
#include "configs/GSL1688_W608_DZ_OGS.h"
#include "configs/GSL1688_W608_QHD_DZ_OGS.h"
//695 
#include "configs/L695_GSL915_FC_PG.h"
#include "configs/L695_GSL915_PG_HK.h"
#include "configs/L695_GSL915_ZHZY_PG.h"
//698
#include "configs/GSL1688_W698_DZ_OGS.h"
#include "configs/GSL1688_W698_WSVGA_DZ_OGS.h"
//706
#include "configs/GSL1688_W706_CTD_PG.h"
#include "configs/GSL1688_W706_DZ_OGS.h"
#include "configs/GSL1688_W706_FC_PG.h"
#include "configs/GSL1688_W706_HT_PG.h"
#include "configs/GSL1688_W706_SG_OGS.h"
#include "configs/GSL1688_W706_MGL_PG.h"
#include "configs/GSL1688_W706_XLL_PG.h"
#include "configs/W706_GSL1688_CTD_PG_HAVE_KEY.h"
#include "configs/W706_GSL1688_FC_PG_HAVE_KEY.h"
#include "configs/W706_GSL1688_HT_PG_HAVE_KEY.h"
#include "configs/GSL1688_W706_QCH_PG.h"
#include "configs/GSL1688_L706JF_PB_PG.h"
#include "configs/GSL1688_706_QCY_PG.h"
//7Z3 
#include "configs/GSL1688_L7Z31_PB_PG.h"
#include "configs/GSL1688_L7Z32_PB_PG.h"
//718
#include "configs/GSL1688_W718_FC_PG.h"
#include "configs/GSL1688_W718_HT_PG.h"
#include "configs/GSL1688_W718_OGS_DZ.h"
//710
#include "configs/GSL915_E710_DZ_OGS.h"
//785
#include "configs/GSL3676_W785_PB_GG.h"
#include "configs/GSL3676_W785_YTG_GG.h"
//798
#include "configs/GSL3676_W798_DZH_GG.h"
#include "configs/GSL3676_W798_DZ_OGS.h"
#include "configs/GSL3676_W798_HH_OGS.h"
#include "configs/GSL3676_W798_RSS_GG.h"
//801
#include "configs/GSL3676_W801_CTD_PG.h"
#include "configs/GSL3676_W801_DZH_GG.h"
#include "configs/GSL3676_W801_DZ_OGS.h"
#include "configs/GSL3676_W801_RSS_GG.h"
#include "configs/GSL3676_W801_XCL_OGS.h"
#include "configs/GSL3676_W801_ZC_GG.h"
//806
#include "configs/GSL915_W806_DPT_PG.h"
#include "configs/GSL915_W806_DZ_OGS.h"
#include "configs/GSL915_W806_JND_OGS.h"
#include "configs/GSL915_W806_WTGD_GG.h"
#include "configs/GSL915_W806_YTG_GG.h"
//8T1
#include "configs/GSL915_L8T1_DZ_OGS_KEY.h"
//805
#include "configs/GSL3670_W803_PB_GG.h"
//808
#include "configs/GSL3670_W808_FC_PG.h"
#include "configs/GSL3670_L808_CJL_PG.h"//add by heliang 20180726

//888
#include "configs/GSL3676_W888_HK_PG.h" //for 8' size
#include "configs/GSL3676_W888_HS_PG.h" //for 10' size
//8T1
#include "configs/GSL915_L8T1_DZ_OGS.h"
//100
#include "configs/GSL3676_L100_YTG_GG.h"
#include "configs/GSL3676_H100_HK_PG.h"
#include "configs/GSL3676_H100_HK_GG.h"
#include "configs/GSL3676_H100_XLL_GG.h"
#include "configs/GSL3676_H100_XLL_PG.h"
#include "configs/GSL3676_H100_DZ_OGS.h"
#include "configs/GSL3676_H106_XLL_2_GG.h"
#include "configs/GSL3676_W100_YTG_GG_1920.h"//add by heliang 20180929

//W100
#include "configs/GSL3676_W100_YTG_GG.h"//add by heliang 20181101

//101
#include "configs/GSL3676_W101_RS_GG.h"
#include "configs/GSL3676_W101_RSS_GG.h"
#include "configs/GSL3676_W101_WTGD_GG.h"
#include "configs/GSL3676_W101_YTG_GG.h"
#include "configs/GSL3676_W101_WJ_PG.h"
#include "configs/GSL3676_W101_FC_PG.h"
#include "configs/GSL3676_W101_HT_PG.h"
#include "configs/GSL3676_W101_ZHZY_PG.h"
#include "configs/GSL3676_W101_RP_GG.h"
#include "configs/GSL3676_L101_YTG_GG.h"//add by heliang 20160721
#include "configs/GSL3676_L101_HT_PG_ARCHOS.h" //ELink_Matt@20170607 add 
//102
#include "configs/GSL3676_W102_DH_PG.h"
#include "configs/GSL3676_W102_ZHZY_PG.h"
#include "configs/GSL3676_W102_YTG_GG.h"
#include "configs/GSL3676_W102_FC_PG.h"
//103
#include "configs/GSL3676_W103_HKGD_PG.h"
#include "configs/GSL3676_W103_DH_PG.h"
#include "configs/GSL3676_W103_WTGD_PG.h"
#include "configs/GSL3676_W103_WJ_PG.h"
#include "configs/GSL3676_W103_HK_PG.h"
//108
#include "configs/GSL3676_W108_YTG_GG.h"
//109
#include "configs/GSL3676_W109_HT_PG.h"
//108J
#include "configs/GSL3676_E108J_FC_PG.h"
#include "configs/GSL3676_L108J_HK_PG.h"
//806J
#include "configs/GSL3676_L806J_HK_PG.h"
#include "configs/GSL3676_L806J_FC_PG.h"
// #include "configs/GSL3676_L806JF_HK_PG_8.h"//add by heliang 20180416

//L808J
#include "configs/GSL915_L808_RP_PG.h"

//863J
#include "configs/GSL3676_L863J_XC_PG.h"

//L863J
#include "configs/GSL1686_L863_PG_SQ_7_600.h"//by heliang 20180611
#include "configs/GSL1686_L863J_DZ70A68_OGS_7.h"
#include "configs/GSL1686_L863J_DZ70A69_OGS_7.h"
#include "configs/GSL3676_L863J_DZ_OGS_80B21.h"
#include "configs/GSL3676_L863J_DZ_OGS_80B22.h"
#include "configs/GSL3676_L863J_DZ_OGS_80B23_KEY.h"
#include "configs/GSL3676_L863J_DZ_OGS_10_1.h"
#include "configs/GSL3676_L863J_DZ_OGS_10A79.h"
#include "configs/GSL3676_E863_ZYD_PG_8.h"//by heliang 20180907

#include "configs/GSL1688E_L863J_XC_PG_7.h"
#include "configs/GSL3676_L863J_XC_PG_8.h"
#include "configs/GSL3676_L863J_XC_PG_10_1200.h"
#include "configs/GSL3676_L863J_XC_PG_10_1920.h"
#include "configs/GSL3676_L863_XHS_PG_10.h"
#include "configs/GSL3676_L863_XHS_PG_8.h"
#include "configs/GSL3676_E863G_SQ_GG_10.h" //by heliang 20180409
#include "configs/GSL3676_L863J_XC_PG_10_1280.h"//by heliang 20180508
#include "configs/GSL3676_L863_HK_PG_10_1280.h"//by heliang 20180514
#include "configs/W100_GSL3676_HK_PG_10_1280.h"//by heliang 20180810

//L7U4J
#include "configs/GSL3676_L7U4J_ZYD_GG_10.1.h"
#include "configs/GSL3676_L7U4J_ZYD_GG_8.h"
#include "configs/GSL1688_L7U4J_CY_GG_7.h"

//L7C7WA   7'
#include "configs/GSL1688_L7C7WA_ZYD_PG_600.h"//by heliang 20180608

//L706   7'
#include "configs/L706_GSL1686_PG_DP.h"//by heliang 20180629

//863
#include "configs/863_GSL3676_DZ_OGS_10.h"

//t8s1
#include "configs/GSL3692_T8S1GF_DZ_OGS_10_1.h"
#include "configs/GSL3692_T8S1GF_DZ_OGS_8.h"

//T6P2
#include "configs/GSL915_T6P2_PB_GG.h"

//--------------------------------------------
#if 0
//tp key
#ifdef TPD_HAVE_BUTTON 
static int tpd_keys_local[TPD_KEY_COUNT] = TPD_KEYS;
static int tpd_keys_local_home[1] = {KEY_HOMEPAGE};
static int tpd_keys_local_2[2] = {KEY_APPSELECT, KEY_BACK}; //Elink_Matt@20170411 add for two keys in tp(L8T1)
//590
static int tpd_keys_dim_local_590_dz_ogs[TPD_KEY_COUNT][4] = TPD_KEYS_DIM_590_DZ_OGS;
static int tpd_keys_dim_local_590_zhzy_gg[TPD_KEY_COUNT][4] = TPD_KEYS_DIM_590_ZHZY_GG;
//706
static int tpd_keys_dim_local_706_fc_pg[TPD_KEY_COUNT][4] = TPD_KEYS_DIM_706_FC_PG;
static int tpd_keys_dim_local_706_ctd_pg[TPD_KEY_COUNT][4] = TPD_KEYS_DIM_706_CTD_PG; 
static int tpd_keys_dim_local_706_ht_pg[TPD_KEY_COUNT][4] = TPD_KEYS_DIM_706_HT_PG; 
//785
static int tpd_keys_dim_local_785_ytg_gg[TPD_KEY_COUNT][4] = TPD_KEYS_DIM_785_YTG_GG;
//798
static int tpd_keys_dim_local_798_dzh_gg[1][4] = TPD_KEYS_DIM_798_DZH_GG;
static int tpd_keys_dim_local_798_dz_ogs[1][4] = TPD_KEYS_DIM_798_DZ_OGS;
static int tpd_keys_dim_local_798_rss_gg[1][4] = TPD_KEYS_DIM_798_RSS_GG;
static int tpd_keys_dim_local_798_hh_ogs[1][4] = TPD_KEYS_DIM_798_HH_OGS;
//108
static int tpd_keys_dim_local_108_ytg_gg[TPD_KEY_COUNT][4] = TPD_KEYS_DIM_108_YTG_GG;

//L8T1
static int tpd_keys_dim_local_8T1_dz_ogs[2][4] = TPD_KEYS_DIM_8T1_DZ_OGS;  //Elink_zqb@@20170411 add for L8T1 TP
#endif
#endif
//--------------------------------------------


//590
// static const ELINK_SILEAD_CONFIG mx590_tp_data[] = {
   // {"MX590_DZ_OGS", {0xff,0x80,0x78,0x17}, gsl_config_data_id_590_DZ_OGS, GSL1688_FW_590_DZ_OGS, ARRAY_SIZE(GSL1688_FW_590_DZ_OGS), tpd_keys_dim_local_590_dz_ogs},
   // {"MX590_ZHZY_GG", {0xff,0x80,0x78,0x27}, gsl_config_data_id_590_ZHZY_GG, GSL1688_FW_590_ZHZY_GG, ARRAY_SIZE(GSL1688_FW_590_ZHZY_GG), tpd_keys_dim_local_590_zhzy_gg},
   // {"MX590_DEFAULT", {0xff,0xff,0xff,0xff}, gsl_config_data_id_590_DZ_OGS, GSL1688_FW_590_DZ_OGS, ARRAY_SIZE(GSL1688_FW_590_DZ_OGS), tpd_keys_dim_local_590_dz_ogs},
// };
//601
//608
static const ELINK_SILEAD_CONFIG mx608_tp_data_dz_ogs[] = {
   {"MX608_TP_DZ_OGS", {0xff,0xff,0xff,0xff}, gsl_config_data_id_608_DZ_OGS, GSL1688_FW_608_DZ_OGS,  ARRAY_SIZE(GSL1688_FW_608_DZ_OGS), NULL},
};
static const ELINK_SILEAD_CONFIG mx608_tp_data_qhd_dz_ogs[] = {
   {"MX608_TP_QHD_DZ_OGS", {0xff,0xff,0xff,0xff}, gsl_config_data_id_608_QHD_DZ_OGS, GSL1688_FW_608_QHD_DZ_OGS,  ARRAY_SIZE(GSL1688_FW_608_QHD_DZ_OGS), NULL},
};
//695 
static const ELINK_SILEAD_CONFIG mx695_tp_data[] = {
	{"MX695_FC_PG",  {0xfc,0x00,0x10,0x00}, gsl_config_data_id_L695_PG_FC, GSLX680_FW_L695_PG_FC,  ARRAY_SIZE(GSLX680_FW_L695_PG_FC), NULL},
	{"MX695_HK_PG",  {0xfc,0x00,0x08,0x00}, gsl_config_data_id_L695_PG_HK, GSLX680_FW_L695_PG_HK,  ARRAY_SIZE(GSLX680_FW_L695_PG_HK), NULL},
	{"MX695_ZHZY_PG", {0xfc,0x00,0x04,0x00}, gsl_config_data_id_L695_ZHZY_PG, GSL915_FW_L695_ZHZY_PG,  ARRAY_SIZE(GSL915_FW_L695_ZHZY_PG), NULL},
    {"MX695_DEFAULT",{0xff,0xff,0xff,0xff}, gsl_config_data_id_L695_PG_FC, GSLX680_FW_L695_PG_FC,  ARRAY_SIZE(GSLX680_FW_L695_PG_FC), NULL},	
};
//698
static const ELINK_SILEAD_CONFIG mx698_tp_data[] = {
   {"MX698_DZ_OGS", {0xfc,0x00,0x00,0x00}, gsl_config_data_id_698_DZ_OGS, GSL1688_FW_698_DZ_OGS,  ARRAY_SIZE(GSL1688_FW_698_DZ_OGS), NULL},
   {"MX698_DEFAULT", {0xff,0xff,0xff,0xff}, gsl_config_data_id_698_DZ_OGS, GSL1688_FW_698_DZ_OGS,  ARRAY_SIZE(GSL1688_FW_698_DZ_OGS), NULL},
};
static const ELINK_SILEAD_CONFIG mx698_tp_data_wsvga[] = {
   {"MX698_WSVGA_DZ_OGS", {0xfc,0x00,0x00,0x00}, gsl_config_data_id_698_WSVGA_DZ_OGS, GSL1688_FW_698_WSVGA_DZ_OGS,  ARRAY_SIZE(GSL1688_FW_698_WSVGA_DZ_OGS), NULL},
   {"MX698_WSVGA_DEFAULT", {0xff,0xff,0xff,0xff}, gsl_config_data_id_698_WSVGA_DZ_OGS, GSL1688_FW_698_WSVGA_DZ_OGS,  ARRAY_SIZE(GSL1688_FW_698_WSVGA_DZ_OGS), NULL},
};
//706
static const ELINK_SILEAD_CONFIG mx706_tp_data[] = {
   {"MX706_CTD_PG", {0xff,0x80,0x78,0x17}, gsl_config_data_id_706_CTD_PG, GSL1688_FW_706_CTD_PG,  ARRAY_SIZE(GSL1688_FW_706_CTD_PG), NULL},
   {"MX706_FC_PG",  {0xff,0x80,0x78,0x0f}, gsl_config_data_id_706_FC_PG, GSL1688_FW_706_FC_PG,  ARRAY_SIZE(GSL1688_FW_706_FC_PG), NULL},
   {"MX706_DZ_OGS", {0xff,0x80,0x78,0x27}, gsl_config_data_id_706_DZ_OGS, GSL1688_FW_706_DZ_OGS,  ARRAY_SIZE(GSL1688_FW_706_DZ_OGS), NULL},
   {"MX706_SG_OGS", {0xff,0x80,0x78,0x47}, gsl_config_data_id_706_SG_OGS, GSL1688_FW_706_SG_OGS,  ARRAY_SIZE(GSL1688_FW_706_SG_OGS), NULL},
   {"MX706_HT_PG", {0xff,0xc0,0x78,0x07}, gsl_config_data_id_706_HT_PG, GSL1688_FW_706_HT_PG,  ARRAY_SIZE(GSL1688_FW_706_HT_PG), NULL},
   {"MX706_MGL_PG", {0xff,0x84,0x78,0x07}, gsl_config_data_id_706_MGL_PG, GSL1688_FW_706_MGL_PG,  ARRAY_SIZE(GSL1688_FW_706_MGL_PG), NULL},
   {"MX706_XLL_PG", {0xff,0x80,0x79,0x07}, gsl_config_data_id_706_XLL_PG, GSL1688_FW_706_XLL_PG,  ARRAY_SIZE(GSL1688_FW_706_XLL_PG), NULL},
   //{"MX706_QCH_PG", {0xff,0x80,0x78,0x0f},  gsl_config_data_id_706_QCH_PG, GSL1688_FW_706_QCH_PG,  ARRAY_SIZE(GSL1688_FW_706_QCH_PG), NULL},
   {"L706JF_PB_PG", {0xff,0x82,0x78,0x07}, gsl_config_data_id_L706JF_PB_PG, GSL1688_FW_L706JF_PB_PG,  ARRAY_SIZE(GSL1688_FW_L706JF_PB_PG), NULL},
   {"MX706_QCY_PG", {0xff,0x80,0x78,0x0f}, gsl_config_data_id_706_QCY_PG, GSL1688_FW_706_QCY_PG,  ARRAY_SIZE(GSL1688_FW_706_QCY_PG), NULL},//ff80780f
   {"MX706_DEFAULT",{0xff,0xff,0xff,0xff}, gsl_config_data_id_706_CTD_PG, GSL1688_FW_706_CTD_PG,  ARRAY_SIZE(GSL1688_FW_706_CTD_PG), NULL},
}; 
//7Z3 
static const ELINK_SILEAD_CONFIG mx7Z3_tp_data[] = {
   {"MX7Z3_PB_PG", {0xff,0x80,0x78,0x17}, gsl_config_data_id_7Z3_PB_PG, GSL1688_FW_7Z3_PB_PG,  ARRAY_SIZE(GSL1688_FW_7Z3_PB_PG), NULL},
   {"MX7Z3_DEFAULT",{0xff,0xff,0xff,0xff}, gsl_config_data_id_7Z3_PB_PG, GSL1688_FW_7Z3_PB_PG,  ARRAY_SIZE(GSL1688_FW_7Z3_PB_PG), NULL},
}; 
static const ELINK_SILEAD_CONFIG mx7Z3_600x1024_tp_data[] = {
   {"MX706_PB_PG", {0xff,0x80,0x78,0x17}, gsl_config_data_id_W706_PB_PG, GSL1688_FW_W706_PB_PG,  ARRAY_SIZE(GSL1688_FW_W706_PB_PG), NULL},
   {"MX7Z3_DEFAULT",{0xff,0xff,0xff,0xff}, gsl_config_data_id_W706_PB_PG, GSL1688_FW_W706_PB_PG,  ARRAY_SIZE(GSL1688_FW_W706_PB_PG), NULL},
};
// static const ELINK_SILEAD_CONFIG mx706_tp_data_ctd_pg_have_key[] = {
   // {"MX706_TP_CTD_PG_HAVE_KEY", {0xff,0xff,0xff,0xff}, gsl_config_data_id_706_CTD_PG_HAVE_KEY, GSL1688_FW_706_CTD_PG_HAVE_KEY, ARRAY_SIZE(GSL1688_FW_706_CTD_PG_HAVE_KEY), tpd_keys_dim_local_706_ctd_pg},
// };
// static const ELINK_SILEAD_CONFIG mx706_tp_data_fc_pg_have_key[] = {
   // {"MX706_TP_FC_PG_HAVE_KEY", {0xff,0xff,0xff,0xff}, gsl_config_data_id_706_FC_PG_HAVE_KEY, ARRAY_SIZE(GSL1688_FW_706_FC_PG_HAVE_KEY), tpd_keys_dim_local_706_fc_pg},
// };
// static const ELINK_SILEAD_CONFIG mx706_tp_data_ht_pg_have_key[] = {
   // {"MX706_TP_HT_PG_HAVE_KEY", {0xff,0xff,0xff,0xff}, gsl_config_data_id_706_HT_PG_HAVE_KEY, GSL1688_FW_706_HT_PG_HAVE_KEY, ARRAY_SIZE(GSL1688_FW_706_HT_PG_HAVE_KEY), tpd_keys_dim_local_706_ht_pg},
// };
//8T1
// static const ELINK_SILEAD_CONFIG mx8T1_tp_data_dz_ogs_have_key[] = {
   // {"MX8T1_TP_DZ_OGS_HAVE_KEY", {0xff,0xff,0xff,0xff}, gsl_config_data_id_8T1_DZ_OGS_HAVE_KEY, GSL915_FW_8T1_DZ_OGS_HAVE_KEY, ARRAY_SIZE(GSL915_FW_8T1_DZ_OGS_HAVE_KEY), tpd_keys_dim_local_8T1_dz_ogs},
// };
//710
static const ELINK_SILEAD_CONFIG mx710_tp_data[] = {
   {"MX710_DZ_OGS", {0xff,0x00,0x18,0x00}, gsl_config_data_id_710_DZ_OGS, GSL915_FW_710_DZ_OGS,  ARRAY_SIZE(GSL915_FW_710_DZ_OGS), NULL},
   {"MX710_DEFAULT", {0xff,0xff,0xff,0xff}, gsl_config_data_id_710_DZ_OGS, GSL915_FW_710_DZ_OGS,  ARRAY_SIZE(GSL915_FW_710_DZ_OGS), NULL},
};
//718
static const ELINK_SILEAD_CONFIG mx718_tp_data[] = {
   {"MX718_DZ_OGS", {0xfc,0x00,0x00,0x00}, gsl_config_data_id_718_DZ_OGS, GSL1688_FW_718_DZ_OGS,  ARRAY_SIZE(GSL1688_FW_718_DZ_OGS), NULL},
   {"MX718_FC_PG", {0xfc,0x80,0x00,0x00}, gsl_config_data_id_718_FC_PG, GSL1688_FW_718_FC_PG,  ARRAY_SIZE(GSL1688_FW_718_FC_PG), NULL},
   {"MX718_HT_PG", {0xfc,0x10,0x00,0x00}, gsl_config_data_id_718_HT_PG, GSL1688_FW_718_HT_PG,  ARRAY_SIZE(GSL1688_FW_718_HT_PG), NULL},
   {"MX718_DEFAULT", {0xff,0xff,0xff,0xff}, gsl_config_data_id_718_DZ_OGS, GSL1688_FW_718_DZ_OGS,  ARRAY_SIZE(GSL1688_FW_718_DZ_OGS), NULL},
};
//785
// static const ELINK_SILEAD_CONFIG mx785_tp_data_ytg_gg[] = {
   // {"MX785_TP_YTG_GG", {0xff,0xff,0xff,0xff}, gsl_config_data_id_785_YTG_GG, GSL1688_FW_785_YTG_GG, ARRAY_SIZE(GSL1688_FW_785_YTG_GG), tpd_keys_dim_local_785_ytg_gg},
// };
//798
// static const ELINK_SILEAD_CONFIG mx798_tp_data[] = {
   // {"MX798_DZH_GG", {0x00,0x17,0x00,0x10}, gsl_config_data_id_798_DZH_GG, GSL3676_FW_798_DZH_GG, ARRAY_SIZE(GSL3676_FW_798_DZH_GG), tpd_keys_dim_local_798_dzh_gg},
   // {"MX798_DZ_OGS", {0x00,0x93,0x00,0x10}, gsl_config_data_id_798_DZ_OGS, GSL3676_FW_798_DZ_OGS, ARRAY_SIZE(GSL3676_FW_798_DZ_OGS), tpd_keys_dim_local_798_dz_ogs},
   // {"MX798_RSS_GG", {0x04,0x13,0x00,0x10}, gsl_config_data_id_798_RSS_GG, GSL3676_FW_798_RSS_GG, ARRAY_SIZE(GSL3676_FW_798_RSS_GG), tpd_keys_dim_local_798_rss_gg},
   // {"MX798_HH_OGS", {0x08,0x13,0x00,0x10}, gsl_config_data_id_798_HH_OGS, GSL3676_FW_798_HH_OGS, ARRAY_SIZE(GSL3676_FW_798_HH_OGS), tpd_keys_dim_local_798_hh_ogs},
   // {"MX798_DEFAULT", {0xff,0xff,0xff,0xff}, gsl_config_data_id_798_DZH_GG, GSL3676_FW_798_DZH_GG, ARRAY_SIZE(GSL3676_FW_798_DZH_GG), tpd_keys_dim_local_798_dzh_gg},
// };
//801
static const ELINK_SILEAD_CONFIG mx801_tp_data[] = {
   {"MX801_DZ_OGS", {0x00,0x93,0x00,0x10}, gsl_config_data_id_801_DZ_OGS, GSL3676_FW_801_DZ_OGS,  ARRAY_SIZE(GSL3676_FW_801_DZ_OGS), NULL},
   {"MX801_XCL_OGS", {0x00,0x33,0x00,0x10}, gsl_config_data_id_801_XCL_OGS, GSL3676_FW_801_XCL_OGS,  ARRAY_SIZE(GSL3676_FW_801_XCL_OGS), NULL},
   {"MX801_DZH_GG", {0x00,0x93,0x00,0x12}, gsl_config_data_id_801_DZH_GG, GSL3676_FW_801_DZH_GG,  ARRAY_SIZE(GSL3676_FW_801_DZH_GG), NULL},
   {"MX801_ZC_GG", {0x00,0x17,0x00,0x10}, gsl_config_data_id_801_ZC_GG, GSL3676_FW_801_ZC_GG,  ARRAY_SIZE(GSL3676_FW_801_ZC_GG), NULL},
   {"MX801_RSS_GG", {0x00,0x13,0x80,0x10}, gsl_config_data_id_801_RSS_GG, GSL3676_FW_801_RSS_GG,  ARRAY_SIZE(GSL3676_FW_801_RSS_GG), NULL},
   {"MX801_CTD_PG", {0x00,0x13,0xe0,0x10}, gsl_config_data_id_801_CTD_PG, GSL3676_FW_801_CTD_PG,  ARRAY_SIZE(GSL3676_FW_801_CTD_PG), NULL},
   {"MX801_DEFAULT", {0xff,0xff,0xff,0xff}, gsl_config_data_id_801_DZ_OGS, GSL3676_FW_801_DZ_OGS,  ARRAY_SIZE(GSL3676_FW_801_DZ_OGS), NULL},
};
//805
static const ELINK_SILEAD_CONFIG mx805_tp_data[] = {
   {"MX805_PB_GG",   {0xff,0xff,0xff,0xff}, gsl_config_data_id_803_PB_GG, GSLX680_FW_803_PB_GG,  ARRAY_SIZE(GSLX680_FW_803_PB_GG), NULL},    
   {"MX805_DEFAULT", {0xff,0xff,0xff,0xff}, gsl_config_data_id_803_PB_GG, GSLX680_FW_803_PB_GG,  ARRAY_SIZE(GSLX680_FW_803_PB_GG), NULL},
};
//806
static const ELINK_SILEAD_CONFIG mx806_tp_data[] = {
   {"MX806_WTGD_GG", {0xfc,0x00,0x08,0x00}, gsl_config_data_id_806_WTGD_GG, GSL915_FW_806_WTGD_GG,  ARRAY_SIZE(GSL915_FW_806_WTGD_GG), NULL},
   {"MX806_DZ_OGS", {0xfe,0x00,0x10,0x00}, gsl_config_data_id_806_DZ_OGS, GSL915_FW_806_DZ_OGS,  ARRAY_SIZE(GSL915_FW_806_DZ_OGS), NULL},
   {"MX806_JND_OGS", {0xfe,0x00,0x00,0x40}, gsl_config_data_id_806_JND_OGS, GSL915_FW_806_JND_OGS,  ARRAY_SIZE(GSL915_FW_806_JND_OGS), NULL},
   {"MX806_YTG_GG", {0xfc,0x00,0x00,0x80}, gsl_config_data_id_806_YTG_GG, GSL915_FW_806_YTG_GG,  ARRAY_SIZE(GSL915_FW_806_YTG_GG), NULL},
   {"MX806_DPT_PG", {0xfc,0x20,0x00,0x00}, gsl_config_data_id_806_DPT_PG, GSL915_FW_806_DPT_PG,  ARRAY_SIZE(GSL915_FW_806_DPT_PG), NULL},
   {"MX806_DEFAULT", {0xff,0xff,0xff,0xff}, gsl_config_data_id_806_WTGD_GG, GSL915_FW_806_WTGD_GG,  ARRAY_SIZE(GSL915_FW_806_WTGD_GG), NULL},
};

//808
static const ELINK_SILEAD_CONFIG mx808_tp_data[] = {
   {"MX808_FC_PG",   {0xfe,0x04,0x00,0x00}, gsl_config_data_id_808_FC_PG, GSL3676_FW_808_FC_PG,  ARRAY_SIZE(GSL3676_FW_808_FC_PG), NULL},    
   {"MX808_CJL_PG",  {0xfc,0x04,0x00,0x00}, gsl_config_data_id_L808_CJL_PG, GSL3676_FW_L808_CJL_PG,  ARRAY_SIZE(GSL3676_FW_L808_CJL_PG), NULL},    
   {"MX808_RP_PG",   {0xfc,0x00,0x00,0x00}, gsl_config_data_id_L808_RP_PG, GSL915_FW_L808_RP_PG,  ARRAY_SIZE(GSL915_FW_L808_RP_PG), NULL},   
   {"MX808_DEFAULT", {0xff,0xff,0xff,0xff}, gsl_config_data_id_808_FC_PG, GSL3676_FW_808_FC_PG,  ARRAY_SIZE(GSL3676_FW_808_FC_PG), NULL},
};
//888
static const ELINK_SILEAD_CONFIG mx888_tp_data[] = {
   {"MX888_HS_PG", {0x00,0x13,0x00,0x10}, gsl_config_data_id_888_HS_PG, GSL3676_FW_888_HS_PG,  ARRAY_SIZE(GSL3676_FW_888_HS_PG), NULL}, //10' size
   {"MX888_HK_PG", {0x00,0x13,0x00,0x11}, gsl_config_data_id_888_HK_PG, GSL3676_FW_888_HK_PG,  ARRAY_SIZE(GSL3676_FW_888_HK_PG), NULL},  //8' size
   {"MX888_DEFAULT", {0xff,0xff,0xff,0xff}, gsl_config_data_id_888_HK_PG, GSL3676_FW_888_HK_PG,  ARRAY_SIZE(GSL3676_FW_888_HK_PG), NULL},
};
//8T1
static const ELINK_SILEAD_CONFIG mx8T1_tp_data[] = {
   {"MX8T1_DZ_OGS", {0x00,0x00,0x5c,0x00}, gsl_config_data_id_8T1_DZ_OGS, GSL915_FW_8T1_DZ_OGS,  ARRAY_SIZE(GSL915_FW_8T1_DZ_OGS), NULL},  
   {"MX8T1_DEFAULT", {0xff,0xff,0xff,0xff}, gsl_config_data_id_8T1_DZ_OGS, GSL915_FW_8T1_DZ_OGS,  ARRAY_SIZE(GSL915_FW_8T1_DZ_OGS), NULL},
};
//100 
static const ELINK_SILEAD_CONFIG mx100_tp_data[] = {
   {"MX100_YTG_GG", {0x00,0x93,0x00,0x10}, gsl_config_data_id_100_YTG_GG, GSL3676_FW_100_YTG_GG,  ARRAY_SIZE(GSL3676_FW_100_YTG_GG), NULL},
   {"MX100_HK_PG",  {0x00,0x13,0x00,0x12}, gsl_config_data_id_100_HK_PG, GSL3676_FW_100_HK_PG,  ARRAY_SIZE(GSL3676_FW_100_HK_PG), NULL}, 
   {"MX100_HK_GG",  {0x00,0x13,0x00,0x90}, gsl_config_data_id_100_HK_GG, GSL3676_FW_100_HK_GG,  ARRAY_SIZE(GSL3676_FW_100_HK_GG), NULL},
   //{"MX100_XLL_GG", {0x01,0x13,0x00,0x10}, gsl_config_data_id_100_XLL_GG, GSL3676_FW_100_XLL_GG,  ARRAY_SIZE(GSL3676_FW_100_XLL_GG), NULL},
   {"MX100_XLL_PG",  {0x00,0x13,0x04,0x10}, gsl_config_data_id_100_XLL_PG,GSL3676_FW_100_XLL_PG,  ARRAY_SIZE(GSL3676_FW_100_XLL_PG), NULL},
   {"MX100_DZ_OGS",  {0x00,0x13,0x00,0x30}, gsl_config_data_id_100_DZ_OGS,GSLX680_FW_100_DZ_OGS,  ARRAY_SIZE(GSLX680_FW_100_DZ_OGS), NULL},
   {"MX100_XLL_2_GG", {0x01,0x13,0x00,0x10}, gsl_config_data_id_H100_1_XLL_PG,GSL3676_FW_H100_1_XLL_PG,  ARRAY_SIZE(GSL3676_FW_H100_1_XLL_PG), NULL},
   {"MX100_DEFAULT", {0xff,0xff,0xff,0xff}, gsl_config_data_id_100_YTG_GG, GSL3676_FW_100_YTG_GG,  ARRAY_SIZE(GSL3676_FW_100_YTG_GG), NULL},
};

//W100 
static const ELINK_SILEAD_CONFIG W100_tp_data[] = {
   {"W100_YTG_GG", {0x00,0x93,0x00,0x10}, gsl_config_data_id_W100_YTG_GG, GSL3676_FW_W100_YTG_GG,  ARRAY_SIZE(GSL3676_FW_W100_YTG_GG), NULL},
   {"W100_DEFAULT", {0xff,0xff,0xff,0xff}, gsl_config_data_id_W100_YTG_GG, GSL3676_FW_W100_YTG_GG,  ARRAY_SIZE(GSL3676_FW_W100_YTG_GG), NULL},
};

//101
static const ELINK_SILEAD_CONFIG mx101_tp_data[] = {
   {"MX101_RSS_GG", {0x00,0x17,0x00,0x10}, gsl_config_data_id_101_RSS_GG, GSL3676_FW_101_RSS_GG,  ARRAY_SIZE(GSL3676_FW_101_RSS_GG), NULL},
//   {"MX101_WTGD_GG", {0x00,0x33,0x00,0x10}, gsl_config_data_id_101_WTGD_GG, GSL3676_FW_101_WTGD_GG,  ARRAY_SIZE(GSL3676_FW_101_WTGD_GG), NULL},
   {"MX101_WJ_PG", {0x01,0x13,0x00,0x10}, gsl_config_data_id_101_WJ_PG, GSL3676_FW_101_WJ_PG,  ARRAY_SIZE(GSL3676_FW_101_WJ_PG), NULL},
   {"MX101_RS_GG", {0x02,0x13,0x00,0x10}, gsl_config_data_id_101_RS_GG, GSL3676_FW_101_RS_GG,  ARRAY_SIZE(GSL3676_FW_101_RS_GG), NULL},
//   {"MX101_FC_PG", {0x00,0x33,0x00,0x10}, gsl_config_data_id_101_FC_PG, GSL3676_FW_101_FC_PG,  ARRAY_SIZE(GSL3676_FW_101_FC_PG), NULL},
//   {"MX101_HT_PG", {0x00,0x93,0x00,0x10}, gsl_config_data_id_101_HT_PG, GSL3676_FW_101_HT_PG,  ARRAY_SIZE(GSL3676_FW_101_HT_PG), NULL},
   {"MX101_DH_PG", {0x00,0x53,0x00,0x10}, gsl_config_data_id_102_DH_PG, GSL3676_FW_102_DH_PG,  ARRAY_SIZE(GSL3676_FW_102_DH_PG), NULL},
   {"MX101_ZHZY_PG", {0x00,0x33,0x00,0x10}, gsl_config_data_id_101_ZHZY_GG, GSL3676_FW_101_ZHZY_GG,  ARRAY_SIZE(GSL3676_FW_101_ZHZY_GG), NULL},
   {"MX101_YTG_PG", {0x00,0x93,0x00,0x10}, gsl_config_data_id_101_YTG_GG, GSL3676_FW_101_YTG_GG,  ARRAY_SIZE(GSL3676_FW_101_YTG_GG), NULL},
   {"MX101_YTG_2_PG", {0x00,0x13,0x40,0x10}, gsl_config_data_id_101_YTG_2_GG, GSL3676_FW_101_YTG_2_GG,  ARRAY_SIZE(GSL3676_FW_101_YTG_2_GG), NULL},//add by heliang 20160721
   {"MX101_RP_GG", {0x00,0x13,0x00,0x10}, gsl_config_data_id_101_RP_GG, GSL3676_FW_101_RP_GG,  ARRAY_SIZE(GSL3676_FW_101_RP_GG), NULL},
   {"L101_HT_PG_ARCHOS", {0x00,0x13,0x00,0x18}, gsl_config_data_id_L101_HT_PG_ARCHOS, GSL3676_FW_L101_HT_PG_ARCHOS,  ARRAY_SIZE(GSL3676_FW_L101_HT_PG_ARCHOS), NULL},//ELink_Matt@20170607 add 
   {"MX101_DEFAULT", {0xff,0xff,0xff,0xff}, gsl_config_data_id_101_RSS_GG, GSL3676_FW_101_RSS_GG,  ARRAY_SIZE(GSL3676_FW_101_RSS_GG), NULL},
};

//101 _1
static const ELINK_SILEAD_CONFIG mx101_tp_data_1[] = {
   {"MX101_FC_PG_1", {0x00,0x33,0x00,0x10}, gsl_config_data_id_101_FC_PG, GSL3676_FW_101_FC_PG,  ARRAY_SIZE(GSL3676_FW_101_FC_PG), NULL},
   {"MX101_HT_PG_1", {0x00,0x93,0x00,0x10}, gsl_config_data_id_101_HT_PG, GSL3676_FW_101_HT_PG,  ARRAY_SIZE(GSL3676_FW_101_HT_PG), NULL},
   {"MX101_DEFAULT_1", {0xff,0xff,0xff,0xff}, gsl_config_data_id_101_FC_PG, GSL3676_FW_101_FC_PG,  ARRAY_SIZE(GSL3676_FW_101_FC_PG), NULL},

};
//102
static const ELINK_SILEAD_CONFIG mx102_tp_data[] = {
   {"MX102_DH_PG", {0x00,0x53,0x00,0x10}, gsl_config_data_id_102_DH_PG, GSL3676_FW_102_DH_PG,  ARRAY_SIZE(GSL3676_FW_102_DH_PG), NULL},
   {"MX102_ZHZY_PG", {0x00,0x13,0x00,0x10}, gsl_config_data_id_102_ZHZY_PG, GSL3676_FW_102_ZHZY_PG,  ARRAY_SIZE(GSL3676_FW_102_ZHZY_PG), NULL},
   {"MX102_YTG_GG", {0x00,0x93,0x00,0x10}, gsl_config_data_id_102_YTG_GG, GSL3676_FW_102_YTG_GG,  ARRAY_SIZE(GSL3676_FW_102_YTG_GG), NULL},
//   {"MX102_FC_PG", {0x00,0x13,0x00,0x10}, gsl_config_data_id_102_FC_PG, GSL3676_FW_102_FC_PG,  ARRAY_SIZE(GSL3676_FW_102_FC_PG), NULL},
   {"MX102_DEFAULT", {0xff,0xff,0xff,0xff}, gsl_config_data_id_102_ZHZY_PG, GSL3676_FW_102_ZHZY_PG,  ARRAY_SIZE(GSL3676_FW_102_ZHZY_PG), NULL},
};

//103
static const ELINK_SILEAD_CONFIG mx103_tp_data[] = {
   {"MX103_HKGD_PG", {0x00,0x17,0x00,0x10}, gsl_config_data_id_103_HKGD_PG, GSL3676_FW_103_HKGD_PG,  ARRAY_SIZE(GSL3676_FW_103_HKGD_PG), NULL},
   {"MX103_DH_PG", {0x00,0x33,0x00,0x10}, gsl_config_data_id_103_DH_PG, GSL3676_FW_103_DH_PG,  ARRAY_SIZE(GSL3676_FW_103_DH_PG), NULL},
   {"MX103_WTGD_PG", {0x00,0x1b,0x00,0x10}, gsl_config_data_id_103_WTGD_PG, GSL3676_FW_103_WTGD_PG,  ARRAY_SIZE(GSL3676_FW_103_WTGD_PG), NULL},
   {"MX103_WJ_PG", {0x00,0x13,0x20,0x10}, gsl_config_data_id_103_WJ_PG, GSL3676_FW_103_WJ_PG,  ARRAY_SIZE(GSL3676_FW_103_WJ_PG), NULL},
   {"MX103_HK_PG", {0x01,0x13,0x00,0x10}, gsl_config_data_id_103_HK_PG, GSL3676_FW_103_HK_PG,  ARRAY_SIZE(GSL3676_FW_103_HK_PG), NULL}, 
   {"MX103_DEFAULT", {0xff,0xff,0xff,0xff}, gsl_config_data_id_103_HKGD_PG, GSL3676_FW_103_HKGD_PG,  ARRAY_SIZE(GSL3676_FW_103_HKGD_PG), NULL},
};
//108
// static const ELINK_SILEAD_CONFIG mx108_tp_data_gg_ytg[] = {
   // {"MX108_TP_YTG_GG", {0xff,0xff,0xff,0xff}, gsl_config_data_id_108_YTG_GG, GSL3676_FW_108_YTG_GG, ARRAY_SIZE(GSL3676_FW_108_YTG_GG), tpd_keys_dim_local_108_ytg_gg},
// };
//109
static const ELINK_SILEAD_CONFIG mx109_tp_data_ht_pg[] = {
   {"MX109_TP_HT_PG", {0xff,0xff,0xff,0xff}, gsl_config_data_id_109_HT_PG, GSL3676_FW_109_HT_PG,  ARRAY_SIZE(GSL3676_FW_109_HT_PG), NULL},
};

//108J
static const ELINK_SILEAD_CONFIG mx108j_tp_data[] = {
   {"MX108J_FC_PG", {0x00,0x13,0x00,0x11}, gsl_config_data_id_E108J_FC_PG, GSLX680_FW_E108J_FC_PG,  ARRAY_SIZE(GSLX680_FW_E108J_FC_PG), NULL},
   {"MX108J_HK_PG", {0x00,0x17,0x00,0x10}, gsl_config_data_id_L108J_HK_PG, GSLX680_FW_L108J_HK_PG,  ARRAY_SIZE(GSLX680_FW_L108J_HK_PG), NULL},
   {"MX108J_DEFAULT", {0xff,0xff,0xff,0xff}, gsl_config_data_id_E108J_FC_PG, GSLX680_FW_E108J_FC_PG,  ARRAY_SIZE(GSLX680_FW_E108J_FC_PG), NULL},
};
//806J
static const ELINK_SILEAD_CONFIG mx806j_tp_data[] = {
   {"MX806J_FC_PG", {0x00,0x13,0x00,0x11}, gsl_config_data_id_L806J_FC_PG, GSLX680_FW_L806J_FC_PG,  ARRAY_SIZE(GSLX680_FW_L806J_FC_PG), NULL},
   {"MX806J_HK_PG", {0x01,0xff,0x00,0x10}, gsl_config_data_id_L806J_HK_PG, GSLX680_FW_L806J_HK_PG,  ARRAY_SIZE(GSLX680_FW_L806J_HK_PG), NULL},
   {"MX806J_DEFAULT", {0xff,0xff,0xff,0xff}, gsl_config_data_id_L806J_FC_PG, GSLX680_FW_L806J_FC_PG,  ARRAY_SIZE(GSLX680_FW_L806J_FC_PG), NULL},
};

//863J
static const ELINK_SILEAD_CONFIG mx863j_tp_data[] = {
   {"MX863J_XC_PG", {0x00,0x13,0xc0,0x10}, gsl_config_data_id_L863J_XC_PG, GSL3676_FW_L863J_XC_PG,  ARRAY_SIZE(GSL3676_FW_L863J_XC_PG), NULL},
   {"MX863J_DEFAULT", {0xff,0xff,0xff,0xff}, gsl_config_data_id_L863J_XC_PG, GSL3676_FW_L863J_XC_PG,  ARRAY_SIZE(GSL3676_FW_L863J_XC_PG), NULL},
};

//L863J  7'
static const ELINK_SILEAD_CONFIG l863j_tp_data_7[] = {
   {"L863J_DZ_OGS_7", {0xff,0x80,0xf8,0x07}, gsl_config_data_id_GSL1688E_DZ_OGS_68, GSLX680_FW_GSL1688E_DZ_OGS_68,  ARRAY_SIZE(GSLX680_FW_GSL1688E_DZ_OGS_68), NULL},
   {"L863J_DZ_OGS_7_69", {0xff,0xc0,0x78,0x07}, gsl_config_data_id_GSL1688E_DZ_OGS_69, GSLX680_FW_GSL1688E_DZ_OGS_69,  ARRAY_SIZE(GSLX680_FW_GSL1688E_DZ_OGS_69), NULL}, //by pzp
   {"L863J_XC_PG_7", {0xff,0x80,0x78,0x0f}, gsl_config_data_id_GSL1688E_XC_PG_7, GSLX680_FW_GSL1688E_XC_PG_7,  ARRAY_SIZE(GSLX680_FW_GSL1688E_XC_PG_7), NULL},
   {"L863J_SQ_PG_7", {0xff,0xa0,0x78,0x07}, gsl_config_data_id_GSL1686_PG_SQ_7_600, GSLX680_FW_GSL1686_PG_SQ_7_600,  ARRAY_SIZE(GSLX680_FW_GSL1686_PG_SQ_7_600), NULL},
};

//L863J  8'
static const ELINK_SILEAD_CONFIG l863j_tp_data_8[] = {
   {"L863J_DZ_OGS_80B21", {0x80,0x13,0x00,0x10}, gsl_config_data_id_DZ_OGS_80B21, GSLX680_FW_DZ_OGS_80B21,  ARRAY_SIZE(GSLX680_FW_DZ_OGS_80B21), NULL},
   {"L863J_DZ_OGS_80B22", {0x00,0x13,0x80,0x10}, gsl_config_data_id_DZ_OGS_80B22, GSLX680_FW_DZ_OGS_80B22,  ARRAY_SIZE(GSLX680_FW_DZ_OGS_80B22), NULL},
   {"L863J_DZ_OGS_80B23", {0x00,0x13,0x00,0x10}, gsl_config_data_id_DZ_OGS_80B23, GSLX680_FW_DZ_OGS_80B23,  ARRAY_SIZE(GSLX680_FW_DZ_OGS_80B23), NULL},
   {"L863J_XC_PG_8", {0x00,0x13,0xc0,0x10}, gsl_config_data_id_GSL3676_XC_PG_8, GSLX680_FW_GSL3676_XC_PG_8,  ARRAY_SIZE(GSLX680_FW_GSL3676_XC_PG_8), NULL},
   {"L863J_XHS_PG_8", {0x08,0x13,0x00,0x10}, gsl_config_data_id_H863_XHS_PG_8, GSL3676_FW_H863_XHS_PG_8,  ARRAY_SIZE(GSL3676_FW_H863_XHS_PG_8), NULL}, //by pzp
   {"L863J_ZYD_PG_8", {0x40,0x13,0x00,0x10}, gsl_config_data_id_E863_ZYD_PG_8, GSL3676_FW_E863_ZYD_PG_8,  ARRAY_SIZE(GSL3676_FW_E863_ZYD_PG_8), NULL},
};

//L863J  10'
static const ELINK_SILEAD_CONFIG l863j_tp_data_10[] = {
   {"L863J_DZ_OGS_10", {0x80,0x13,0x00,0x10}, gsl_config_data_id_GSL3676_DZ_OGS_10_1, GSLX680_FW_GSL3676_DZ_OGS_10_1,  ARRAY_SIZE(GSLX680_FW_GSL3676_DZ_OGS_10_1), NULL},
   {"L863J_XC_PG_10_1200", {0x40,0x13,0x00,0x10}, gsl_config_data_id_GSL3676_XC_PG_1200, GSLX680_FW_GSL3676_XC_PG_1200,  ARRAY_SIZE(GSLX680_FW_GSL3676_XC_PG_1200), NULL},
   {"L863J_XHS_PG_10", {0x20,0x13,0x00,0x10}, gsl_config_data_id_H863_XHS_PG_10, GSL3676_FW_H863_XHS_PG_10,  ARRAY_SIZE(GSL3676_FW_H863_XHS_PG_10), NULL}, //by pzp
   {"E863J_SQ_GG_10",  {0x00,0x33,0x00,0x10}, gsl_config_data_id_GSL3676_E863G_SQ_GG_10, GSLX680_FW_GSL3676_E863G_SQ_GG_10,  ARRAY_SIZE(GSLX680_FW_GSL3676_E863G_SQ_GG_10), NULL}, //by heliang 20180409
   {"W100_HK_PG_10_1280",{0x00,0x13,0x00,0x12}, gsl_config_data_id_W100_HK_PG_10_1280, GSLX680_FW_W100_HK_PG_10_1280,  ARRAY_SIZE(GSLX680_FW_W100_HK_PG_10_1280), NULL}, //by heliang 20180810
};

//L863J  10_1'
static const ELINK_SILEAD_CONFIG l863j_tp_data_10_1[] = {
   {"L863J_DZ_OGS_10_1",   {0x80,0x13,0x00,0x10}, gsl_config_data_id_DZ_OGS_10A79, GSLX680_FW_DZ_OGS_10A79,  ARRAY_SIZE(GSLX680_FW_DZ_OGS_10A79), NULL},
   {"L863J_XC_PG_10_1920", {0x40,0x13,0x00,0x10}, gsl_config_data_id_GSL3676_XC_PG_1920, GSLX680_FW_GSL3676_XC_PG_1920,  ARRAY_SIZE(GSLX680_FW_GSL3676_XC_PG_1920), NULL},
   {"W100_YTG_GG_10_1920", {0x00,0x93,0x00,0x10}, gsl_config_data_id_100_YTG_GG_1920, GSL3676_FW_100_YTG_GG_1920,  ARRAY_SIZE(GSL3676_FW_100_YTG_GG_1920), NULL},
   {"L863J_HK_PG_10_1280", {0x20,0x13,0x00,0x10}, gsl_config_data_id_GSL3676_HK_PG_10_1280, GSLX680_FW_GSL3676_HK_PG_10_1280,  ARRAY_SIZE(GSLX680_FW_GSL3676_HK_PG_10_1280), NULL},
};

//L863J  10_2'
static const ELINK_SILEAD_CONFIG l863j_tp_data_10_2[] = {
   {"L863J_XC_PG_10_1280", {0x40,0x13,0x00,0x10}, gsl_config_data_id_GSL3676_XC_PG_1280, GSLX680_FW_GSL3676_XC_PG_1280,  ARRAY_SIZE(GSLX680_FW_GSL3676_XC_PG_1280), NULL}, //by pzp
};

//L7U4J  10'
static const ELINK_SILEAD_CONFIG l7u4j_10[] = {
  {"L7U4J_ZYD_GG_10.1",  {0x00,0x93,0x00,0x10}, gsl_config_data_id_L7U4J_ZYD_GG_10_1,  GSL3676_FW_L7U4J_ZYD_GG_10_1,   ARRAY_SIZE(GSL3676_FW_L7U4J_ZYD_GG_10_1), NULL},  //by pzp
};

//L7U4J  8'
static const ELINK_SILEAD_CONFIG l7u4j_8[] = {
  {"L7U4J_ZYD_GG_8",  {0x00,0x93,0x00,0x10}, gsl_config_data_id_L7U4J_ZYD_GG_8,  GSL3676_FW_L7U4J_ZYD_GG_8,   ARRAY_SIZE(GSL3676_FW_L7U4J_ZYD_GG_8), NULL},  //by pzp
};

//L7U4J  7'
static const ELINK_SILEAD_CONFIG l7u4j_7[] = {
   {"L7U4J_CY_GG_7", {0xff,0x84,0x78,0x07}, gsl_config_data_id_L7U4J_CY_GG_7, GSL1688_FW_L7U4J_CY_GG_7,  ARRAY_SIZE(GSL1688_FW_L7U4J_CY_GG_7), NULL},
};

//L7C7WA   7'
static const ELINK_SILEAD_CONFIG l7c7wa_7[] = {
   {"L7C7WA_ZYD_PG_7", {0xff,0x84,0x78,0x07}, gsl_config_data_id_L7C7WA_ZYD_PG_600, GSL1688_FW_L7C7WA_ZYD_PG_600,  ARRAY_SIZE(GSL1688_FW_L7C7WA_ZYD_PG_600), NULL},
};

//L706_7   7'
static const ELINK_SILEAD_CONFIG L706_7[] = {
   {"L706_PG_DP_7", {0xff,0x80,0x78,0x0f}, gsl_config_data_id_706_PG_DP, GSL1686_FW_706_PG_DP,  ARRAY_SIZE(GSL1686_FW_706_PG_DP), NULL},
};

// 863 10inch
// static const ELINK_SILEAD_CONFIG tp_863_10[] = {
   // {"863_DZ_OGS_10", {0x80,0x13,0x00,0x10}, gsl_config_data_id_863_GSL3676_DZ_OGS_10, GSLX680_FW_863_GSL3676_DZ_OGS_10, ARRAY_SIZE(GSLX680_FW_863_GSL3676_DZ_OGS_10), NULL},
// };

//--------------------------------------------

//t8s1 
static const ELINK_SILEAD_CONFIG tp_t8s1[] = {
   {"T8S1GF_DZ_OGS_10_1", {0x00,0x00,0x00,0x01}, gsl_config_data_id_FWT8S1GF_DZ_OGS_10_1, GSLX680_FWT8S1GF_DZ_OGS_10_1, ARRAY_SIZE(GSLX680_FWT8S1GF_DZ_OGS_10_1), NULL},
   {"T8S1GF_DZ_OGS_8", {0xf8,0x00,0x00,0x00}, gsl_config_data_id_T8S1GF_DZ_OGS_8, GSLX680_FW_T8S1GF_DZ_OGS_8, ARRAY_SIZE(GSLX680_FW_T8S1GF_DZ_OGS_8), NULL},
   {"T8S1GF_DEFAULT", {0xff,0xff,0xff,0xff}, gsl_config_data_id_FWT8S1GF_DZ_OGS_10_1, GSLX680_FWT8S1GF_DZ_OGS_10_1, ARRAY_SIZE(GSLX680_FWT8S1GF_DZ_OGS_10_1), NULL},
};

//t6p2
static const ELINK_SILEAD_CONFIG t6p2[] = {
   {"T8S1GF_DZ_OGS_10_1", {0x00,0x00,0x00,0x01}, gsl_config_data_id_T6P2_PB_GG, GSL915_FW_T6P2_PB_GG, ARRAY_SIZE(GSL915_FW_T6P2_PB_GG), NULL},
};

static const ELINK_TP_INFO elink_tp_list[] =
{
	   //590
   // {"MX590_TP",mx590_tp_data,ARRAY_SIZE(mx590_tp_data)},
   //601
   //608
   {"MX608_TP_DZ_OGS",mx608_tp_data_dz_ogs,ARRAY_SIZE(mx608_tp_data_dz_ogs)},   
   {"MX608_TP_QHD_DZ_OGS",mx608_tp_data_qhd_dz_ogs,ARRAY_SIZE(mx608_tp_data_qhd_dz_ogs)},
   //695
   {"MX695_TP",mx695_tp_data,ARRAY_SIZE(mx695_tp_data)},   
   //698
   {"MX698_TP",mx698_tp_data,ARRAY_SIZE(mx698_tp_data)},   
   {"MX698_TP_WSVGA",mx698_tp_data_wsvga,ARRAY_SIZE(mx698_tp_data_wsvga)},   
   //706
   {"MX706_TP",mx706_tp_data,ARRAY_SIZE(mx706_tp_data)},
   // {"MX706_TP_CTD_PG_HAVE_KEY",mx706_tp_data_ctd_pg_have_key,ARRAY_SIZE(mx706_tp_data_ctd_pg_have_key)},
   // {"MX706_TP_FC_PG_HAVE_KEY",mx706_tp_data_fc_pg_have_key,ARRAY_SIZE(mx706_tp_data_fc_pg_have_key)},
   // {"MX706_TP_HT_PG_HAVE_KEY",mx706_tp_data_ht_pg_have_key,ARRAY_SIZE(mx706_tp_data_ht_pg_have_key)},
   //8T1
   // {"MX8T1_TP_DZ_OGS_HAVE_KEY",mx8T1_tp_data_dz_ogs_have_key,ARRAY_SIZE(mx8T1_tp_data_dz_ogs_have_key)},
   //7Z3
   {"MX7Z3_TP",mx7Z3_tp_data,ARRAY_SIZE(mx7Z3_tp_data)},
   {"MX7Z3_C2_TP",mx7Z3_600x1024_tp_data,ARRAY_SIZE(mx7Z3_600x1024_tp_data)},
   //710
   {"MX710_TP",mx710_tp_data,ARRAY_SIZE(mx710_tp_data)},
   //718
   {"MX718_TP",mx718_tp_data, ARRAY_SIZE(mx718_tp_data)},
   //785
   // {"MX785_TP_YTG_GG",mx785_tp_data_ytg_gg,ARRAY_SIZE(mx785_tp_data_ytg_gg)},
   //798
   // {"MX798_TP",mx798_tp_data, ARRAY_SIZE(mx798_tp_data)},
   //801
   {"MX801_TP",mx801_tp_data,ARRAY_SIZE(mx801_tp_data)},
   //805
   {"MX805_TP",mx805_tp_data,ARRAY_SIZE(mx805_tp_data)},
   //806
   {"MX806_TP",mx806_tp_data,ARRAY_SIZE(mx806_tp_data)},
   //808
   {"MX808_TP",mx808_tp_data,ARRAY_SIZE(mx808_tp_data)},
   //888
   {"MX888_TP",mx888_tp_data,ARRAY_SIZE(mx888_tp_data)},
   //8T1
   {"MX8T1_TP",mx8T1_tp_data,ARRAY_SIZE(mx8T1_tp_data)},
   //100
   {"MX100_TP",mx100_tp_data,ARRAY_SIZE(mx100_tp_data)},
   //W100
   {"W100_TP",W100_tp_data,ARRAY_SIZE(W100_tp_data)},   
   //101
   {"MX101_TP",mx101_tp_data,ARRAY_SIZE(mx101_tp_data)},
   {"MX101_TP_1",mx101_tp_data_1,ARRAY_SIZE(mx101_tp_data_1)},
   //102
   {"MX102_TP",mx102_tp_data,ARRAY_SIZE(mx102_tp_data)},
   //103
   {"MX103_TP",mx103_tp_data,ARRAY_SIZE(mx103_tp_data)},
   //108
   // {"MX108_TP_YTG_GG",mx108_tp_data_gg_ytg,ARRAY_SIZE(mx108_tp_data_gg_ytg)},
   //109
   {"MX109_TP_HT_PG",mx109_tp_data_ht_pg,ARRAY_SIZE(mx109_tp_data_ht_pg)},

   //108J
   {"MX108J_TP",mx108j_tp_data,ARRAY_SIZE(mx108j_tp_data)},
   //806J
   {"MX806J_TP",mx806j_tp_data,ARRAY_SIZE(mx806j_tp_data)},
   //863J
   {"MX863J_TP",mx863j_tp_data,ARRAY_SIZE(mx863j_tp_data)},

   //L863J
   {"L863J_TP_7", l863j_tp_data_7,ARRAY_SIZE(l863j_tp_data_7)},
   {"L863J_TP_8",l863j_tp_data_8,ARRAY_SIZE(l863j_tp_data_8)},
   {"L863J_TP_10", l863j_tp_data_10,ARRAY_SIZE(l863j_tp_data_10)},
   {"L863J_TP_10_1", l863j_tp_data_10_1,ARRAY_SIZE(l863j_tp_data_10_1)},
   {"L863J_TP_10_2", l863j_tp_data_10_2,ARRAY_SIZE(l863j_tp_data_10_2)},
   
   //L7U4J
   {"L7U4J_10",l7u4j_10,ARRAY_SIZE(l7u4j_10)},    //by pzp
   {"L7U4J_8",l7u4j_8,ARRAY_SIZE(l7u4j_8)},    //by pzp
   {"L7U4J_7",l7u4j_7,ARRAY_SIZE(l7u4j_7)},
	
	//L7C7WA   7'
   {"L7C7WA_7",l7c7wa_7,ARRAY_SIZE(l7c7wa_7)},
 
 	//L706   7'
   {"L706_7",L706_7,ARRAY_SIZE(L706_7)},
 
	//863 10inch
	// {"863_TP_10", tp_863_10, ARRAY_SIZE(tp_863_10)},
	//t8s1
	{"T8S1", tp_t8s1, ARRAY_SIZE(tp_t8s1)},
	//t6p2
	{"T6P2", t6p2, ARRAY_SIZE(t6p2)},
};

#endif
