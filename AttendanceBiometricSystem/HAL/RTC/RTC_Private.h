/*******************************************************************************************
 *
 * Module: RTC (Real Time Clock)
 *
 * File Name: RTC_Private.h
 *
 * Description: Private header file for the RTC driver
 *
 * Author: Youssef Abdelaziz

 *
 * *******************************************************************************************/


#ifndef RTC_PRIVATE_H_
#define RTC_PRIVATE_H_

#define RTC_SECONDS_REG_ADDRESS		0X00
#define RTC_MINUTS_REG_ADDRESS		0X01
#define RTC_HOURS_REG_ADDRESS		0X02
#define RTC_DAYS_REG_ADDRESS		0X03
#define RTC_DATE_REG_ADDRESS		0X04
#define RTC_MONTH_REG_ADDRESS		0X05
#define RTC_YEAR_REG_ADDRESS		0X06
#define RTC_CONTROL_REG_ADDRESS		0X07

/*Control register pins*/
#define RTC_OUT_PIN					7
#define RTC_SQWE_PIN				4
#define RTC_RS1_PIN					1
#define RTC_RS0_PIN					0


#endif /* RTC_PRIVATE_H_ */
