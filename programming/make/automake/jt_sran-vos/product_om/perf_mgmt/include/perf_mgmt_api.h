/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:29 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 19日 星期二 14:31:30 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 19日 星期二 14:31:24 CST. 
*/
/***********************************************************************************************************************
** 
** Copyright (c) 2008-2019 ICT/CAS.
** All rights reserved.
**
** File name: perf_mgmt_api.h
** Description:  performance statistics module's api
**
** Current Version: 
** $Revision$
** Author: Rong Tao
** Date: 2019.02 - 
**
***********************************************************************************************************************/
#ifndef _PERF_MGMT_API_H
#define _PERF_MGMT_API_H

/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "perfs_types.h"
#include "perfs_l1l2l3_perf.h"

/* Constants ---------------------------------------------------------------------------------------------------------*/
/* Types -------------------------------------------------------------------------------------------------------------*/
typedef struct {
    UINT32 cell_id;
    perf_mac_t mac;
    union {
    perf_realphy_t realphy;
    perf_simuphy_t simuphy;
    };
}__attribute__((packed)) cell_perf_t;

/* Macros ------------------------------------------------------------------------------------------------------------*/
/* Globals -----------------------------------------------------------------------------------------------------------*/
/* Functions ---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
extern INT32 perf_mgmt_module_init();

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
extern INT32 perf_mgmt_clean_up();

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 *
 * Example:
```c
#include "prfm_mgmt_api.h"
...
CELL_PERF cell_perf;
get_cell_performance(cell_id, &cell_perf);
...

```
 **********************************************************************************************************************/
extern INT32 get_cell_perf(UINT32 cell_id, cell_perf_t *cell_perf);



#endif /*<_PERF_MGMT_API_H>*/

