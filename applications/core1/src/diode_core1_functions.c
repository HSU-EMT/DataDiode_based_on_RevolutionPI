/*
 * diode_core1_functions.c
 *
 *  Created on: 29.06.2022
 *      Author: Ha
 */


#include <unistd.h>
#include "xil_types.h"
#include "xgpiops.h"
#include "xgpio.h"
#include "xuartlite.h"
#include "custom_definitions.h"
#include "mutex.h"
#include "shared_memory.h"
#include "diode_core1_functions.h"
#include "others.h"


static u8 ResponseBuffer_GetDeviceInfor[BYTE_SIZE_GET_DEVICE_INFOR_DIO_RESPONSE] = {0x00,0x00,
		0x0A,0x40,0x00,0x00,0x16,0x81,0x36,0x00,0x00,0x60,0x00,0x01,0x00,0x01,0x00,0x04,
		0x00,0xD1,0x30,0x00,0x00,0x46,0x00,0x12,0x00,0x02,0x00,0x00};
static u8 ResponseBuffer_SetAddress[BYTE_SIZE_SET_ADDRESS_DIO_RESPONSE] = {0x00, 0x00, 0x14, 0x40, 0x00, 0x00, 0x00, 0x00};
static u8 ResetSignal[BYTE_SIZE_RESET_SIGNAL] = {0x7F, 0x00, 0x7F};
static u8 Command, ReqRes, Device_address = 0;
static u8 Self_Data_Buffer[SELF_DATA_BUFFER_SIZE];
static u8 HeaderBuffer[BYTE_SIZE_HEADER];


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
			usleep(18000);
			XGpioPs_SetDirectionPin(Gpio_Ptr, JF_PMOD_CORE0_WIRE2, 1);
			XGpioPs_SetOutputEnablePin(Gpio_Ptr, JF_PMOD_CORE0_WIRE2, 1);
			XGpioPs_WritePin(Gpio_Ptr, JF_PMOD_CORE0_WIRE2, 1);
			Init_Config_status->Puls_detected = 1;
			xil_printf("program1: Puls_detected %d\r\n", Init_Config_status->Puls_detected);
			XGpioPs_SetDirectionPin(Gpio_Ptr, JF_PMOD_CORE0_WIRE1, 0);
		}
	}
}


