/*
 * main_core1.c
 *
 *  Created on: 19.01.2022
 *      Author: Ha
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>


#include "xparameters.h"
#include "xuartlite.h"
#include "xil_exception.h"
#include "xgpio.h"
#include "xgpiops.h"
#include "xstatus.h"
#include "xil_printf.h"
#include "xscugic.h"
#include "xil_types.h"


#include "custom_definitions.h"
#include "default_types.h"
#include "platform.h"
#include "mutex.h"
#include "shared_memory.h"
#include "diode_core1_functions.h"
#include "diode_uartlite.h"
#include "others.h"



/************************** GLOBAL VARIABLES ******************************/

int Status, error_count = 0;
int Normal_operation = 1;
u8 DataDiode_address = 0;

INTC IntcInstance;	      /* The instance of the Interrupt Controller */
XUartLite UartLiteInst;
XGpioPs Gpio, *Gpio_Ptr = &Gpio;	//The driver instance for GPIO Device
XGpioPs_Config* ConfigPtr;
XGpio   rx_disnable, tx_enable;

/*
 * Set up some control values for Pmod RS-485
 */
volatile int TotalSentCount;
volatile int TotalReceivedCount;
u32 sniff1_check, sniff2_check;
PmodRS485_Control Pmod_Control ={&TotalSentCount, &TotalReceivedCount, &rx_disnable, &tx_enable};

/*
 * Set up some status values for initialization phase and configuration phase on PiBridge
 */
Init_Config_s Init_Config_status = {0};

/*
 * Custom error message was used because not all errors are
 * included by xstatus.h. Define more on default_types.h
 * for your own purposes.
 */
HRESULT hr = SYSTEM_SUCCESS;


