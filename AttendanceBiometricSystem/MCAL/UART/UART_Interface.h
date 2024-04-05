/*
 * UART_Interface.h
 *
 *  Created on: Mar 19, 2024
 *      Author: tarek
 */

#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_

#include "STD.h"
#include "BitMath.h"
#include "UART_Config.h"
#include "avr/interrupt.h"

void UART_Init(void);
void UART_sendByte(u8);
void UART_sendString(u8*);
u8 UART_receiveByteSynch(void);
void UART_receiveByteAsynchCallBack(void(*)(u8));

#endif /* UART_INTERFACE_H_ */
