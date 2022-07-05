/*
 * diode_core0_functions.c
 *
 *  Created on: 27.06.2022
 *      Author: Ha
 */


/*****************************************************************************/

#include <unistd.h>
#include "xil_types.h"
#include "xgpiops.h"
#include "xgpio.h"
#include "xuartlite.h"
#include "custom_definitions.h"
#include "mutex.h"
#include "shared_memory.h"
#include "diode_core0_functions.h"
#include "others.h"



static u8 ResponseBuffer_GetDeviceInfor[BYTE_SIZE_GET_DEVICE_INFOR_DIO_RESPONSE] = {INIT_RES_START_CODE,0x00,
		INFOR_EXCHANGED_CODE,0x40,0x00,0x00,0x16,0x81,0x36,0x00,0x00,0x60,0x00,0x01,0x00,0x01,0x00,0x04,
		0x00,0xD1,0x30,0x00,0x00,0x46,0x00,0x12,0x00,0x02,0x00,0x00};
static u8 ResponseBuffer_SetAddress[BYTE_SIZE_SET_ADDRESS_DIO_RESPONSE] = {INIT_RES_START_CODE, 0x00, ADDRESS_SETTING_CODE, 0x40, 0x00, 0x00, 0x00, 0x00};
static u8 Self_Data_Buffer[SELF_DATA_BUFFER_SIZE];
static u8 HeaderBuffer[BYTE_SIZE_HEADER];
static u8 ResetSignal[BYTE_SIZE_RESET_SIGNAL] = {0x7F, 0x00, 0x7F};
static u8 Command, ReqRes, Device_address = 0;


/****************************************************************************/
/**
*
* This function monitors PiBridge and set variable Puls_detected to 1 when the pulse from RevPi master occurs.
*
* @param	Gpio_Ptr is a pointer to the GPIO instance.
*
* @param	sniff1 is the status of sniff wire 1.
*
* @param	sniff2 is the status of sniff wire 2.
*
* @param	Init_Config_s is a pointer to Init_Config_s instance, which contains informations for the initialization
* and configuration of devices on PiBridge.
*
* @return	None.
*
* @note		None.
*
*****************************************************************************/
void Puls_detect(XGpioPs *Gpio_Ptr, u32 sniff1, u32 sniff2, Init_Config_s *Init_Config_status){
	if ((sniff1 == 1) && (sniff2 == 1) && (Init_Config_status->Puls_detected == 0)) {

		usleep(1000);
		sniff1 = XGpioPs_ReadPin(Gpio_Ptr, JF_PMOD_CORE0_WIRE1);
		sniff2 = XGpioPs_ReadPin(Gpio_Ptr, JF_PMOD_CORE0_WIRE2);

		if ((sniff1 == 1) && (sniff2 == 1) && (Init_Config_status->Puls_detected == 0)) {
			usleep(17000);
			XGpioPs_SetDirectionPin(Gpio_Ptr, JF_PMOD_CORE0_WIRE2, 1);
			XGpioPs_SetOutputEnablePin(Gpio_Ptr, JF_PMOD_CORE0_WIRE2, 1);
			XGpioPs_WritePin(Gpio_Ptr, JF_PMOD_CORE0_WIRE2, 1);
			Init_Config_status->Puls_detected = 1;
			xil_printf("program0: Puls_detected %d\r\n", Init_Config_status->Puls_detected);
			XGpioPs_SetDirectionPin(Gpio_Ptr, JF_PMOD_CORE0_WIRE1, 0);

		}
	}
}



