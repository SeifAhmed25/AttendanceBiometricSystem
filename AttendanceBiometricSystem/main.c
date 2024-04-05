/******************************************************************************
 *
 * Project: Biometric Attendance System for companies
 *
 * File Name: Main.c
 *
 * Description: This project aims mainly to check the attendance of employees in companies
 *              on time, there are two general cases:
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
#include "F_CPU_Interface.h"
#include "HAL/EEPROM/EEPROM_Interfac.h"
#include "HAL/KeyPad/Keypad_Interface.h"
#include "HAL/LCD/LCD_Interface.h"
#include "HAL/LED/LED_Interface.h"
#include "HAL/RTC/RTC_Inter.h"
#include "APP/Biometric_Attendance_Interface.h"
#include "HAL/FingerPrint/FingerPrint_Interface.h"

#define MAX_TRIALS              3

/*****************************************************************************
 *                            Global Variables Definition
 ******************************************************************************/
APP_CurrentState g_SystemState = INITIAL_STATE;

/*********************************************************************************
 *                                    MAIN
 *********************************************************************************/
void main(void)
{
    Date_And_Time Current_Date_And_Time = {35, 21, 2, 6, 28, 3, 24};
    //RTC_Set_Time(&Current_Date_And_Time);
    APP_EmpID EmpID = 0xFF;

    /* Initialization of the used modules in the MCAL layer */
    M_Void_I2C_Init();
    UART_Init();

    /* Initialization of the used modules in the HAL layer */
    H_LED_Void_LedInit(LED_RED);
    H_LED_Void_LedInit(LED_GRN);

    H_KeyPad_Void_KeyPadInit();
    H_Lcd_Void_LCDInit();
    RTC_Init();

    u8 WrongFingerPrintCounter = 0;
    while (1)
    {
        /* Initial function
         * Press '*' Attendance Confirmation
         * Press '#' New Enrollment */
        if (g_SystemState == INITIAL_STATE)
        {
            APP_Init();
        }
        switch (g_SystemState)
        {
        case CONFIRM_ATTENDANCE_ENTER_ID:
            EmpID = APP_GetID();
            if (EmpID == GOBACK)
            {
                g_SystemState = INITIAL_STATE;
            }
            else
            {
                g_SystemState = APP_CheckIDPresence(EmpID);
                if (g_SystemState == FOUND_ID)
                {
                    /* In case ID is found then ask for the fingerPrint scan */
                    APP_PutFingerToScan();
                    _delay_ms(3000);
                    FingerPS_AuraLedConfig();
                    do
                    {
                        if (MATCHED == FingerPS_CheckOneToOneMatch((u16)EmpID))
                        {
                            /* If the fingerprint matches the employee's fingerprint saved, LCD will show that the confirmation of attendance has been successful */
                            APP_Confirm_Attendance_Success(EmpID, &Current_Date_And_Time);
                            g_SystemState = INITIAL_STATE;
                            WrongFingerPrintCounter = 0;
                        }
                        else if (NOTMATCHED == FingerPS_CheckOneToOneMatch((u16)EmpID))
                        {
                            /* If the fingerprint doesn't match the employee's fingerprint saved will try again for 3 times then a warning will be displayed */
                            WrongFingerPrintCounter++;
                            if (WrongFingerPrintCounter == MAX_TRIALS)
                            {
                                if (APP_WarningHandler(FINGERPRINT_NOT_FOUND) == MAIN_MENU_BUTTON_PRESSED)
                                {
                                    /* Go back to the main menu to choose new enrollment */
                                    g_SystemState = INITIAL_STATE;
                                }
                                else if (APP_WarningHandler(FINGERPRINT_NOT_FOUND) == GOBACK_BUTTON_PRESSED)
                                {
                                    /* Take one step back in the program and go back to confirm attendance */
                                    g_SystemState = CONFIRM_ATTENDANCE_ENTER_ID;
                                }
                            }
                        }
                    } while (WrongFingerPrintCounter < MAX_TRIALS);
                    /* Return the value of wrong fingerprint counter to 0 */
                    WrongFingerPrintCounter = 0;
                }
                else if (g_SystemState == NOTFOUND_ID)
                {
                    if (APP_WarningHandler(ID_NOT_FOUND) == MAIN_MENU_BUTTON_PRESSED)
                    {
                        /* Go back to the main menu to choose new enrollment */
                        g_SystemState = INITIAL_STATE;
                    } 
					else if (APP_WarningHandler(ID_NOT_FOUND) == GOBACK_BUTTON_PRESSED){
						/* Go back to the Enter ID */
						g_SystemState = CONFIRM_ATTENDANCE_ENTER_ID;
					}
                }
            }
            break; /* End of confirm attendance case */

        case NEW_ENROLLMENT_SET_NEW_ID:
            EmpID = APP_GetID();
            if (EmpID == GOBACK)
            {
                g_SystemState = INITIAL_STATE;
            }
            else
            {
                g_SystemState = APP_CheckIDPresence(EmpID);
                if (g_SystemState == NOTFOUND_ID)
                {
                    /* If the ID inserted out of range, display on LCD warning that the ID is out of range */
                    if (EmpID == OUT_OF_RANGE_ID)
                    {
                        if (APP_WarningHandler(OUT_OF_RANGE_ID) == MAIN_MENU_BUTTON_PRESSED)
                        {
                            /* Go back to the main menu to choose new enrollment */
                            g_SystemState = INITIAL_STATE;
                        }
                    }
                    else
                    {
                        /* Show Put your finger to scan on the lcd */
                        APP_PutFingerToScan();
                        _delay_ms(4000);
                        FingerPS_AuraLedConfig();
                        /* Save the new fingerprint of the new employee in the corresponding address in the fingerprint memory */
                        FingerPS_SetNewFingerPrint((u16)EmpID);
                        /* Write the New employee ID in the corresponding address in the EEPROM */
                        APP_SetNewID(EmpID);
                        /* Call LCD Display SUCCESS NEW ENROLLMENT
                         * Press Go-back button to the main menu */
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
                    else if (APP_WarningHandler(ALREADY_TAKEN_ID) == GOBACK_BUTTON_PRESSED)
                    {
                        /* Take one step back in the program and go back to confirm attendance */
                        g_SystemState = NEW_ENROLLMENT_SET_NEW_ID;
                    }
                }
            }
            break; /* End of New Enrollment */
        }
    }
}
