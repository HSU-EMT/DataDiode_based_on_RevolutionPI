/*
 * custom_definition.h
 *
 *  Created on: 25.01.2022
 *      Author: Ha
 */

#ifndef __CUSTOM_DEFINITIONS_H_
#define __CUSTOM_DEFINITIONS_H_

#include "xgpiops.h"
#include "xparameters.h"
#include "xscugic.h"
#include "xgpio.h"


/************************** Constant Definitions *****************************/
#define UARTLITE_DEVICE_0	  XPAR_UARTLITE_0_DEVICE_ID
#define UARTLITE_DEVICE_1	  XPAR_UARTLITE_1_DEVICE_ID
#define UARTLITE_IRPT_INTR_0	  XPAR_FABRIC_UARTLITE_0_VEC_ID
#define UARTLITE_IRPT_INTR_1	  XPAR_FABRIC_UARTLITE_1_VEC_ID
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID


/************************** Custom Definitions *****************************/
#if XPAR_CPU_ID == 0
#define JF_PMOD_CORE0_WIRE1		        			13
#define JF_PMOD_CORE0_WIRE2		        			10
#elif XPAR_CPU_ID == 1
#define JF_PMOD_CORE0_WIRE1		        			0
#define JF_PMOD_CORE0_WIRE2		        			9
#endif

#define SELF_DATA_BUFFER_SIZE		        		100
#define BYTE_SIZE_GET_DEVICE_INFOR_DIO_REQUEST		8
#define BYTE_SIZE_GET_DEVICE_INFOR_DIO_RESPONSE		30
#define BYTE_SIZE_SET_ADDRESS_DIO_REQUEST			8
#define BYTE_SIZE_SET_ADDRESS_DIO_RESPONSE			8

#define BYTE_SIZE_CONFIG_DIO_RESPONSE				3
#define BYTE_SIZE_CONFIG_RESPONSE					3
#define BYTE_SIZE_DATA_EXCHANGE_DIO_RESPONSE 		33
#define BYTE_SIZE_NUTZ_DATA_EXCHANGE_DIO_RESPONSE 	30
#define BYTE_SIZE_BROADCAST   						8
#define BYTE_SIZE_HEADER   						    2
#define BYTE_SIZE_HEADER_INIT   					7
#define BYTE_SIZE_TOTAL_INIT_REQ   					8
#define BYTE_SIZE_WRITE_REQ_STEP   					4
#define BYTE_SIZE_RESET_SIGNAL						3

#define MAX_DEVICE_NUM    						    10
#define MAX_TRY_IF_ERROR    						5
#define MAX_WAIT_TIME    							2 //seconds

#define SMALLEST_ADDRESS 							0x1B
#define BIGGEST_ADDRESS 							0x24

#define ADDRESS_MASK 								0x3F //last 6 bits
#define DATA_LENGTH_MASK 							0x1F //last 5 bits
#define COMMAND_MASK 							    0xE0 //first 3 bits
#define RESPONSE_MASK 							    0x80 //first 1 bits
#define INFOR_EXCHANGED_CODE    					0x0A
#define ADDRESS_SETTING_CODE    					0x14
#define DIO_CONFIG_CODE    							0x20
#define INIT_RES_START_CODE    						0x00
#define INIT_REQ_START_CODE    						0x4D
#define BROADCAST_START_CODE    					0xFF

#define IOP_TYP1_CMD_DATA       					0 //command for cyclic data exchange DIO no counter/encoder
#define IOP_TYP1_CMD_CFG        					1 //command for configuration module type 1 like DIO module
#define IOP_TYP1_CMD_DATA2							2 //command for cyclic data exchange DIO with counter/encoder



#define INTC		    XScuGic
#define INTC_HANDLER	XScuGic_InterruptHandler

/************************** Custom Structures *****************************/

/*
 * This typedef contains control informations for the Pmod RS-485.
 */
typedef struct _PmodRS485_Control {
	volatile int *TotalSentCount_Ptr; 	  /* Number of bytes are sent through Pmod RS-485 */
	volatile int *TotalReceivedCount_Ptr; /* Number of bytes are received through Pmod RS-485 */
	XGpio 		 *rx_disnable_Ptr;        /* Disable receive channel of Pmod RS-485 */
	XGpio 		 *tx_enable_Ptr;          /* Enable transmit channel of Pmod RS-485 */
} PmodRS485_Control;

/*
 * This typedef contains informations for the initialization (devices infors exchange and address setting)
 * and configuration of devices on PiBridge
 */
typedef struct _Init_Config_status {
	int Puls_detected;
	int Infor_exchanged;
	int Device_count;
	int All_emulated;
	int Config_Res_Paket_count;
	int Config_Req_Paket_count;
	int All_configured;
}Init_Config_s;



#endif /* __CUSTOM_DEFINITIONS_H_ */
