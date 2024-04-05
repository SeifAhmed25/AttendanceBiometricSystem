/*
 * Relay_Interface.h
 *
 *  Created on: Apr 4, 2024
 *      Author: tarek
 */

#ifndef HAL_RELAY_RELAY_INTERFACE_H_
#define HAL_RELAY_RELAY_INTERFACE_H_
#include "../../MCAL/DIO/DIO_Interface.h"
#include "Relay_Config.h"

void Relay_Init(u8 Relay_ID);
void Relay_ON(u8 Relay_ID);
void Relay_OFF(u8 Relay_ID);
#endif /* HAL_RELAY_RELAY_INTERFACE_H_ */
