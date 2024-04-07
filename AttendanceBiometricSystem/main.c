/******************************************************************************
 *
 * Project: Biometric Attendance System for companies
 *
 * File Name: Main.c
 *
 * Description: This project aims mainly to check the attendance of employees in companies
 *              on time. There are two general cases:
 *              1- A new employee shall set up his ID and FingerPrint for the first time.
 *              2- An employee that has already set up his ID and fingerPrint and needs to
 *                 confirm his attendance on a daily basis
 *
 * Author: Group Effort (Youssef Abdelaziz - Seif Ahmed - Tarek Gamal)
 *
 ******************************************************************************/

/*****************************************************************************
 *                            INCLUDED MODULES
 ******************************************************************************/
#include "MCAL\F_CPU\F_CPU_Interface.h"
#include "HAL/EEPROM/EEPROM_Interfac.h"
#include "HAL/KeyPad/Keypad_Interface.h"
#include "HAL/LCD/LCD_Interface.h"
#include "HAL/LED/LED_Interface.h"
#include "HAL/RTC/RTC_Inter.h"
#include "HAL/FingerPrint/FingerPrint_Interface.h"
#include "APP/Biometric_Attendance_Interface.h"

#define MAX_TRIALS 3

/*****************************************************************************
 *                            Global Variables Definition
 ******************************************************************************/
APP_CurrentState g_SystemState = INITIAL_STATE;
u8 is_Finger_Match = 0;

/*********************************************************************************
 *                                    MAIN
 *********************************************************************************/
int main(void)
{
    Date_And_Time Current_Date_And_Time = {0, 43, 12, 1, 7, 4, 24};
    //RTC_Set_Time(&Current_Date_And_Time);
    APP_EmpID EmpID = 0xFF;

    /* Initialization of the used modules in the MCAL layer */
    M_Void_I2C_Init();
    UART_Init();
    UART_receiveByteAsynchCallBack(StoreAckBytes);
    sei();
    /* Initialization of the used modules in the HAL layer */
    H_LED_Void_LedInit(LED_RED);
    H_LED_Void_LedInit(LED_GRN);
    H_KeyPad_Void_KeyPadInit();
    H_Lcd_Void_LCDInit();
    RTC_Init();

    while (1)
    {
        /* Initial function
         * Press '*' for Attendance Confirmation
         * Press '#' for New Enrollment */ 
	switch (g_SystemState)
     { 
/**************************************************************************************************************************************
 *																Initial State Condition												  *
 **************************************************************************************************************************************/
		case INITIAL_STATE:
            APP_Init(); 
			break; 
/**************************************************************************************************************************************
 *															   Confirm Attendance Enter ID											  * 
 **************************************************************************************************************************************/ 						
        case CONFIRM_ATTENDANCE_ENTER_ID:
            EmpID = APP_GetID(); 
			/*If the user pressed the Go back Button We go to the initial state*/
            if (EmpID == GOBACK)
            {
                g_SystemState = INITIAL_STATE;
            } 
			/*If the user entered the Id, then we check if it is present*/
            else
            {
                g_SystemState = APP_CheckIDPresence(EmpID); 
				/*If the ID Already Present in the Data Base then check the Finger Print*/
                if (g_SystemState == FOUND_ID)
                {
                    /*If ID is found, ask for the fingerprint scan*/
                    APP_PutFingerToScan();
                    FingerPS_AuraLedConfig();
                    _delay_ms(3000);
                    is_Finger_Match = FingerPS_CheckOneToOneMatch((u16)EmpID); 
					/*If the fingerprint matches the employee's saved fingerprint, show success message*/
                    if (MATCHED == is_Finger_Match)
                    {
                        APP_Confirm_Attendance_Success(EmpID, &Current_Date_And_Time);
                        g_SystemState = INITIAL_STATE;
                    } 
					/*If the fingerprint does not match then,
					  Turn on Red Led, Show Warning Message, wait for 3 sec delay time and go for Initial State*/
                    else if (NOTMATCHED == is_Finger_Match) 
                    {
                        H_LED_Void_LedOn(LED_RED);
                        H_Lcd_Void_LCDClear();
                        H_Lcd_Void_LCDWriteString((u8*)"Warning:");
                        H_Lcd_Void_LCDGoTo(1,0);
                        H_Lcd_Void_LCDWriteString((u8*)"FINGERPRINT NOTFOUND");
                        _delay_ms(3000);
                        H_LED_Void_LedOff(LED_RED);
                        g_SystemState = INITIAL_STATE;
                    }
                }
                else if (g_SystemState == NOTFOUND_ID)
                {
                    if (APP_WarningHandler(ID_NOT_FOUND) == MAIN_MENU_BUTTON_PRESSED){
                        /* Go back to the main menu to choose new enrollment */
                        g_SystemState = INITIAL_STATE;
					}
                }
            }
            break; /* End of confirm attendance case */
/***************************************************************************************************************************************************
 *															New Enrollment Case																	   * 
 ***************************************************************************************************************************************************/
        case NEW_ENROLLMENT_SET_NEW_ID:
            EmpID = APP_GetID();
			/*If the user pressed the Go back Button We go to the initial state*/
            if (EmpID == GOBACK)
            {
                g_SystemState = INITIAL_STATE;
            }
			/*If the user entered the Id, then we check if it is present*/
            else
            {
                g_SystemState = APP_CheckIDPresence(EmpID);
			/*For a new enrollment, the ID must not be present in Data base*/
                if (g_SystemState == NOTFOUND_ID)
                {
                    /* If the ID is out of range, display a warning on the LCD */
                    if (EmpID == OUT_OF_RANGE_ID)
                    {
                        if (APP_WarningHandler(OUT_OF_RANGE_ID) == MAIN_MENU_BUTTON_PRESSED)
                        {
                            /* Go back to the main menu to choose new enrollment */
                            g_SystemState = INITIAL_STATE;
                        }
                    } 
					/*IF the ID is in range, we go for saving Embloyee Finger Print*/
                    else
                    {
                        /* Prompt for finger scan */
                        APP_PutFingerToScan();
                        FingerPS_AuraLedConfig();
                        _delay_ms(4000);
                        /* Save the new fingerprint in memory */
                        FingerPS_SetNewFingerPrint((u16)EmpID);
                        /* Write the new employee ID to EEPROM */
                        APP_SetNewID(EmpID);
                        /* Show success message */
                        APP_NewEnrollemtSuccessful();
                        g_SystemState = INITIAL_STATE;
                    }
                }
                else if (g_SystemState == FOUND_ID)
                {
                    if (APP_WarningHandler(ALREADY_TAKEN_ID) == MAIN_MENU_BUTTON_PRESSED)
                    {
                        /* Go back to the main menu to choose new enrollment */
                        g_SystemState = INITIAL_STATE;
                    }
                }
            }
            break; /* End of New Enrollment */
        }
    }
    return 0;
}
