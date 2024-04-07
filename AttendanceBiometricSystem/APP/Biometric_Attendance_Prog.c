/******************************************************************************
 *
 * Project: Biometric Attendance System for companies
 *
 * File Name: Biometric_Attendance_Prog.c
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
#include "Biometric_Attendance_Interface.h"
#include "../HAL/KeyPad/Keypad_Interface.h"
#include "../HAL/LCD/LCD_Interface.h"
#include "../HAL/LED/LED_Interface.h"
#include "../HAL/Relay/Relay_Interface.h"
#include "../HAL/RTC/RTC_Inter.h"
#include "EEPROM_Interfac.h"

#define TAREK_ID 7
#define YOUSSEF_ID 6
#define SEIF_ID 5

#define DISPLAY_FOR_5_SEC			5000
/*****************************************************************************
 * 							Functions*Definition
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
void APP_Init(void)
{
	u8 key = KEYPAD_RELEASED;
	H_Lcd_Void_LCDClear();
	H_Lcd_Void_LCDGoTo(0,0);
	H_Lcd_Void_LCDWriteString((u8*)"Please Press");
	H_Lcd_Void_LCDGoTo(1,0);
	H_Lcd_Void_LCDWriteString((u8*)"(*) : Attendance");
	H_Lcd_Void_LCDGoTo(2,0);
	H_Lcd_Void_LCDWriteString((u8*)"(#) : New Employee");
	
 	while ((key != (NEW_ENROLLEMENT_BUTTON_PRESSED)) && (key != (CONFIRM_ATTENDANCE_BUTTON_PRESSED))){
 		key = H_KeyPad_U8_KeyPadRead();
 	}
	switch(key)
	{
	case CONFIRM_ATTENDANCE_BUTTON_PRESSED:
	g_SystemState = CONFIRM_ATTENDANCE_ENTER_ID;
	break;
	case NEW_ENROLLEMENT_BUTTON_PRESSED:
	g_SystemState = NEW_ENROLLMENT_SET_NEW_ID;
	break;
	default:
	break;
	}
}


/**********************************************************************
*  syntax :  APP_GetID(void);
*  Description : Return the value of specific ID of the employee.
				This value is either chosen for first enrollment
				or for attendance confirmation on daily basis.
*  Synch/Asynch     : Synchronous
*  Reentrance       : Non-Reentrant
*  parameters (in)  : None
*  parameters (out) : APP_EmpID ID value
*  Return Value     : from 0x01 to 0x0F
*************************************************************************/
APP_EmpID APP_GetID(void)
{
	APP_EmpID Copy_ID =0;
	u8 Temp_keyPressed =0;

	/*go to the first place in the second line */
	H_Lcd_Void_LCDClear();
	H_Lcd_Void_LCDGoTo(0,0);
	H_Lcd_Void_LCDWriteString((u8*)"Please Enter Your ID");
	H_Lcd_Void_LCDGoTo(1,0);

	/*this while loop keeps taking input buttons from user via Keypad
	 *until user press ENTER button and convert the input uint8 char into u8 integer value*/
	do
	{
		Temp_keyPressed = H_KeyPad_U8_KeyPadRead();/*get char reading from user via keypad*/
		if(GOBACK_BUTTON_PRESSED == Temp_keyPressed || MAIN_MENU_BUTTON_PRESSED == Temp_keyPressed)
		{	/*if the user pressed the go-back button in the middle of the operation
		 	  Ignore the inputs by overwriting the Copy_ID by 0xFF and break the loop*/
			Copy_ID = GOBACK;
			break;
		}
		if(((Temp_keyPressed >= MIN_DIGIT) && (Temp_keyPressed <= MAX_DIGIT)))
		{
			/*convert the ASCII to decimal char by char BY -THE ZERO ASCII (48) from the input*/
			Temp_keyPressed-=48;

			H_Lcd_Void_LCDWriteNumber(Temp_keyPressed);/*display the number on the LCD for User interface*/

			Copy_ID = ((Copy_ID * 10 ) + Temp_keyPressed) ;/*convert ID into u8 integer value*/
			if(Copy_ID>MAX_ALLOWED_ID) Copy_ID = OUT_OF_RANGE_ID;
		}
	} while(ENTER_BUTTON_PRESSED != Temp_keyPressed);
	
	return Copy_ID;/*return the ID u8 integer value*/
}


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

