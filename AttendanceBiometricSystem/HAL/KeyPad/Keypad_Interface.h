/*
 * Keypad_Interface.h
 *
 *  Created on: ???/???/????
 *      Author: dell
 */

#ifndef HAL_KEYPAD_KEYPAD_INTERFACE_H_
#define HAL_KEYPAD_KEYPAD_INTERFACE_H_

/*============================================================================================*/
/*   INCLUDES   */
/*============================================================================================*/
#include "../../MCAL/DIO/DIO_Interface.h"
#include "F_CPU_Interface.h"

/*============================================================================================*/
/*   DEFINES   */
/*============================================================================================*/
#define KEYPAD_PRESSED                 0
#define KEYPAD_RELEASED                1

/*============================================================================================*/
/*   PROTOTYPES   */
/*============================================================================================*/
void H_KeyPad_Void_KeyPadInit(void);
u8   H_KeyPad_U8_KeyPadRead(void);

#endif /* HAL_KEYPAD_KEYPAD_INTERFACE_H_ */