/****************************************************************************/
/**
*
* This function monitors PiBridge and collects all informations of all real devices during their initialization phase.
* The data will be saved on the shared memory for future use in the IT network.
*
* @param	Init_Config_s is a pointer to Init_Config_s instance, which contains status for the initialization
* and configuration of devices on PiBridge.
*
* @param	UartLiteInstPtr is a pointer to the Uartlite instance.
*
* @param	Gpio_Ptr is a pointer to the GPIO instance.
*
* @param	Pmod_Control contains control informations for the Pmod RS-485.
*
* @return	None.
*
* @note		None.
*
*****************************************************************************/
void Collect_Information_Devices(Init_Config_s *Init_Config_status, XUartLite *UartLiteInstPtr, XGpioPs *Gpio_Ptr, PmodRS485_Control Pmod_Control){
	int data_length = 0;
	u32 sniff1 = XGpioPs_ReadPin(Gpio_Ptr, JF_PMOD_CORE0_WIRE1);
	u32 sniff2 = XGpioPs_ReadPin(Gpio_Ptr, JF_PMOD_CORE0_WIRE2);
	while ((sniff1 == 1) && (sniff2 == 1) && (Init_Config_status->Puls_detected == 1)) {

		Reset_FIFO(UartLiteInstPtr);

		u8 HelpBuffer[BYTE_SIZE_HEADER_INIT];
		XUartLite_Recv(UartLiteInstPtr, HelpBuffer, BYTE_SIZE_HEADER_INIT);
		Pmod_Wait_Receive_Data(Pmod_Control, UartLiteInstPtr, BYTE_SIZE_HEADER_INIT);

		data_length = (int)(HelpBuffer[BYTE_SIZE_HEADER_INIT-1]) + 1; // one more for CRC Check Byte at the end of frame
		u8 HelpDataBuffer[data_length];

		XUartLite_Recv(UartLiteInstPtr, HelpDataBuffer, data_length);
		Pmod_Wait_Receive_Data(Pmod_Control, UartLiteInstPtr, data_length);

		if ((HelpBuffer[0] == INIT_RES_START_CODE) && (HelpBuffer[2] == INFOR_EXCHANGED_CODE)){
			/*
			 * HelpDataBuffer contains device information.
			 * Save all device information with header on the shared memory to future use
			 */
			u8* Help_MEM_BASE = MEM_DEVICE_INFOR + 100*(Init_Config_status->Device_count);
			MTX_Lock(MUTEX_ID_MEM_DEVICE_INFOR);

			for(int i = 0; i< BYTE_SIZE_HEADER_INIT; i++){
				Help_MEM_BASE[i] = HelpBuffer[i];
			}
			for(int i = 0; i< data_length; i++){
				Help_MEM_BASE[i + BYTE_SIZE_HEADER_INIT] = HelpDataBuffer[i];
			}
			MTX_Unlock(MUTEX_ID_MEM_DEVICE_INFOR);
			Init_Config_status->Device_count +=1;
		}else{;}

		sniff1 = XGpioPs_ReadPin(Gpio_Ptr, JF_PMOD_CORE0_WIRE1);
		sniff2 = XGpioPs_ReadPin(Gpio_Ptr, JF_PMOD_CORE0_WIRE2);
	}
	/*
	 * write the number of real module
	 */
	MTX_Lock(MUTEX_ID_MEM_DEVICE_INFOR);
	MEM_DEVICE_NUM[0] = Init_Config_status->Device_count;
	MTX_Unlock(MUTEX_ID_MEM_DEVICE_INFOR);

	xil_printf("program0: Number of real I/O Module: %d\r\n", Init_Config_status->Device_count);

}



