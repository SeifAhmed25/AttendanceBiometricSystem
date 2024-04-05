/*
 * Relay_Prog.c
 *
 *  Created on: Apr 4, 2024
 *      Author: tarek
 */

#include "Relay_Interface.h"

void Relay_Init(u8 Relay_ID){
	M_DIO_Void_SetPinDirection(Relay_ID,OUTPUT);
	M_DIO_Void_SetPinValue(Relay_ID,LOW);
}
void Relay_ON(u8 Relay_ID){
	M_DIO_Void_SetPinValue(Relay_ID,HIGH);
}
void Relay_OFF(u8 Relay_ID){
	M_DIO_Void_SetPinValue(Relay_ID,LOW);
}
