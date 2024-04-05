/*******************************************************************************************
 *
 * Module: RTC (Real Time Clock)
 *
 * File Name: RTC_Inter.h
 *
 * Description: Interface header file for the RTC driver
 *
 * Author: Youssef Abdelaziz

 *
 * *******************************************************************************************/

#ifndef RTC_INTER_H_
#define RTC_INTER_H_
#include "../../MCAL/I2C/I2C_Interface.h"
#include "RTC_Config.h"
#include "RTC_Private.h"

#define RTC_SLAVE_ADDRESS_WRITE  0xD0
#define RTC_SLAVE_ADDRESS_READ	 0xD1

#define _24_Mode	0
#define _12_Mode	1

#define AM			0
#define PM			1

extern u8 RTC_Day_str_display[7][4];

typedef enum
{
	_1_HZ,_4point096KHZ,_8point192KHZ,_32point768KHZ
}RATE_SELECT;


typedef struct {
	 u8 sec ;
	 u8 min ;
	 u8 hours ;
	 u8 day ;  /*01>>07*/
	 u8 date; /*01>>31*/
	 u8 month;
	 u8 year;
}Date_And_Time;


void RTC_Init();
void RTC_Set_Time(Date_And_Time *Config_Date);
void RTC_Set_Date(Date_And_Time *Config_Date);
void RTC_Get_Time(Date_And_Time *Get_Time_And_Date);
void RTC_Get_Date(Date_And_Time *Get_Time_And_Date);

#endif /* RTC_INTER_H_ */