/****************************************************************************/
/**
*
* This function proceeds the device infors exchange ( part 1 of initialization phase) for the data diode with the RevPi Master.
*
* @param	Init_Config_s is a pointer to Init_Config_s instance, which contains status for the initialization
* and configuration of devices on PiBridge.
*
* @param	UartLiteInstPtr is a pointer to the Uartlite instance.
*
* @param	Gpio_Ptr is a pointer to the GPIO instance.
*
* @param	Pmod_Control contains control informations for the Pmod RS-485.
*
* @return	None.
*
* @note		None.
*
*****************************************************************************/
void DataDiode_Infor_Exchange(Init_Config_s *Init_Config_status, XUartLite *UartLiteInstPtr, XGpioPs *Gpio_Ptr, PmodRS485_Control Pmod_Control){

	u32 sniff1 = XGpioPs_ReadPin(Gpio_Ptr, JF_PMOD_CORE0_WIRE1);
	u32 sniff2 = XGpioPs_ReadPin(Gpio_Ptr, JF_PMOD_CORE0_WIRE2);

	if ((sniff1 == 0) && (sniff2 == 1) && (Init_Config_status->Puls_detected == 1) && (Init_Config_status->Infor_exchanged == 0)){
		Reset_FIFO(UartLiteInstPtr);
		u8 RequestBuffer_GetDeviceInfor1[BYTE_SIZE_GET_DEVICE_INFOR_DIO_REQUEST];

		XUartLite_Recv(UartLiteInstPtr, RequestBuffer_GetDeviceInfor1, BYTE_SIZE_GET_DEVICE_INFOR_DIO_REQUEST);
		Pmod_Wait_Receive_Data(Pmod_Control, UartLiteInstPtr, BYTE_SIZE_GET_DEVICE_INFOR_DIO_REQUEST);

		if (RequestBuffer_GetDeviceInfor1[0] == INIT_REQ_START_CODE){
			ResponseBuffer_GetDeviceInfor[BYTE_SIZE_GET_DEVICE_INFOR_DIO_RESPONSE-1] = Crc_Cal(ResponseBuffer_GetDeviceInfor, BYTE_SIZE_GET_DEVICE_INFOR_DIO_RESPONSE-1);

			usleep(1000);
			XGpio_DiscreteWrite(Pmod_Control.tx_enable_Ptr, 1, 1);
			XGpio_DiscreteWrite(Pmod_Control.rx_disnable_Ptr, 1, 1);

			XUartLite_Send(UartLiteInstPtr, ResponseBuffer_GetDeviceInfor, BYTE_SIZE_GET_DEVICE_INFOR_DIO_RESPONSE);
			Pmod_Wait_Send_Data(Pmod_Control, UartLiteInstPtr, BYTE_SIZE_GET_DEVICE_INFOR_DIO_RESPONSE);

			XGpio_DiscreteWrite(Pmod_Control.tx_enable_Ptr, 1, 0);
			XGpio_DiscreteWrite(Pmod_Control.rx_disnable_Ptr, 1, 0);

			xil_printf("program0: Device infor exchange for Data Diode successful\r\n");
			Init_Config_status->Infor_exchanged = 1;

		}
	}
}