int main()
{
	/*************************** INITIALIZE PLATFORM ******************************/
	init_platform();
	PlatformDisableOcmCache();

    if(FAILED(hr = MTX_Init()))
    {
    	print("Error: Initialize Mutex 0 failed!\r\n");
    	return SYSTEM_FAILED;
    }

	/************************** INITIALIZE GPIO PINS ******************************/


	/* Initialize the GPIOPS driver. */
	ConfigPtr = XGpioPs_LookupConfig(XPAR_XGPIOPS_0_DEVICE_ID);
	Status = XGpioPs_CfgInitialize(Gpio_Ptr, ConfigPtr,
	ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		xil_printf("Error: Initialize MIO GPIO 0 failed!\r\n");
	return XST_FAILURE;
	}
	/*
	 * Setting the GPIO pins for sniff wire 1 and sniff wire 2.
	 * value of JF_PMOD_CORE0_WIRE1 and JF_PMOD_CORE0_WIRE2 can
	 * be changed depending on which pin the user have choose
	 * on Zybo board
	 */
	XGpioPs_SetDirectionPin(Gpio_Ptr, JF_PMOD_CORE0_WIRE1, 0);
	XGpioPs_SetDirectionPin(Gpio_Ptr, JF_PMOD_CORE0_WIRE2, 1);
	XGpioPs_SetOutputEnablePin(Gpio_Ptr, JF_PMOD_CORE0_WIRE2, 1);
	XGpioPs_WritePin(Gpio_Ptr, JF_PMOD_CORE0_WIRE2, 0);
	XGpioPs_SetDirectionPin(Gpio_Ptr, JF_PMOD_CORE0_WIRE2, 0);

	/*
	 * Initialize the GPIO driver for Tx_enable and Rx_disnable
	 * pin for RS-485 Pmod. By default, Tx channel is deactivate
	 * and Rx channel is active. XPAR_GPIO_3_DEVICE_ID and
	 * XPAR_GPIO_4_DEVICE_ID are depending on which GPIO pins
	 * were choose for hardware design in VIVADO. See README
	 * for more details.
	 */
	XGpio_Initialize(Pmod_Control.rx_disnable_Ptr, XPAR_GPIO_2_DEVICE_ID);
	XGpio_Initialize(Pmod_Control.tx_enable_Ptr, XPAR_GPIO_3_DEVICE_ID);
	XGpio_DiscreteWrite(Pmod_Control.rx_disnable_Ptr, 1, 0);
	XGpio_DiscreteWrite(Pmod_Control.tx_enable_Ptr, 1, 0);


	/************************** INITIALIZE UART DRIVER ******************************/

	Status = XUartLite_Initialize(&UartLiteInst, UARTLITE_DEVICE_1);
	if (Status != XST_SUCCESS) {
		xil_printf("Error: Initialize UartLite 1 failed\r\n");
		return XST_FAILURE;
	}

	/*
	 * Perform a self-test to ensure that the hardware was built correctly.
	 */
	Status = XUartLite_SelfTest(&UartLiteInst);
	if (Status != XST_SUCCESS) {
		xil_printf("Error: self-test UartLite 1 failed\r\n");
		return XST_FAILURE;
	}
	/*
	* Connect the UartLite to the interrupt subsystem such that interrupts
	* can occur.
	*/
	Status = UartLiteSetupIntrSystem(&IntcInstance, &UartLiteInst, UARTLITE_IRPT_INTR_1);
	if (Status != XST_SUCCESS) {
		xil_printf("Error: Connect the UartLite 1 to the interrupt subsystem failed\r\n");
		return XST_FAILURE;
	}

	/*
	 * Setup the handlers for the UartLite that will be called from the
	 * interrupt context when data has been sent and received,
	 * specify a pointer to the UartLite driver instance as the callback
	 * reference so the handlers are able to access the instance data.
	 */
	XUartLite_SetSendHandler(&UartLiteInst, UartLiteSendHandler, &UartLiteInst);
	XUartLite_SetRecvHandler(&UartLiteInst, UartLiteRecvHandler, &UartLiteInst);

	/*
	 * Enable the interrupt of the UartLite so that the interrupts
	 * will occur.
	 */
	XUartLite_EnableInterrupt(&UartLiteInst);

	xil_printf("Initialization of drivers has succeeded. program1 is ready to communicate with RevPi\r\n");



	/************************** COMMUNICATE ON PIBRIDGE ******************************/

	while(Normal_operation){
		/*
		 * Wait until the pulse occurred. The pulse is created by RevPi Master
		 * module to inform the Slaves about the start of initialization phase
		 */
		while(!Init_Config_status.Puls_detected){
			sniff1_check = XGpioPs_ReadPin(Gpio_Ptr, JF_PMOD_CORE0_WIRE1);
			sniff2_check = XGpioPs_ReadPin(Gpio_Ptr, JF_PMOD_CORE0_WIRE2);
			Puls_detect(Gpio_Ptr, sniff1_check, sniff2_check, &Init_Config_status);
		}

		/*
		 * After the pulse have occurred. The initialization phase on PiBridge
		 * will be started. The Data Diode emulates all devices in OT and uses
		 * data saved on the shared memory by program0 to initialize with RevPi Master
		 */
		Init_real_device(&Init_Config_status, &UartLiteInst, Gpio_Ptr, Pmod_Control);

		/*
		 * All other real devices were successfully initialized. Next step is the initialization
		 * for the data diode, which contains 2 steps: Informations about device exchange and
		 * address for data diode setting
		 */
		DataDiode_Infor_Exchange(&Init_Config_status, &UartLiteInst, Gpio_Ptr, Pmod_Control);
		DataDiode_Adress_Setting(&Init_Config_status, &DataDiode_address, &UartLiteInst, Gpio_Ptr, Pmod_Control);

		/*
		 * After the pulse have occurred and the initialization phase is done,
		 * the configuration phase and the cyclically data exchange starts in
		 * function Read_OCM_Write_PiBridge().
		 */

		while(Init_Config_status.Puls_detected){
			Status = Read_OCM_Write_PiBridge(&Init_Config_status, DataDiode_address, &UartLiteInst, Pmod_Control, &error_count);

			/*
			 * Handle with return value of the function Read_PiBridge_Write_OCM().
			 */
			if (Status == CONFIG_FAILED) {
				xil_printf("program 1: Configuration failed. The system in LAN must be identical the system in WAN: Exit");
				Reset(&Init_Config_status.Puls_detected);
				Reset(&Normal_operation);
			}
			else if (Status == SYSTEM_RESET) {
				xil_printf("program1: IT system reset\r\n");
				Reset_Variables();
			}
			else if ((Status == SYSTEM_FAILED)&&(error_count <= MAX_TRY_IF_ERROR)) {
				xil_printf("program1: IT system error: Trying to join the data exchange phase again\r\n");
				(&UartLiteInst)->ReceiveBuffer.RequestedBytes = 0;
				*Pmod_Control.TotalReceivedCount_Ptr = 0;
				Reset_FIFO(&UartLiteInst);

			}
			else if ((Status == SYSTEM_FAILED)&&(error_count > MAX_TRY_IF_ERROR)) {
				xil_printf("program1: Too much system error: Exit\r\n");
				Reset(&Init_Config_status.Puls_detected);
				Reset(&Normal_operation);

			}
			else if ((Status == SYSTEM_SUCCESS)&&(error_count != 0)){
				error_count = 0;
			}
		}
	 }

	UartLiteDisableIntrSystem(&IntcInstance, UARTLITE_IRPT_INTR_1);
	cleanup_platform();
    return 0;

}



