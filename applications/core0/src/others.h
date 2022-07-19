/*
 * others.h
 *
 *  Created on: 28.06.2022
 *      Author: Ha
 */

#ifndef SRC_OTHERS_H_
#define SRC_OTHERS_H_

void Reset_Variables();
void Reset(int *val);
int Pmod_Wait_Receive_Data(PmodRS485_Control Pmod_Control, XUartLite *UartLiteInstPtr, unsigned int byte_num);
int Pmod_Wait_Send_Data(PmodRS485_Control Pmod_Control, XUartLite *UartLiteInstPtr, unsigned int byte_num);
void Reset_FIFO(XUartLite *UartLiteInstPtr);

#endif /* SRC_OTHERS_H_ */