/****************************************************************************/
/**
*
* This function proceeds the device address setting ( part 2 of initialization phase) for the data diode with the RevPi Master.
*
* @param	Init_Config_s is a pointer to Init_Config_s instance, which contains informations for the initialization
* and configuration of devices on PiBridge.
*
* @param	DataDiode_address_Ptr is a pointer to the address of the data diode.
*
* @param	UartLiteInstPtr is a pointer to the Uartlite instance.
*
* @param	Gpio_Ptr is a pointer to the GPIO instance.
*
* @param	Pmod_Control contains control informations for the Pmod RS-485.
*
* @return	None.
*
* @note		None.
*
*****************************************************************************/
void DataDiode_Adress_Setting(Init_Config_s *Init_Config_status, u8 *DataDiode_address_Ptr, XUartLite *UartLiteInstPtr, XGpioPs *Gpio_Ptr, PmodRS485_Control Pmod_Control){
	u32 sniff1 = XGpioPs_ReadPin(Gpio_Ptr, JF_PMOD_CORE0_WIRE1);
	u32 sniff2 = XGpioPs_ReadPin(Gpio_Ptr, JF_PMOD_CORE0_WIRE2);

	if ((sniff1 == 0) && (sniff2 == 1) && (Init_Config_status->Puls_detected == 1) && (Init_Config_status->Infor_exchanged == 1) && (*DataDiode_address_Ptr == 0)){

		usleep(1000);
		u8 RequestBuffer_SetAddress[BYTE_SIZE_SET_ADDRESS_DIO_REQUEST];

		XUartLite_Recv(UartLiteInstPtr, RequestBuffer_SetAddress, BYTE_SIZE_SET_ADDRESS_DIO_REQUEST);
		Pmod_Wait_Receive_Data(Pmod_Control, UartLiteInstPtr, BYTE_SIZE_SET_ADDRESS_DIO_REQUEST);

		*DataDiode_address_Ptr = RequestBuffer_SetAddress[0] & ADDRESS_MASK;

		ResponseBuffer_SetAddress[1] = *DataDiode_address_Ptr;
		ResponseBuffer_SetAddress[BYTE_SIZE_SET_ADDRESS_DIO_RESPONSE-1] = Crc_Cal(ResponseBuffer_SetAddress, BYTE_SIZE_SET_ADDRESS_DIO_RESPONSE-1);

		usleep(2000);
		XGpio_DiscreteWrite(Pmod_Control.tx_enable_Ptr, 1, 1);
		XGpio_DiscreteWrite(Pmod_Control.rx_disnable_Ptr, 1, 1);

		XUartLite_Send(UartLiteInstPtr, ResponseBuffer_SetAddress, BYTE_SIZE_SET_ADDRESS_DIO_RESPONSE);
		Pmod_Wait_Send_Data(Pmod_Control, UartLiteInstPtr, BYTE_SIZE_SET_ADDRESS_DIO_RESPONSE);

		XGpio_DiscreteWrite(Pmod_Control.tx_enable_Ptr, 1, 0);
		XGpio_DiscreteWrite(Pmod_Control.rx_disnable_Ptr, 1, 0);

		XGpioPs_SetDirectionPin(Gpio_Ptr, JF_PMOD_CORE0_WIRE2, 0);
		usleep(100);
		XGpioPs_SetDirectionPin(Gpio_Ptr, JF_PMOD_CORE0_WIRE1, 0);
		usleep(1000);
		xil_printf("program0: Address setting for Data Diode successful\r\n");

	}
}



