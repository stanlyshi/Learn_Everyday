/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:34 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:46 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:29 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:04 CST. */
/************************************************************************
 *Copyright (c) 2018 Sylincom.
 *All rights reserved.
 *
 *File name: rrus_mgmt_mac_api.h
 *Description: the all api of rrus_mgmt with mac.
 *
 *Current Version: 0.l
 *Author: 
 *Date: 2018.06.05
 *
 ************************************************************************/


//RRU绠＄悊妯″潡涓嶤PU_MAC妯″潡浜や簰鎺ュ彛
#ifndef MAC_RRUM_H
#define MAC_RRUM_H

#pragma pack(1)

/* Message Code of RRUS_MGMT with MAC */
#define CODE_RRUS_MGMT_MAC_MIN			200
#define CODE_IMR_CELL_BUILD_CFG_REQ    	201
#define CODE_IMR_CELL_BUILD_CFG_RSP     202
#define CODE_IMR_CELL_DELETE_REQ        203
#define CODE_IMR_CELL_DELETE_RSP        204

typedef struct{
	UINT16 MsgType;
	UINT16 MsgCode;
	UINT32 MsgLen;
	UINT32 cell_id;
}OmMacHead;

//灏忓尯寤虹珛閰嶇疆璇锋眰	鍗忚鏍堣皟鐢ㄥ畬鎴愬灏忓尯RRU鐨勯厤缃?
typedef struct 
{
	UINT8			cell_cdg_cmd;	//灏忓尯閰嶇疆鏍囪瘑	0锛氬缓绔?1锛氶噸閰?
    UINT32          lc_id;			//鏈湴灏忓尯鏍囪瘑
	UINT16			single_carrier_cell_power;//鍗曡浇娉㈠皬鍖哄姛鐜?鍗曚綅锛?/256 dbm
	UINT8			ant_set_no;		//澶╃嚎缁勫彿 1-8锛堝～1锛?
	UINT8			freq_ie_num;	//棰戠偣鏁帮紙濉?锛夋寚褰撳墠娑堟伅涓寘鍚殑棰戠偣閰嶇疆IE鐨勬暟鐩?
}IMR_CELL_BUILD_CFG_CELL;


typedef struct 
{
	UINT8			freq_cdg_cmd;	//棰戠偣閰嶇疆鏍囪瘑	0锛氬缓绔?1锛氬垹闄?

    UINT32          lc_id;			//鏈湴灏忓尯鏍囪瘑
    UINT8			carrier_no;		//杞芥尝鍙凤紙濉?锛変粠1寮€濮?
    UINT32			mid_freq;		//涓績棰戠巼	鍗曚綅锛?00kHz
    UINT32			freq_attribute;	//棰戠偣鐨勪富杈呯壒鎬?0锛氫富棰戠偣 榛樿	1锛氳緟棰戠偣
    UINT8			tsinfo;			//鏃堕殭杞崲鐐?-6锛?锛?
    UINT8			special_subframe_cfg;//鐗规畩鏃堕殭閰嶇疆0-8(娉?锛?
    UINT32			carrier_width;	//杞芥尝甯﹀锛氬€间负锛?, 10, 20
    UINT8			circ_len_ch;	//寰幆鍓嶇紑闀垮害閫夋嫨 甯歌CP-0 鎵╁睍CP-1
}IMR_CELL_BUILD_CFG_FREQ;

typedef struct
{
	OmMacHead					head;
	IMR_CELL_BUILD_CFG_CELL		cell_build_cell;
	IMR_CELL_BUILD_CFG_FREQ		cell_build_freq;
}IMR_CELL_BUILD_CFG;
extern INT32 handle_imr_cell_build_cfg_req(void*, UINT32);

//灏忓尯寤虹珛閰嶇疆鍝嶅簲	杩斿洖灏忓尯閰嶇疆鍝嶅簲娑堟伅
typedef struct 
{
	UINT32			lc_id;			//鏈湴灏忓尯鏍囪瘑
	UINT32			result;			//杩斿洖缁撴灉	0锛氭垚鍔燂紙鍖呭惈涓昏浇棰戯級	1锛氬け璐?
}IMR_CELL_BUILD_RSP_CELL;

typedef struct 
{
	UINT8			freq_cdg_cmd;	//棰戠偣閰嶇疆鏍囪瘑	0锛氬缓绔?1锛氬垹闄?
    UINT32          lc_id;			//鏈湴灏忓尯鏍囪瘑
    UINT8			carrier_no;		//杞芥尝鍙?
    UINT32			result;			//杩斿洖缁撴灉	0锛氭垚鍔燂紙鍖呭惈涓昏浇棰戯級1锛氬け璐?
}IMR_CELL_BUILD_RSP_FREQ;

//灏忓尯鍒犻櫎璇锋眰	鍗忚鏍堣皟鐢ㄥ畬鎴愬灏忓尯鐨勫垹闄?
typedef struct 
{
	OmMacHead					head;
    UINT32          lc_id;			//本地小区标识
	UINT16			single_carrier_cell_power;//单载波小区功�?单位�?/256 dbm
	UINT8			ant_set_no;		//天线组号 1-8（填1�?
	UINT8			freq_ie_num;	//频点数（�?�?指当前消息中包含的频点配置IE的数�?
}IMR_CELL_DELETE_REQ;

extern INT32 handle_imr_cell_delete_req(void*, UINT32);

//灏忓尯鍒犻櫎鍝嶅簲	杩斿洖灏忓尯鍒犻櫎鍝嶅簲娑堟伅
typedef struct 
{
	UINT32			lc_id;			//鏈湴灏忓尯鏍囪瘑
	UINT32			result;			//杩斿洖缁撴灉	0锛氭垚鍔燂紙鍖呭惈涓昏浇棰戯級	1锛氬け璐?
}IMR_CELL_DELETE_RSP;


#pragma pack()

#endif