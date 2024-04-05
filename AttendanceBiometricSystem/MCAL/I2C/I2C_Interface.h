/*
 * I2C_Interface.h
 *
 * Created: 3/25/2024 10:02:25 PM
 *  Author: seif
 */ 


#ifndef I2C_INTERFACE_H_
#define I2C_INTERFACE_H_
#include "../../LIB/STD.h"
#include "../../LIB/BitMath.h"
#include "I2C_Config.h"
void M_Void_I2C_Init (void); 
u8 M_U8_I2C_Start(void); 
u8 M_U8_I2C_SendSlaveAddressWithWrite (u8);
u8 M_U8_I2C_Repeated_Start(void); 
u8 M_U8_I2C_SendSlaveAddressWithRead(u8); 
u8 M_U8_I2C_Write(u8);
u8 M_U8_I2C_ReadWithAck(void); 
u8 M_U8_I2C_ReadWithNack(void); 
void M_Void_I2C_STOP(void);
#endif /* I2C_INTERFACE_H_ */