/****************************************************************************/
/**
*
* This function proceeds the main function of data diode in the OT network. The data diode reads data on the PiBridge
* and writes data on the shared memory for future use in the IT network.
*
* @param	Init_Config_s is a pointer to Init_Config_s instance, which contains informations for the initialization
* and configuration of devices on PiBridge.
*
* @param	DataDiode_address_Ptr is a pointer to the address of the data diode.
*
* @param	UartLiteInstPtr is a pointer to the Uartlite instance.
*
* @param	Pmod_Control contains control informations for the Pmod RS-485.
*
* @param	error_count is a pointer to the number of error occurs during the functionality of the data diode.
*
* @return	SYSTEM_RESET: when the user resets whole system on PiCtory.
* 			SYSTEM_FAILED: when an error occurs during functionality of the data diode.
* 			otherwise SYSTEM_SUCCESS.
*
* @note		None.
*
*****************************************************************************/
int Read_PiBridge_Write_OCM(Init_Config_s *Init_Config_status, u8 DataDiode_address, XUartLite *UartLiteInstPtr, PmodRS485_Control Pmod_Control, int *error_count){
	int data_length = 0;

	XUartLite_Recv(UartLiteInstPtr, HeaderBuffer, BYTE_SIZE_HEADER);
	Pmod_Wait_Receive_Data(Pmod_Control, UartLiteInstPtr, BYTE_SIZE_HEADER);

	if(HeaderBuffer[0] == BROADCAST_START_CODE){
		XUartLite_Recv(UartLiteInstPtr, Self_Data_Buffer, (BYTE_SIZE_BROADCAST-BYTE_SIZE_HEADER));
		Pmod_Wait_Receive_Data(Pmod_Control, UartLiteInstPtr, (BYTE_SIZE_BROADCAST-BYTE_SIZE_HEADER));

		xil_printf("program0: Broadcast signal from RevPi Master received\r\n");
		xil_printf("program0: Configuration & Cyclically data exchange phase started\r\n");
		xil_printf("program0: Normal Operation...\r\n");
	}
	else {

	data_length = (int)(HeaderBuffer[1] & DATA_LENGTH_MASK) + 1; // one more for CRC Check Byte an the end of frame
	u8 DataBuffer[data_length];

	/*
	 * check reset signal for whole system
	 */
	if ((HeaderBuffer[0] == ResetSignal[0]) && (data_length == 1)) {
		return SYSTEM_RESET;
	}

	/*
	 * Receive useful data
	 */
	XUartLite_Recv(UartLiteInstPtr, DataBuffer, data_length);
	Pmod_Wait_Receive_Data(Pmod_Control, UartLiteInstPtr, data_length);

	int sizeheader = sizeof(HeaderBuffer);
	int sizedata = sizeof(DataBuffer);
	Command = (HeaderBuffer[1] & COMMAND_MASK) >> 5;
	Device_address = HeaderBuffer[0] & ADDRESS_MASK;
	ReqRes = (HeaderBuffer[0] & RESPONSE_MASK) >> 7;

	/*
	 * Data diode responses to master in its cyclically data exchange phase
	 */
	if ((Device_address == DataDiode_address) && (Init_Config_status->All_configured != 0)) {
		DataDiode_Response_to_master(DataDiode_address, Command, UartLiteInstPtr, Pmod_Control);
	}

	/*
	 * Data diode responses to master in its configuration phase
	 */
	else if ((Device_address == DataDiode_address) &&(Init_Config_status->All_configured == 0)) {
		DataDiode_Response_to_master(DataDiode_address, Command, UartLiteInstPtr, Pmod_Control);
		Init_Config_status->All_configured = 1;

		MTX_Lock(MUTEX_ID_MEM_DEVICE_CONF_NUM);
		MEM_DEVICE_CONF_NUM[0] = Init_Config_status->Config_Req_Paket_count;
		MTX_Unlock(MUTEX_ID_MEM_DEVICE_CONF_NUM);
	}
	/*
	 * Data diode saves configuration data of all other real devices to the shared memory
	 */
	else if ((Device_address != DataDiode_address) && (Init_Config_status->All_configured == 0)) {

		if (ReqRes == 0){
			Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_DEVICE_CONF_REQ + 100*(Init_Config_status->Config_Req_Paket_count), MUTEX_ID_MEM_DEVICE_CONF_REQ);
			Init_Config_status->Config_Req_Paket_count += 1;
		}
		else if (ReqRes == 1){
			Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_DEVICE_CONF_RES + 10*(Init_Config_status->Config_Res_Paket_count), MUTEX_ID_MEM_DEVICE_CONF_RES);
			Init_Config_status->Config_Res_Paket_count += 1;

		}
	}
	// Data diode writes data of all other real devices in cyclically data exchange phase to the shared memory
	else if ((Device_address != DataDiode_address) && (Init_Config_status->All_configured == 1)) {
		if (ReqRes == 0) {
			switch(Device_address)
			{
			case 0x1F:
				Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_REQ_1F, MUTEX_SHARED_MEMORY_ID_1F_REQ);
				break;
			case 0x1E:
				Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_REQ_1E, MUTEX_SHARED_MEMORY_ID_1E_REQ);
				break;
			case 0x1D:
				Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_REQ_1D, MUTEX_SHARED_MEMORY_ID_1D_REQ);
				break;
			case 0x1C:
				Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_REQ_1C, MUTEX_SHARED_MEMORY_ID_1C_REQ);
				break;
			case 0x1B:
				Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_REQ_1B, MUTEX_SHARED_MEMORY_ID_1B_REQ);
				break;
			case 0x20:
				Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_REQ_20, MUTEX_SHARED_MEMORY_ID_20_REQ);
				break;
			case 0x21:
				Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_REQ_21, MUTEX_SHARED_MEMORY_ID_21_REQ);
				break;
			case 0x22:
				Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_REQ_22, MUTEX_SHARED_MEMORY_ID_22_REQ);
				break;
			case 0x23:
				Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_REQ_23, MUTEX_SHARED_MEMORY_ID_23_REQ);
				break;
			case 0x24:
				Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_REQ_24, MUTEX_SHARED_MEMORY_ID_24_REQ);
				break;
			default:
				xil_printf("Error: program0 reads wrong request packet\r\n");
				*error_count += 1;
				return SYSTEM_FAILED;
			}
		}
		else if (ReqRes == 1) {
			switch(Device_address)
			{
			case 0x1F:
				Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_RES_1F, MUTEX_SHARED_MEMORY_ID_1F_RES);
				break;
			case 0x1E:
				Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_RES_1E, MUTEX_SHARED_MEMORY_ID_1E_RES);
				break;
			case 0x1D:
				Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_RES_1D, MUTEX_SHARED_MEMORY_ID_1D_RES);
				break;
			case 0x1C:
				Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_RES_1C, MUTEX_SHARED_MEMORY_ID_1C_RES);
				break;
			case 0x1B:
				Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_RES_1B, MUTEX_SHARED_MEMORY_ID_1B_RES);
				break;
			case 0x20:
				Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_RES_20, MUTEX_SHARED_MEMORY_ID_20_RES);
				break;
			case 0x21:
				Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_RES_21, MUTEX_SHARED_MEMORY_ID_21_RES);
				break;
			case 0x22:
				Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_RES_22, MUTEX_SHARED_MEMORY_ID_22_RES);
				break;
			case 0x23:
				Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_RES_23, MUTEX_SHARED_MEMORY_ID_23_RES);
				break;
			case 0x24:
				Write_data_to_OCM(HeaderBuffer, sizeheader, DataBuffer, sizedata, MEM_RES_24, MUTEX_SHARED_MEMORY_ID_24_RES);
				break;
			default:
				xil_printf("Error: program0 reads wrong response packet\r\n");
				*error_count += 1;
				return SYSTEM_FAILED;
				}
			}
		}
	}
	return SYSTEM_SUCCESS;
}





