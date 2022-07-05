/*
 * diode_core0_functions.h
 *
 *  Created on: 27.06.2022
 *      Author: Ha
 */

#ifndef __DIODE_CORE0_FUNCTIONS_H_
#define __DIODE_CORE0_FUNCTIONS_H_


#include <stdio.h>
#include <unistd.h>
#include "xuartlite.h"
#include "xil_printf.h"
#include "xgpio.h"
#include "xgpiops.h"


void Puls_detect(XGpioPs* Gpio_Ptr, u32 sniff1, u32 sniff2, Init_Config_s *Init_Config_status);
void Collect_Information_Devices(Init_Config_s *Init_Config_status, XUartLite *UartLiteInstPtr, XGpioPs *Gpio_Ptr, PmodRS485_Control Pmod_Control);
void DataDiode_Infor_Exchange(Init_Config_s *Init_Config_status, XUartLite *UartLiteInstPtr, XGpioPs *Gpio_Ptr, PmodRS485_Control Pmod_Control);
void DataDiode_Adress_Setting(Init_Config_s *Init_Config_status, u8 *DataDiode_address, XUartLite *UartLiteInstPtr, XGpioPs *Gpio_Ptr, PmodRS485_Control Pmod_Control);
int Read_PiBridge_Write_OCM(Init_Config_s *Init_Config_status, u8 DataDiode_address, XUartLite *UartLiteInstPtr, PmodRS485_Control Pmod_Control, int *error_count);
void DataDiode_Response_to_master(u8 DataDiode_address, u8 Command, XUartLite *UartLiteInstPtr, PmodRS485_Control Pmod_Control);
void Write_data_to_OCM(u8* HeaderBuffer, int sizeheader, u8* DataBuffer, int sizedata, u8* MEM_BASE_ADDRESS, int MUTEX_MEMORY_ID);
u8 Crc_Cal(u8 * pi8uFrame_p, u16 i16uLen_p);




#endif /* __DIODE_CORE0_FUNCTIONS_H_ */
