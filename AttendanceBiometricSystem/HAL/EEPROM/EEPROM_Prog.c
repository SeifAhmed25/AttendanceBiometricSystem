/*
 * EEPROM_Prog.c
 *
 * Created: 3/27/2024 5:29:15 PM
 *  Author: seif
 */ 
#include "EEPROM_Interfac.h"
void H_Void_EEPROMWrite(u16 address, u8 data){
	/*Address to Write = 0011 1010 1111*/  
	/*				   & 0011 0000 0000*/
	/*		0xA0	   |      1010 0000*/
	M_U8_I2C_Start();  
	/*SEND 1010 + E A9 A8 + W*/
	M_U8_I2C_SendSlaveAddressWithWrite((u8)(0xA0 | ((address & 0x300) >> 7))); 
	/*Complete Address to write data in*/
	M_U8_I2C_Write((u8) address); 
	M_U8_I2C_Write(data); 
	M_Void_I2C_STOP(); 
} 
u8 H_Void_EEPROMRead (u16 address){ 
	u8 data;
	/*Write Address = 0011 1010 1111*/
	/*              & 0011 0000 0000*/
	/*              |      1010 0000*/
	M_U8_I2C_Start(); 
	/*SEND 1010 + E A9 A8 + W*/
	M_U8_I2C_SendSlaveAddressWithWrite((u8)((0xA0) | ((address & 0x300) >> 7)));
	/*Complete address to read from*/ 
	M_U8_I2C_Write((u8) address);  
	M_U8_I2C_Repeated_Start();  
	M_U8_I2C_SendSlaveAddressWithRead((u8)(0xA0 | ((address & 0x300) >> 7) | 0x01)); 
	data = M_U8_I2C_ReadWithNack(); 
	M_Void_I2C_STOP(); 
	return data; 
}