/****************************************************************************/
/**
*
* This function used by data diode to response to RevPi master in their the configuration phase or cyclically data exchange phase.
*
* @param	DataDiode_address is the address of the data diode.
*
* @param	Command tells the data diode must response on the configuration phase or cyclically data exchange phase.
*
* @param	UartLiteInstPtr is a pointer to the Uartlite instance.
*
* @param	Pmod_Control contains control informations for the Pmod RS-485.
*
* @return	None.
*
* @note		None.
*
*****************************************************************************/
void DataDiode_Response_to_master(u8 DataDiode_address, u8 Command, XUartLite *UartLiteInstPtr, PmodRS485_Control Pmod_Control){

	if   (Command == IOP_TYP1_CMD_CFG) {

		XGpio_DiscreteWrite(Pmod_Control.tx_enable_Ptr, 1, 1);
		XGpio_DiscreteWrite(Pmod_Control.rx_disnable_Ptr, 1, 1);
		usleep(100);

		u8 ResponseBuffer_Config_DIO[BYTE_SIZE_CONFIG_DIO_RESPONSE] = {0x00, DIO_CONFIG_CODE, 0x00};
		ResponseBuffer_Config_DIO[0] = DataDiode_address ^ RESPONSE_MASK;
		ResponseBuffer_Config_DIO[BYTE_SIZE_CONFIG_DIO_RESPONSE-1] = Crc_Cal(ResponseBuffer_Config_DIO, BYTE_SIZE_CONFIG_DIO_RESPONSE-1);


		XUartLite_Send(UartLiteInstPtr, ResponseBuffer_Config_DIO, BYTE_SIZE_CONFIG_DIO_RESPONSE);
		Pmod_Wait_Send_Data(Pmod_Control, UartLiteInstPtr, BYTE_SIZE_CONFIG_DIO_RESPONSE);

		XGpio_DiscreteWrite(Pmod_Control.tx_enable_Ptr, 1, 0);
		XGpio_DiscreteWrite(Pmod_Control.rx_disnable_Ptr, 1, 0);

	}
	else if (Command == IOP_TYP1_CMD_DATA){

		XGpio_DiscreteWrite(Pmod_Control.tx_enable_Ptr, 1, 1);
		XGpio_DiscreteWrite(Pmod_Control.rx_disnable_Ptr, 1, 1);
		usleep(100);

		u8 ResponseBuffer_Data_Exchange_Diode[BYTE_SIZE_DATA_EXCHANGE_DIO_RESPONSE] = {0};
		ResponseBuffer_Data_Exchange_Diode[0] = DataDiode_address ^ RESPONSE_MASK;
		ResponseBuffer_Data_Exchange_Diode[1] = BYTE_SIZE_NUTZ_DATA_EXCHANGE_DIO_RESPONSE ^ (IOP_TYP1_CMD_DATA2 << 5);
		ResponseBuffer_Data_Exchange_Diode[BYTE_SIZE_DATA_EXCHANGE_DIO_RESPONSE-1] = Crc_Cal(ResponseBuffer_Data_Exchange_Diode, BYTE_SIZE_DATA_EXCHANGE_DIO_RESPONSE-1);


		XUartLite_Send(UartLiteInstPtr, ResponseBuffer_Data_Exchange_Diode, BYTE_SIZE_DATA_EXCHANGE_DIO_RESPONSE);
		Pmod_Wait_Send_Data(Pmod_Control, UartLiteInstPtr, BYTE_SIZE_DATA_EXCHANGE_DIO_RESPONSE);

		XGpio_DiscreteWrite(Pmod_Control.tx_enable_Ptr, 1, 0);
		XGpio_DiscreteWrite(Pmod_Control.rx_disnable_Ptr, 1, 0);

	}
}




