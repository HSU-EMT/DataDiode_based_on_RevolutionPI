/******************************************************************************
*
* Copyright (C) 2008 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

#ifndef __PLATFORM_H_
#define __PLATFORM_H_

#include "xparameters.h"
#include "xil_mmu.h"

//////////////////////////////////////////////////////////////////////////////
/// CORE-Depended DDR-Memory
///

#if XPAR_CPU_ID == 0
//////////////////////////////////////////////////////////////////////////////
/// CORE 0 - Defines
///

#define DDR3_RAM_BASE_ADDRESS			0x00100000
#define DDR3_RAM_SIZE					0x0FF80000

#elif XPAR_CPU_ID == 1
//////////////////////////////////////////////////////////////////////////////
/// CORE 1 - Defines
///

#define DDR3_RAM_BASE_ADDRESS			0x10080000
#define DDR3_RAM_SIZE					0x0FF80000

#endif

//////////////////////////////////////////////////////////////////////////////
/// Shared Memory
///
#define SHARED_OCM_BASE_ADDRESS			0xFFFF0000
#define SHARED_OCM_SIZE					0x0000FE00

//////////////////////////////////////////////////////////////////////////////
/// DualCore - Start Address
///
#define ARM1_STARTADR 0xFFFFFFF0
#define ARM1_BASEADDR 0x10080000

#define ARM_CORE_0						0
#define ARM_CORE_1						1





//////////////////////////////////////////////////////////////////////////////
/// MACROS
///
#define ExecSEV()	__asm__("sev")

#define PlatformDisableOcmCache()		    Xil_SetTlbAttributes(SHARED_OCM_BASE_ADDRESS,0x14de2);

void PlatformWakeupCore(unsigned int startaddress, unsigned int baseaddress);


void init_platform();
void cleanup_platform();

#endif