/****************************************************************************/
/**
*
* This function used by the data diode to initialize all real devices connected to RevPi master in OT network.
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
void Init_real_device(Init_Config_s *Init_Config_status, XUartLite *UartLiteInstPtr, XGpioPs *Gpio_Ptr, PmodRS485_Control Pmod_Control){

	if (Init_Config_status->Puls_detected != 0){
		// take the number of real modules from the shared memory
		MTX_Lock(MUTEX_ID_MEM_DEVICE_INFOR);
		Init_Config_status->Device_count = MEM_DEVICE_NUM[0];
		MTX_Unlock(MUTEX_ID_MEM_DEVICE_INFOR);

		int Device_index = 0;
		int Index_Device_infor = 0;
		int Index_Address_set  = 0;

		if(Init_Config_status->Device_count != 0){
		 while(Init_Config_status->Device_count > Device_index){
			Reset_FIFO(UartLiteInstPtr);

			u8 HelpBuffer[BYTE_SIZE_TOTAL_INIT_REQ] = {0};
			XUartLite_Recv(UartLiteInstPtr, HelpBuffer, BYTE_SIZE_TOTAL_INIT_REQ);
			Pmod_Wait_Receive_Data(Pmod_Control, UartLiteInstPtr, BYTE_SIZE_TOTAL_INIT_REQ);

			if (HelpBuffer[0] == INIT_REQ_START_CODE){
				u8* Help_MEM_BASE = MEM_DEVICE_INFOR + 100*Device_index;
				int paket_length = (int)(Help_MEM_BASE[BYTE_SIZE_HEADER_INIT-1]) + BYTE_SIZE_HEADER_INIT + 1; // 1 byte more for CRC
				u8 device_infor_res[paket_length];
				MTX_Lock(MUTEX_ID_MEM_DEVICE_INFOR);
				for(int i = 0; i< paket_length; i++){
					device_infor_res[i] = Help_MEM_BASE[i];
				}
				MTX_Unlock(MUTEX_ID_MEM_DEVICE_INFOR);

				usleep(2000);
				XGpio_DiscreteWrite(Pmod_Control.tx_enable_Ptr, 1, 1);
				XGpio_DiscreteWrite(Pmod_Control.rx_disnable_Ptr, 1, 1);

				XUartLite_Send(UartLiteInstPtr, device_infor_res, paket_length);
				Pmod_Wait_Send_Data(Pmod_Control, UartLiteInstPtr, paket_length);

				XGpio_DiscreteWrite(Pmod_Control.tx_enable_Ptr, 1, 0);
				XGpio_DiscreteWrite(Pmod_Control.rx_disnable_Ptr, 1, 0);
				Index_Device_infor += 1;

			}
			else if ((HelpBuffer[0] != INIT_REQ_START_CODE) &&(HelpBuffer[2] == ADDRESS_SETTING_CODE)){

				ResponseBuffer_SetAddress[1] = (u8)(HelpBuffer[0] & ADDRESS_MASK);
				ResponseBuffer_SetAddress[BYTE_SIZE_SET_ADDRESS_DIO_RESPONSE-1] = Crc_Cal(ResponseBuffer_SetAddress, BYTE_SIZE_SET_ADDRESS_DIO_RESPONSE-1);

				usleep(2000);
				XGpio_DiscreteWrite(Pmod_Control.tx_enable_Ptr, 1, 1);
				XGpio_DiscreteWrite(Pmod_Control.rx_disnable_Ptr, 1, 1);

				XUartLite_Send(UartLiteInstPtr, ResponseBuffer_SetAddress, BYTE_SIZE_SET_ADDRESS_DIO_RESPONSE);
				Pmod_Wait_Send_Data(Pmod_Control, UartLiteInstPtr, BYTE_SIZE_SET_ADDRESS_DIO_RESPONSE);

				XGpio_DiscreteWrite(Pmod_Control.tx_enable_Ptr, 1, 0);
				XGpio_DiscreteWrite(Pmod_Control.rx_disnable_Ptr, 1, 0);
				Index_Address_set += 1;
			}

			if ((Index_Address_set == Index_Device_infor) && (Index_Address_set != 0)){
				Device_index += 1;
			}
		 }
		} else if (Init_Config_status->Device_count == 0){
			xil_printf("program 1: No real modules were connected\r\n");
		}
		xil_printf("program1: %d real modules were successfully emulated\r\n", Device_index);
		Init_Config_status->All_emulated = 1;
		}
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


	if ((Init_Config_status->All_emulated == 1)&&(Init_Config_status->Infor_exchanged == 0)){
		Reset_FIFO(UartLiteInstPtr);

		u8 RequestBuffer_GetDeviceInfor[BYTE_SIZE_GET_DEVICE_INFOR_DIO_REQUEST];
		XUartLite_Recv(UartLiteInstPtr, RequestBuffer_GetDeviceInfor, BYTE_SIZE_GET_DEVICE_INFOR_DIO_REQUEST);
		Pmod_Wait_Receive_Data(Pmod_Control, UartLiteInstPtr, BYTE_SIZE_GET_DEVICE_INFOR_DIO_REQUEST);

		if (RequestBuffer_GetDeviceInfor[0] == INIT_REQ_START_CODE){
			ResponseBuffer_GetDeviceInfor[BYTE_SIZE_GET_DEVICE_INFOR_DIO_RESPONSE-1] = Crc_Cal(ResponseBuffer_GetDeviceInfor, BYTE_SIZE_GET_DEVICE_INFOR_DIO_RESPONSE-1);

			usleep(2000);
			XGpio_DiscreteWrite(Pmod_Control.tx_enable_Ptr, 1, 1);
			XGpio_DiscreteWrite(Pmod_Control.rx_disnable_Ptr, 1, 1);
			XUartLite_Send(UartLiteInstPtr, ResponseBuffer_GetDeviceInfor, BYTE_SIZE_GET_DEVICE_INFOR_DIO_RESPONSE);
			Pmod_Wait_Send_Data(Pmod_Control, UartLiteInstPtr, BYTE_SIZE_GET_DEVICE_INFOR_DIO_RESPONSE);

			XGpio_DiscreteWrite(Pmod_Control.tx_enable_Ptr, 1, 0);
			XGpio_DiscreteWrite(Pmod_Control.rx_disnable_Ptr, 1, 0);

			xil_printf("program1: Device informations exchange for diode diode successes\r\n");
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
	if ((Init_Config_status->Infor_exchanged == 1) && (*DataDiode_address_Ptr == 0)){

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
		xil_printf("program1: Address setting for Data Diode successful\r\n");
	}
}


/****************************************************************************/
/**
*
* This function proceeds the main function of data diode in the IT network. The data diode reads data on the shared memory
* and writes it on the PiBridge to communicate with the RevPi master.
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
* 			CONFIG_FAILED: when the user configures the IT and OT system not identical on PiCtory application.
* 			SYSTEM_FAILED: when an error occurs during functionality of the data diode.
* 			otherwise SYSTEM_SUCCESS.
*
* @note		None.
*
*****************************************************************************/
int Read_OCM_Write_PiBridge(Init_Config_s *Init_Config_status, u8 DataDiode_address, XUartLite *UartLiteInstPtr, PmodRS485_Control Pmod_Control, int *error_count){
	int data_length = 0;
	int Config_packet_index = 0;

	MTX_Lock(MUTEX_ID_MEM_DEVICE_CONF_NUM);
	int Config_Paket_num = MEM_DEVICE_CONF_NUM[0];
	MTX_Unlock(MUTEX_ID_MEM_DEVICE_CONF_NUM);

	XUartLite_Recv(UartLiteInstPtr, HeaderBuffer, BYTE_SIZE_HEADER);
	Pmod_Wait_Receive_Data(Pmod_Control, UartLiteInstPtr, BYTE_SIZE_HEADER);


	if(HeaderBuffer[0] == BROADCAST_START_CODE){
		XUartLite_Recv(UartLiteInstPtr, Self_Data_Buffer, (BYTE_SIZE_BROADCAST-BYTE_SIZE_HEADER));
		Pmod_Wait_Receive_Data(Pmod_Control, UartLiteInstPtr, (BYTE_SIZE_BROADCAST-BYTE_SIZE_HEADER));

		xil_printf("program1: Broadcast signal from RevPi Master received\r\n");
		xil_printf("program1: Configuration & Cyclically data exchange phase started\r\n");
		xil_printf("program1: Normal Operation...\r\n");
	}
	else {

	data_length = (int)(HeaderBuffer[1] & DATA_LENGTH_MASK) + 1; // one more for CRC Check Byte at the end of frame
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
	 * Data diode copies configuration data of all other devices on the shared memory to configure with the RevPi Master
	 */
	if ((Device_address != DataDiode_address) && (Init_Config_status->All_configured == 0) && (Config_packet_index < Config_Paket_num)) {
		u8* MEM_BASE_HELP_REQ = MEM_DEVICE_CONF_REQ + 100*Config_packet_index;
		u8* MEM_BASE_HELP_RES = MEM_DEVICE_CONF_RES + 10*Config_packet_index;
		u8  Res_Help_buffer[BYTE_SIZE_CONFIG_RESPONSE];

		/*
		 * Data diode checks whether the system in LAN was configured identically to the system in WAN or not
		 */
		MTX_Lock(MUTEX_ID_MEM_DEVICE_CONF_REQ);
		for (int i = 0; i < sizeheader; i++){
			if (HeaderBuffer[i] != MEM_BASE_HELP_REQ[i]){
				MTX_Unlock(MUTEX_ID_MEM_DEVICE_CONF_REQ);
				return CONFIG_FAILED;
			}
		}
		for (int i = 0; i < sizedata; i++){
			if (DataBuffer[i] != MEM_BASE_HELP_REQ[i + sizeheader]){
				MTX_Unlock(MUTEX_ID_MEM_DEVICE_CONF_REQ);
				return CONFIG_FAILED;
			}
		}
		MTX_Unlock(MUTEX_ID_MEM_DEVICE_CONF_REQ);

		XGpio_DiscreteWrite(Pmod_Control.tx_enable_Ptr, 1, 1);
		XGpio_DiscreteWrite(Pmod_Control.rx_disnable_Ptr, 1, 1);
		usleep(100);

		MTX_Lock(MUTEX_ID_MEM_DEVICE_CONF_RES);
		for (int i = 0; i < BYTE_SIZE_CONFIG_RESPONSE; i++){
			Res_Help_buffer[i] = MEM_BASE_HELP_RES[i];
		}
		MTX_Unlock(MUTEX_ID_MEM_DEVICE_CONF_RES);


		XUartLite_Send(UartLiteInstPtr, Res_Help_buffer, BYTE_SIZE_CONFIG_RESPONSE);
		Pmod_Wait_Send_Data(Pmod_Control, UartLiteInstPtr, BYTE_SIZE_CONFIG_RESPONSE);

		XGpio_DiscreteWrite(Pmod_Control.tx_enable_Ptr, 1, 0);
		XGpio_DiscreteWrite(Pmod_Control.rx_disnable_Ptr, 1, 0);

		Config_packet_index += 1;


	}
	/*
	 * Data diode copies the data exchanged of all other real devices on the shared memory saved by
	 * program0 to response to the RevPi Master request packets in the cyclically data exchange phase
	 */
	else if ((Device_address != DataDiode_address) && (Init_Config_status->All_configured == 1)) {

			switch(Device_address)
			{
			case 0x1F:
				Response_to_master(UartLiteInstPtr, MEM_RES_1F, MUTEX_SHARED_MEMORY_ID_1F_RES, Pmod_Control);
				break;
			case 0x1E:
				Response_to_master(UartLiteInstPtr, MEM_RES_1E, MUTEX_SHARED_MEMORY_ID_1E_RES, Pmod_Control);
				break;
			case 0x1D:
				Response_to_master(UartLiteInstPtr, MEM_RES_1D, MUTEX_SHARED_MEMORY_ID_1D_RES, Pmod_Control);
				break;
			case 0x1C:
				Response_to_master(UartLiteInstPtr, MEM_RES_1C, MUTEX_SHARED_MEMORY_ID_1C_RES, Pmod_Control);
				break;
			case 0x1B:
				Response_to_master(UartLiteInstPtr, MEM_RES_1B, MUTEX_SHARED_MEMORY_ID_1B_RES, Pmod_Control);
				break;
			case 0x20:
				Response_to_master(UartLiteInstPtr, MEM_RES_20, MUTEX_SHARED_MEMORY_ID_20_RES, Pmod_Control);
				break;
			case 0x21:
				Response_to_master(UartLiteInstPtr, MEM_RES_21, MUTEX_SHARED_MEMORY_ID_21_RES, Pmod_Control);
				break;
			case 0x22:
				Response_to_master(UartLiteInstPtr, MEM_RES_22, MUTEX_SHARED_MEMORY_ID_22_RES, Pmod_Control);
				break;
			case 0x23:
				Response_to_master(UartLiteInstPtr, MEM_RES_23, MUTEX_SHARED_MEMORY_ID_23_RES, Pmod_Control);
				break;
			case 0x24:
				Response_to_master(UartLiteInstPtr, MEM_RES_24, MUTEX_SHARED_MEMORY_ID_24_RES, Pmod_Control);
				break;
			default:
				xil_printf("Error: program1 reads wrong request packet\r\n");
				*error_count += 1;
				return SYSTEM_FAILED;
			}

		}
	/*
	 * Data diode emulates itself and does its configuration phase and its cyclically data exchange phase
	 */
	else if (Device_address == DataDiode_address) {
		DataDiode_Response_to_master(DataDiode_address, Command, UartLiteInstPtr, Pmod_Control);
		if(!Init_Config_status->All_configured) Init_Config_status->All_configured = 1;
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

	if   (Command == IOP_TYP1_CMD_CFG) { //configuration phase command

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
	else if (Command == IOP_TYP1_CMD_DATA){ //cyclically data exchange phase command

		XGpio_DiscreteWrite(Pmod_Control.tx_enable_Ptr, 1, 1);
		XGpio_DiscreteWrite(Pmod_Control.rx_disnable_Ptr, 1, 1);
		usleep(100);

		u8 ResponseBuffer_Data_Exchange_Diode[BYTE_SIZE_DATA_EXCHANGE_DIO_RESPONSE] = {0};
		ResponseBuffer_Data_Exchange_Diode[0] = DataDiode_address ^ RESPONSE_MASK;
		ResponseBuffer_Data_Exchange_Diode[1] = BYTE_SIZE_NUTZ_DATA_EXCHANGE_DIO_RESPONSE ^ (IOP_TYP1_CMD_DATA2 << 5);

		Write_Real_Request_to_Diode(0x1F, MEM_REQ_1F, MUTEX_SHARED_MEMORY_ID_1F_REQ, ResponseBuffer_Data_Exchange_Diode);
		Write_Real_Request_to_Diode(0x1E, MEM_REQ_1E, MUTEX_SHARED_MEMORY_ID_1E_REQ, ResponseBuffer_Data_Exchange_Diode);
		Write_Real_Request_to_Diode(0x1D, MEM_REQ_1D, MUTEX_SHARED_MEMORY_ID_1D_REQ, ResponseBuffer_Data_Exchange_Diode);
		Write_Real_Request_to_Diode(0x1C, MEM_REQ_1C, MUTEX_SHARED_MEMORY_ID_1C_REQ, ResponseBuffer_Data_Exchange_Diode);

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
* This function used by data diode to reads the data on the shared memory of all real devices to response to RevPi master
* in the cyclically data exchange phase of devices to emulate the real devices.
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
void Response_to_master(XUartLite *UartLiteInstPtr, u8* MEM_BASE_ADDRESS, int MUTEX_MEMORY_ID, PmodRS485_Control Pmod_Control){

	int res_packet_length;
	MTX_Lock(MUTEX_MEMORY_ID);
	res_packet_length = (int)(MEM_BASE_ADDRESS[1] & DATA_LENGTH_MASK) + BYTE_SIZE_HEADER + 1; // 1 byte more for CRC
	u8 res_buffer[res_packet_length];

	for(int i = 0; i< res_packet_length; i++){
		res_buffer[i] = MEM_BASE_ADDRESS[i];
	}
	MTX_Unlock(MUTEX_MEMORY_ID);


	XGpio_DiscreteWrite(Pmod_Control.tx_enable_Ptr, 1, 1);
	XGpio_DiscreteWrite(Pmod_Control.rx_disnable_Ptr, 1, 1);
	usleep(50);

	XUartLite_Send(UartLiteInstPtr, res_buffer, res_packet_length);
	Pmod_Wait_Send_Data(Pmod_Control, UartLiteInstPtr, res_packet_length);

	XGpio_DiscreteWrite(Pmod_Control.tx_enable_Ptr, 1, 0);
	XGpio_DiscreteWrite(Pmod_Control.rx_disnable_Ptr, 1, 0);
}


/****************************************************************************/
/**
*
* This function used by data diode to write data to the shared memory.
*
* @param	DataDiode_address is the address of the data diode.
*
* @param	MEM_BASE_ADDRESS is the pointer to the start of register on the shared memory used to save the data.
*
* @param	MUTEX_MEMORY_ID is the Mutex key ID for the location pointed by MEM_BASE_ADDRESS.
*
* @param    ResponseBuffer_Data_Exchange_Diode is the pointer to the response packet of the data diode.
* On this packet, the payloads from request packet of all real devices will be located.
*
* @return	None.
*
* @note		None.
*
*****************************************************************************/
void Write_Real_Request_to_Diode(u8 Device_address, u8* MEM_BASE_ADDRESS, int MUTEX_MEMORY_ID, u8* ResponseBuffer_Data_Exchange_Diode){

	int Device_index;
	int Base_index;

	Device_index = (int)(Device_address) - (int)(SMALLEST_ADDRESS); // The smallest address can (but not must) be the data diode address.
																	// Another real module can never take the SMALLEST_ADDRESS value as its address.
																	// Because the data diode must be at the end of RevPi chain. So, the minimal
																	//value of Device_index is 1
	Base_index = BYTE_SIZE_DATA_EXCHANGE_DIO_RESPONSE - Device_index*BYTE_SIZE_WRITE_REQ_STEP - 1 - 8; // minus 1 because don't count 1 CRC Byte at the end of frame,
																			    					   //minus 8 because don't use last 2 counter channel. Reserve for future use

	int data_length = (int)(MEM_BASE_ADDRESS[1] & DATA_LENGTH_MASK); // no Header, no CRC

	MTX_Lock(MUTEX_MEMORY_ID);
	for(int i = 0; i < data_length; i++){
		ResponseBuffer_Data_Exchange_Diode[Base_index + i] = MEM_BASE_ADDRESS[i + BYTE_SIZE_HEADER];
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