/****************************************************************************/
/**
*
* This function used by data diode to write data to the shared memory.
*
* @param	HeaderBuffer is the pointer to header of packet.
*
* @param	sizeheader is size of header of packet.
*
* @param	DataBuffer is the pointer to data (payload) of packet.
*
* @param	MEM_BASE_ADDRESS is the pointer to the start of register on the shared memory used to save the data.
*
* @param	MUTEX_MEMORY_ID is the Mutex key ID for the location pointed by MEM_BASE_ADDRESS.
*
* @return	None.
*
* @note		None.
*
*****************************************************************************/
void Write_data_to_OCM(u8* HeaderBuffer, int sizeheader, u8* DataBuffer, int sizedata, u8* MEM_BASE_ADDRESS, int MUTEX_MEMORY_ID){

	MTX_Lock(MUTEX_MEMORY_ID);

	for(int i = 0; i< sizeheader; i++){
		MEM_BASE_ADDRESS[i] = HeaderBuffer[i];
	}
	for(int i = 0; i< sizedata; i++){
		MEM_BASE_ADDRESS[i + sizeheader] = DataBuffer[i];
	}
	MTX_Unlock(MUTEX_MEMORY_ID);

}



/****************************************************************************/
/**
*
* This function used to calculate the CRC check byte for a frame.
*
* @param	pi8uFrame_p is the pointer to the start of the frame packet.
*
* @param	i16uLen_p is the length of the frame packet.
*
* @return	None.
*
* @note		None.
*
*****************************************************************************/
u8 Crc_Cal(u8 * pi8uFrame_p, u16 i16uLen_p)
{
	u8 i8uRv_l = 0;

	while (i16uLen_p--) {
		i8uRv_l = i8uRv_l ^ pi8uFrame_p[i16uLen_p];
	}
	return i8uRv_l;
}

