/******************************************************************************
 *
 * Project: Biometric Attendance System for companies
 *
 * File Name: Biometric_Attendance_Interface.h
 *
 * Description:	This project aims mainly to check the attendance of employees in companies
 * 				on time , there is two general cases:
 * 				1- A new employee shall set up his ID and FingerPrint for the first time.
 * 				2- An employee that has already set up his ID and fingerPrint and need to
 * 				   confirm his attendance on a daily basis
 *
 * Author: Group Effort (Youssef Abdelaziz - Seif Ahmed - Tarek Gamal)
 *
 *******************************************************************************/

#ifndef APP_BIOMETRIC_ATTENDANCE_INTERFACE_H_
#define APP_BIOMETRIC_ATTENDANCE_INTERFACE_H_

// #include "F_CPU_Interface.h"
#include "../LIB/STD.h"
#include "Biometric_Attendance_Types.h" 
#include "RTC_Inter.h" 
#include "F_CPU_Interface.h"

/*the ASCII values according to the Keypad buttons */
#define CONFIRM_ATTENDANCE_BUTTON_PRESSED 	'*'
#define NEW_ENROLLEMENT_BUTTON_PRESSED 		'#'
#define ENTER_BUTTON_PRESSED 				'D'
#define GOBACK_BUTTON_PRESSED 				'A'
#define MAIN_MENU_BUTTON_PRESSED			'C'
#define MIN_DIGIT							'0'
#define MAX_DIGIT							'9'
/*These values represent the allowed range of ID and the Go back button*/
#define MIN_ALLOWED_ID     					 1
#define MAX_ALLOWED_ID     					 15
#define GOBACK								0xFF
/***************************************************************
 * 					global Variable Prototypes				   *
 ***************************************************************/
extern APP_CurrentState g_SystemState;

/*****************************************************************************
 *
 * 							Functions*Prototypes
 *
 ******************************************************************************/

/**********************************************************************
*  syntax :  APP_Init(void);
*  Description : this function displays the user options
*  					* press '*' Attendance Confirmation
		 	 	 	* press '#' New Enrollment *.
*  Synch/Asynch     : Synchronous
*  Reentrance       : Non-Reentrant
*  parameters (in)  : None
*  parameters (out) : None
*  Return Value     : NONE
*************************************************************************/
void APP_Init(void);
/**********************************************************************
*  syntax :  APP_GetID(void);
*  Description : Return the value of specific ID of the employee.
				This value is either chosen for first enrollment
				or for attendance confirmation on daily basis.
*  Synch/Asynch     : Synchronouse
*  Reentrance       : Non-Reentrant
*  parameters (in)  : None
*  parameters (out) : APP_EmpID ID value
*  Return Value     : from 0x01 to 0x0F
*************************************************************************/
APP_EmpID APP_GetID(void);


/**********************************************************************
*  syntax :  APP_SetNewID(APP_EmpID copy_ID);
*  Description : IF a given APP_EmpID value is found into the EEPROM return  CONFIRM_ATTENDANCE_FOUND_ID
				 Else if not found in the EEPROM return  CONFIRM_ATTENDANCE_NOTFOUND_ID
*  Synch/Asynch     : Synchronous
*  Reentrance       : Non-Reentrant
*  parameters (in)  : APP_EmpID copy_ID
*  parameters (out) : None
*  Return Value     : CONFIRM_ATTENDANCE_NOTFOUND_ID
*  					  CONFIRM_ATTENDANCE_FOUND_ID
*************************************************************************/

u8 APP_CheckIDPresence ( APP_EmpID copy_ID);

/**********************************************************************
*  syntax :  APP_SetNewID(APP_EmpID copy_ID);
*  Description : This function shall call the EEPROM to save the  APP_EmpID Value
*  				 into the Address of the same value of  APP_EmpID.
				 If the Address of the same value of  APP_EmpID is already
				 written with the value of  APP_EmpID that means this ID
				 is Already taken by another employee.
*  Synch/Asynch     : Synchronouse
*  Reentrance       : Non-Reentrant
*  parameters (in)  : APP_EmpID copy_ID
*  parameters (out) : None
*  Return Value     : None
*************************************************************************/
void APP_SetNewID(APP_EmpID copy_ID);

/**********************************************************************
*  syntax :  APP_NewEnrollemtSuccessful();
*  Description : This function shall call the LCD to show that the new enrollment has been done successfully.
*  Synch/Asynch     : Synchronous
*  Reentrance       : Reentrant
*  parameters (in)  : None
*  parameters (out) : None
*  Return Value     : None
*************************************************************************/
void APP_NewEnrollemtSuccessful(void);

/**********************************************************************
*  syntax :  APP_RTC_Display_Time_and_Date(Date_And_Time*);
*  Description : This function shall call the LCD to show the time and date.
*  Synch/Asynch     : Synchronous
*  Reentrance       : Reentrant
*  parameters (in)  : Pointer to struct of date and time represents current time
*  parameters (out) : None
*  Return Value     : None
*************************************************************************/

void  APP_RTC_Display_Time_and_Date(Date_And_Time* ptr);
/**********************************************************************
*  syntax :  APP_ConfirmAttendanceSuccessful();
*  Description : This function shall call the LCD to show that the confirmation of attendance has been done successfully.
*  Synch/Asynch     : Synchronous
*  Reentrance       : Reentrant
*  parameters (in)  : None
*  parameters (out) : None
*  Return Value     : None
*************************************************************************/
void APP_Confirm_Attendance_Success(APP_EmpID copy_ID, Date_And_Time* CurrentDateAndTime_ptr);

/**********************************************************************
*  syntax :  APP_PutFingerToScan();
*  Description : This function shall call the LCD to tell the employee to put his finger to scan.
*  Synch/Asynch     : Synchronous
*  Reentrance       : Reentrant
*  parameters (in)  : None
*  parameters (out) : None
*  Return Value     : None
*************************************************************************/
void APP_PutFingerToScan(void);

/**********************************************************************
*  syntax :  APP_WarningHandler(warnings);
*  Description : This function shall call the LCD to show the warning according to the input describing the current error.
*  Synch/Asynch     : Synchronous
*  Reentrance       : Reentrant
*  parameters (in)  : WarningType
*  parameters (out) : None
*  Return Value     : user_action (GOBACK_BUTTON_PRESSED or MAIN_MENU_BUTTON_PRESSED)
*************************************************************************/
u8 APP_WarningHandler(APP_Warnings warningType);



#endif /* APP_BIOMETRIC_ATTENDANCE_INTERFACE_H_ */
