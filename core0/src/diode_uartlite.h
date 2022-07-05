/*
 * diode_uartlite.h
 *
 *  Created on: 27.06.2022
 *      Author: Ha
 */

#ifndef __DIODE_UARTLITE_H_
#define __DIODE_UARTLITE_H_



/************************** Function Prototypes ******************************/
void UartLiteSendHandler(void *CallBackRef, unsigned int EventData);

void UartLiteRecvHandler(void *CallBackRef, unsigned int EventData);

int UartLiteSetupIntrSystem(INTC *IntcInstancePtr,
				XUartLite *UartLiteInstancePtr,
				u16 UartLiteIntrId0);

void UartLiteDisableIntrSystem(INTC *IntrInstancePtr,
				u16 UartLiteIntrId);



#endif /* __DIODE_UARTLITE_H_ */
