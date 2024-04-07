/*
 * FingerPrint_Interface.h
 *
 *  Created on: Mar 22, 2024
 *      Author: tarek
 */

#ifndef FINGERPRINT_INTERFACE_H_
#define FINGERPRINT_INTERFACE_H_

#include "FingerPrint_Config.h"
#include "UART_Interface.h"
extern u8 AckPack [12] ;
void FingerP_send(u8,u16);
void FingerPS_handShake(void);
void FingerPS_genImg(void);
void FingerPS_convertImg1CharFile();
void FingerPS_convertImg2CharFile(void);
void FingerPS_AuraLedConfig(void); 
void FingerPS_genTemplate(void);
void FingerPS_strTemplate(u16);
void FingerPS_searchFinger(void);
void FingerPS_emptyLibrary(void);
void FingerPS_deleteFinger(void);
void FingerPS_LoadCharFile(u16);
void FingerPS_calcCheckSum(void);
void FingerPS_match(void);
void StoreAckBytes (u8);
u8 FingerPS_CheckAck ();
void FingerPS_SetNewFingerPrint (u16);
u8 FingerPS_CheckOneToOneMatch(u16);
#endif /* FINGERPRINT_INTERFACE_H_ */
