/*
 * shared_memory.h
 *
 *  Created on: 19.01.2022
 *      Author: Ha
 */

#ifndef SRC_SHARED_MEMORY_H_
#define SRC_SHARED_MEMORY_H_

#include "platform.h"
#include "custom_definitions.h"


//#define COUNT			(*(volatile unsigned long *)(SHARED_OCM_BASE_ADDRESS))    // 4 byte
//#define ARRAY			((volatile unsigned char *)(SHARED_OCM_BASE_ADDRESS + 4)) // 1 byte

//memory parts of 10 module, each owns 200 bytes for request and response packets, which should enough for all module when they drives fully functions
#define MEM_REQ_1F			((u8 *)(SHARED_OCM_BASE_ADDRESS))       // 0-99
#define MEM_RES_1F			((u8 *)(SHARED_OCM_BASE_ADDRESS + 100)) // 100-199
#define MEM_REQ_1E			((u8 *)(SHARED_OCM_BASE_ADDRESS + 200)) // 200-299
#define MEM_RES_1E			((u8 *)(SHARED_OCM_BASE_ADDRESS + 300)) // 300-399
#define MEM_REQ_1D			((u8 *)(SHARED_OCM_BASE_ADDRESS + 400)) // 400-499
#define MEM_RES_1D			((u8 *)(SHARED_OCM_BASE_ADDRESS + 500)) // 500-599
#define MEM_REQ_1C			((u8 *)(SHARED_OCM_BASE_ADDRESS + 600)) // 600-699
#define MEM_RES_1C			((u8 *)(SHARED_OCM_BASE_ADDRESS + 700)) // 700-799
#define MEM_REQ_1B			((u8 *)(SHARED_OCM_BASE_ADDRESS + 800)) // 800-899
#define MEM_RES_1B			((u8 *)(SHARED_OCM_BASE_ADDRESS + 900)) // 900-999
#define MEM_REQ_20			((u8 *)(SHARED_OCM_BASE_ADDRESS + 1000)) // 1000-1099
#define MEM_RES_20			((u8 *)(SHARED_OCM_BASE_ADDRESS + 1100)) // 1100-1199
#define MEM_REQ_21			((u8 *)(SHARED_OCM_BASE_ADDRESS + 1200)) // 1200-1299
#define MEM_RES_21			((u8 *)(SHARED_OCM_BASE_ADDRESS + 1300)) // 1300-1399
#define MEM_REQ_22			((u8 *)(SHARED_OCM_BASE_ADDRESS + 1400)) // 1400-1499
#define MEM_RES_22			((u8 *)(SHARED_OCM_BASE_ADDRESS + 1500)) // 1500-1599
#define MEM_REQ_23			((u8 *)(SHARED_OCM_BASE_ADDRESS + 1600)) // 1600-1699
#define MEM_RES_23			((u8 *)(SHARED_OCM_BASE_ADDRESS + 1700)) // 1700-1799
#define MEM_REQ_24			((u8 *)(SHARED_OCM_BASE_ADDRESS + 1800)) // 1800-1899
#define MEM_RES_24			((u8 *)(SHARED_OCM_BASE_ADDRESS + 1900)) // 1900-1999



//memory parts of device information for 10 module, each reserves 100 bytes
#define MEM_DEVICE_INFOR	((u8 *)(SHARED_OCM_BASE_ADDRESS + 2000))       // 2000-2999
//memory parts of device number, one byte only
#define MEM_DEVICE_NUM  	((int *)(SHARED_OCM_BASE_ADDRESS + 3000))      // 3000
//memory to save configuration request and response of all devices
#define MEM_DEVICE_CONF_REQ     ((u8 *)(SHARED_OCM_BASE_ADDRESS + 3100))       //3100-6099, because each can use up to 300 bytes maximal
#define MEM_DEVICE_CONF_RES     ((u8 *)(SHARED_OCM_BASE_ADDRESS + 6100))       //6100-6399, because each can use up to 30 bytes maximal
//memory parts of configuration packet number, one byte only
#define MEM_DEVICE_CONF_NUM  	((int *)(SHARED_OCM_BASE_ADDRESS + 6200))      // 6200
#define MAX_BYTE			6201





#define MUTEX_SHARED_MEMORY_ID_1F_REQ 		3
#define MUTEX_SHARED_MEMORY_ID_1F_RES 		4
#define MUTEX_SHARED_MEMORY_ID_1E_REQ 		5
#define MUTEX_SHARED_MEMORY_ID_1E_RES 		6
#define MUTEX_SHARED_MEMORY_ID_1D_REQ 		7
#define MUTEX_SHARED_MEMORY_ID_1D_RES 		8
#define MUTEX_SHARED_MEMORY_ID_1C_REQ 		9
#define MUTEX_SHARED_MEMORY_ID_1C_RES 		10
#define MUTEX_SHARED_MEMORY_ID_1B_REQ 		11
#define MUTEX_SHARED_MEMORY_ID_1B_RES 		12
#define MUTEX_SHARED_MEMORY_ID_20_REQ 		13
#define MUTEX_SHARED_MEMORY_ID_20_RES 		14
#define MUTEX_SHARED_MEMORY_ID_21_REQ 		15
#define MUTEX_SHARED_MEMORY_ID_21_RES 		16
#define MUTEX_SHARED_MEMORY_ID_22_REQ 		17
#define MUTEX_SHARED_MEMORY_ID_22_RES 		18
#define MUTEX_SHARED_MEMORY_ID_23_REQ 		19
#define MUTEX_SHARED_MEMORY_ID_23_RES 		20
#define MUTEX_SHARED_MEMORY_ID_24_REQ 		21
#define MUTEX_SHARED_MEMORY_ID_24_RES 		22
#define MUTEX_ID_MEM_DEVICE_INFOR 		    23
#define MUTEX_ID_MEM_DEVICE_CONF_REQ 		24
#define MUTEX_ID_MEM_DEVICE_CONF_RES 		25
#define MUTEX_ID_MEM_DEVICE_CONF_NUM 		26
#define MUTEX_ID_MEM_INTC_CONF  		    27


#endif /* SRC_SHARED_MEMORY_H_ */