APP_CurrentState APP_CheckIDPresence ( APP_EmpID copy_ID)
{
	/*check if the ID value is saved in the EEPROM
	 * or not and return the state of presence */
	APP_CurrentState Local_Is_found = 0;
	if(copy_ID == H_Void_EEPROMRead(copy_ID))
	{
		Local_Is_found = FOUND_ID;
	}
	else
	{
		Local_Is_found = NOTFOUND_ID;
	}
	return Local_Is_found;
}

/**********************************************************************
*  syntax :  APP_SetNewID(APP_EmpID copy_ID);
*  Description : This function shall call the EEPROM to save the  APP_EmpID Value
*  				 into the Address of the same value of  APP_EmpID.
				 If the Address of the same value of  APP_EmpID is already
				 written with the value of  APP_EmpID that means this ID
				 is Already taken by another employee.
*  Synch/Asynch     : Synchronous
*  Reentrance       : Non-Reentrant
*  parameters (in)  : APP_EmpID copy_ID
*  parameters (out) : None
*  Return Value     : None
*************************************************************************/
void APP_SetNewID(APP_EmpID copy_ID)
{
	H_Void_EEPROMWrite((u16)copy_ID , copy_ID);
	g_SystemState = NEW_ENROLLMENT_ID_SUCCESS;
}
/**********************************************************************
*  syntax :  APP_NewEnrollemtSuccessful();
*  Description : This function shall call the LCD to show that the new enrollment has been done successfully.
*  Synch/Asynch     : Synchronous
*  Reentrance       : Reentrant
*  parameters (in)  : None
*  parameters (out) : None
*  Return Value     : None
*************************************************************************/
void APP_NewEnrollemtSuccessful(void){
	H_Lcd_Void_LCDClear();
	H_Lcd_Void_LCDWriteString((u8*) "ENROLLMENT SUCCESS");
	H_LED_Void_LedOn(LED_GRN);
	_delay_ms(DISPLAY_FOR_5_SEC);
	H_LED_Void_LedOff(LED_GRN);
}

/**********************************************************************
*  syntax :  APP_RTC_Display_Time_and_Date(Date_And_Time*);
*  Description : This function shall call the LCD to show the time and date.
*  Synch/Asynch     : Synchronous
*  Reentrance       : Reentrant
*  parameters (in)  : Pointer to struct of date and time represents current time
*  parameters (out) : None
*  Return Value     : None
*************************************************************************/

void  APP_RTC_Display_Time_and_Date(Date_And_Time * ptr)
{
    H_Lcd_Void_LCDGoTo(2,0);
    H_Lcd_Void_LCDWriteString(RTC_Day_str_display[ptr->day]);
    H_Lcd_Void_LCDWriteCharacter(' ');
    H_Lcd_Void_LCDWriteNumber(ptr->date);
    H_Lcd_Void_LCDWriteCharacter('/');
    H_Lcd_Void_LCDWriteNumber(ptr->month);
    H_Lcd_Void_LCDWriteCharacter('/');
    H_Lcd_Void_LCDWriteNumber(20);
    H_Lcd_Void_LCDWriteNumber(ptr->year);
	H_Lcd_Void_LCDGoTo(3,0);
    H_Lcd_Void_LCDWriteNumber(ptr->hours);
    H_Lcd_Void_LCDWriteCharacter(':');
    H_Lcd_Void_LCDWriteNumber(ptr->min);
    H_Lcd_Void_LCDWriteCharacter(':');
    H_Lcd_Void_LCDWriteNumber(ptr->sec);
    if((ptr->hours>=9)&&(ptr->min>0)){
    	H_Lcd_Void_LCDWriteString((u8*) " LATE");
    }
    else{
    	H_Lcd_Void_LCDWriteString((u8*) " welcome");
    }
}

