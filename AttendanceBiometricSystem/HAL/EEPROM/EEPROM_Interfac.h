/*
 * EEPROM_Interfac.h
 *
 * Created: 3/27/2024 5:31:23 PM
 *  Author: seif
 */ 


#ifndef EEPROM_INTERFAC_H_
#define EEPROM_INTERFAC_H_
#include "EEPROM_CONFIG.h"

#include "../../MCAL/I2C/I2C_Interface.h"
#include "../../LIB/STD.h"
#include "../../LIB/BitMath.h"
void H_Void_EEPROMWrite(u16,u8);
u8 H_Void_EEPROMRead (u16);


#endif /* EEPROM_INTERFAC_H_ */
