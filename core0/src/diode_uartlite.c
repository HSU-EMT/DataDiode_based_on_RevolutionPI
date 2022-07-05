/*
 * diode_uartlite.c
 *
 *  Created on: 27.06.2022
 *      Author: Ha
 */


#include "custom_definitions.h"
#include "xuartlite.h"

/*****************************************************************************/
/**
*
* This function is the handler which performs processing to send data to the
* UartLite. It is called from an interrupt context such that the amount of
* processing performed should be minimized. It is called when the transmit
* FIFO of the UartLite is empty and more data can be sent through the UartLite.
*
* This handler provides an example of how to handle data for the UartLite, but
* is application specific.
*
* @param	CallBackRef contains a callback reference from the driver.
*		In this case it is the instance pointer for the UartLite driver.
* @param	EventData contains the number of bytes sent or received for sent
*		and receive events.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
extern volatile int TotalSentCount;
void UartLiteSendHandler(void *CallBackRef, unsigned int EventData)
{
	TotalSentCount = EventData;
}

/****************************************************************************/
/**
*
* This function is the handler which performs processing to receive data from
* the UartLite. It is called from an interrupt context such that the amount of
* processing performed should be minimized. It is called when any data is
* present in the receive FIFO of the UartLite such that the data can be
* retrieved from the UartLite. The amount of data present in the FIFO is not
* known when this function is called.
*
* This handler provides an example of how to handle data for the UartLite, but
* is application specific.
*
* @param	CallBackRef contains a callback reference from the driver,
*		in this case it is the instance pointer for the UartLite driver.
* @param	EventData contains the number of bytes sent or received for sent
*		and receive events.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
extern volatile int TotalReceivedCount;
void UartLiteRecvHandler(void *CallBackRef, unsigned int EventData)
{
	TotalReceivedCount = EventData;
}

/****************************************************************************/
/**
*
* This function setups the interrupt system such that interrupts can occur
* for the UartLite. This function is application specific since the actual
* system may or may not have an interrupt controller. The UartLite could be
* directly connected to a processor without an interrupt controller. The
* user should modify this function to fit the application.
*
* @param	IntcInstancePtr is a pointer to the instance of INTC driver.
* @param	UartLiteInstPtr is a pointer to the instance of UartLite driver.
*		XPAR_<UARTLITE_instance>_DEVICE_ID value from xparameters.h.
* @param	UartLiteIntrId is the Interrupt ID and is typically
*		XPAR_<INTC_instance>_<UARTLITE_instance>_VEC_ID
*		value from xparameters.h.
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None.
*
****************************************************************************/
int UartLiteSetupIntrSystem(INTC *IntcInstancePtr,
				XUartLite *UartLiteInstPtr,
				u16 UartLiteIntrId0)
{
	int Status;

	XScuGic_Config *IntcConfig;

	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 */
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);


	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig,
					IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	XScuGic_SetPriorityTriggerType(IntcInstancePtr, UartLiteIntrId0,
					0xA0, 0x3);

	Status = XScuGic_Connect(IntcInstancePtr, UartLiteIntrId0,
				 (Xil_ExceptionHandler)XUartLite_InterruptHandler,
				 UartLiteInstPtr);
	if (Status != XST_SUCCESS) {
		return Status;
	}

	XScuGic_Enable(IntcInstancePtr, UartLiteIntrId0);

	/*
	 * Initialize the exception table.
	 */
	Xil_ExceptionInit();

	/*
	 * Register the interrupt controller handler with the exception table.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler)INTC_HANDLER,
			IntcInstancePtr);

	/*
	 * Enable exceptions.
	 */
	Xil_ExceptionEnable();

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function disables the interrupts that occur for the UartLite.
*
* @param	IntcInstancePtr is a pointer to the instance of the INTC driver.
* @param	UartLiteIntrId is the Interrupt ID and is typically
*		XPAR_<INTC_instance>_<UARTLITE_instance>_VEC_ID
*		value from xparameters.h.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void UartLiteDisableIntrSystem(INTC *IntcInstancePtr,
					  u16 UartLiteIntrId)
{

	/*
	 * Disconnect and disable the interrupt for the UartLite.
	 */
	XScuGic_Disable(IntcInstancePtr, UartLiteIntrId);
	XScuGic_Disconnect(IntcInstancePtr, UartLiteIntrId);

}