/**********************************************************************
*  syntax :  APP_ConfirmAttendanceSuccessful();
*  Description : This function shall call the LCD to show that the confirmation of attendance has been done successfully.
*  Synch/Asynch     : Synchronous
*  Reentrance       : Reentrant
*  parameters (in)  : None
*  parameters (out) : None
*  Return Value     : None
*************************************************************************/
void APP_Confirm_Attendance_Success(APP_EmpID copy_ID, Date_And_Time* CurrentDateAndTime_ptr){
	H_Lcd_Void_LCDClear();
	/*LCD says Hi Tarek for ID 7, Hi youssef for ID 6 and Hi seif for ID 5 */
	if(copy_ID == TAREK_ID){
		H_Lcd_Void_LCDWriteString((u8*) "Hi Tarek");
		H_Lcd_Void_LCDGoTo(1,0);
	}
	else if(copy_ID == YOUSSEF_ID){
		H_Lcd_Void_LCDWriteString((u8*) "Hi Youssef");
		H_Lcd_Void_LCDGoTo(1,0);
	}
	else if(copy_ID == SEIF_ID){
		H_Lcd_Void_LCDWriteString((u8*) "Hi Seif");
		H_Lcd_Void_LCDGoTo(1,0);
	}
	H_Lcd_Void_LCDWriteString((u8*) "Attendance success");
	/*Get the current time and display it on LCD to show if late or not*/
	RTC_Get_Time(CurrentDateAndTime_ptr);
	APP_RTC_Display_Time_and_Date(CurrentDateAndTime_ptr);
	/*Opens the door for the employee and the green led for 5 secs*/
	Relay_ON(RELAY_1_PIN);
	H_LED_Void_LedOn(LED_GRN);
	_delay_ms(DISPLAY_FOR_5_SEC);
	Relay_OFF(RELAY_1_PIN);
	H_LED_Void_LedOff(LED_GRN);
}

/**********************************************************************
*  syntax :  APP_PutFingerToScan();
*  Description : This function shall call the LCD to tell the employee to put his finger to scan.
*  Synch/Asynch     : Synchronous
*  Reentrance       : Reentrant
*  parameters (in)  : None
*  parameters (out) : None
*  Return Value     : None
*************************************************************************/
void APP_PutFingerToScan(void){
	H_Lcd_Void_LCDClear();
	H_Lcd_Void_LCDWriteString((u8*) "Put finger to scan");
}

/**********************************************************************
*  syntax :  APP_WarningHandler(warnings);
*  Description : This function shall call the LCD to show the warning according to the input describing the current error.
*  Synch/Asynch     : Synchronous
*  Reentrance       : Reentrant
*  parameters (in)  : WarningType
*  parameters (out) : None
*  Return Value     : user_action (GOBACK_BUTTON_PRESSED or MAIN_MENU_BUTTON_PRESSED)
*************************************************************************/
u8 APP_WarningHandler(APP_Warnings warningType){
	/* sequence
	* led red on  +  lcd warning display
	* wait  for the go back key to get pressed
	* then
	* initial state */
	u8 user_action = KEYPAD_RELEASED;
	H_LED_Void_LedOn(LED_RED);
	H_Lcd_Void_LCDClear();
	H_Lcd_Void_LCDWriteString((u8*) "Warning:");
	H_Lcd_Void_LCDGoTo(1,0);
	/*Switch on warning type and print the warning on LCD*/
	switch (warningType){
	case ALREADY_TAKEN_ID:
		H_Lcd_Void_LCDWriteString((u8*) "ALREADY TAKEN ID");
	break;
	case OUT_OF_RANGE_ID:
		H_Lcd_Void_LCDWriteString((u8*) "OUT OF RANGE ID");
	break;
	case ID_NOT_FOUND:
		H_Lcd_Void_LCDWriteString((u8*) "ID NOT FOUND");
	break;
	}
	H_Lcd_Void_LCDGoTo(3,0);
	H_Lcd_Void_LCDWriteString((u8*) "C : Main Menu");
	while(user_action != MAIN_MENU_BUTTON_PRESSED){
		user_action=H_KeyPad_U8_KeyPadRead();
	}
	H_LED_Void_LedOff(LED_RED);
	return user_action;
}


