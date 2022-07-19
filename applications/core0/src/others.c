/*
 * others.c
 *
 *  Created on: 28.06.2022
 *      Author: Ha
 */
#include <sys/time.h>
#include "xil_types.h"
#include "custom_definitions.h"
#include "xuartlite.h"
#include "default_types.h"
#include "xtime_l.h"
#include <stdio.h>
#include <unistd.h>



extern int error_count;
extern u8 DataDiode_address;


extern Init_Config_s Init_Config_status;


/****************************************************************************/
/**
*
* This function used to reset some variables created by the during of program.
*
* @param	None.
*
* @return	None.
*
* @note		None.
*
*****************************************************************************/
void Reset_Variables(){
	DataDiode_address = 0;
	Init_Config_status.Device_count = 0;
	Init_Config_status.Puls_detected = 0;
	Init_Config_status.Infor_exchanged = 0;
	Init_Config_status.All_emulated = 0;
	Init_Config_status.All_configured = 0;
	Init_Config_status.Config_Req_Paket_count = 0;
	Init_Config_status.Config_Res_Paket_count = 0;
	error_count = 0;
}


/****************************************************************************/
/**
*
* This function used to reset a variable given by its pointer.
*
* @param	val is the pointer to the variable.
*
* @return	None.
*
* @note		None.
*
*****************************************************************************/
void Reset(int *val){
	*val = 0;
}


/****************************************************************************/
/**
*
* This function used to reset the FIFO of Uartlite instance.
*
* @param	UartLiteInstPtr is the pointer to the Uartlite instance.
*
* @return	None.
*
* @note		None.
*
*****************************************************************************/
void Reset_FIFO(XUartLite *UartLiteInstPtr){
	usleep(20);
	XUartLite_ResetFifos(UartLiteInstPtr);
	usleep(20);
}


/****************************************************************************/
/**
*
* This function used to wait until a number of bytes are received successfully through the Pmod RS-485 for a MAX_WAIT_TIME.
*
* @param	Pmod_Control contains control informations for the Pmod RS-485.
*
* @param	UartLiteInstPtr is a pointer to the Uartlite instance.
*
* @param	byte_num is the number of byte needs to be received.
*
* @return	SYSTEM_TIME_OUT: time out when tries to receive byte_num of data bytes.
* 			Otherwise SYSTEM_SUCCESS.
*
* @note		None.
*
*****************************************************************************/
int Pmod_Wait_Receive_Data(PmodRS485_Control Pmod_Control, XUartLite *UartLiteInstPtr, unsigned int byte_num){
	int time_out = 0;
	XTime tStart = 0, tEnd = 0, tLimit = 0;
	XTime_GetTime(&tStart);
	XTime_GetTime(&tEnd);

	tLimit = MAX_WAIT_TIME*COUNTS_PER_SECOND;

	while ((!(time_out=((tEnd - tStart) > tLimit))) && (*Pmod_Control.TotalReceivedCount_Ptr != byte_num)){
		XTime_GetTime(&tEnd);
	}
	if (time_out == 0) {
		*Pmod_Control.TotalReceivedCount_Ptr = 0;
		Reset_FIFO(UartLiteInstPtr);
	}
	else if (time_out == 1){
		xil_printf("Error: program%d: Time runs out when waiting to receive data on RS-485 channel\r\n",XPAR_CPU_ID);
		return SYSTEM_TIME_OUT;
	}
	return SYSTEM_SUCCESS;
}


/****************************************************************************/
/**
*
* This function used to wait until a number of bytes are sent successfully through the Pmod RS-485 for a MAX_WAIT_TIME.
*
* @param	Pmod_Control contains control informations for the Pmod RS-485.
*
* @param	UartLiteInstPtr is a pointer to the Uartlite instance.
*
* @param	byte_num is the number of byte needs to be received.
*
* @return	SYSTEM_TIME_OUT: time out when tries to receive byte_num of data bytes.
* 			Otherwise SYSTEM_SUCCESS.
*
* @note		None.
*
*****************************************************************************/
int Pmod_Wait_Send_Data(PmodRS485_Control Pmod_Control, XUartLite *UartLiteInstPtr, unsigned int byte_num){
	int time_out = 0;
	XTime tStart = 0, tEnd = 0, tLimit = 0;
	XTime_GetTime(&tStart);
	XTime_GetTime(&tEnd);

	tLimit = MAX_WAIT_TIME*COUNTS_PER_SECOND;

	while ((!(time_out=((tEnd - tStart) > tLimit))) && (*Pmod_Control.TotalSentCount_Ptr != byte_num)){
		XTime_GetTime(&tEnd);
	}

	if (time_out == 0) {
		*Pmod_Control.TotalSentCount_Ptr = 0;
		Reset_FIFO(UartLiteInstPtr);
	}
	else {
		xil_printf("Error: program%d: Time runs out when waiting to send data on RS-485 channel\r\n",XPAR_CPU_ID);
		return SYSTEM_TIME_OUT;
	}
	return SYSTEM_SUCCESS;
}




