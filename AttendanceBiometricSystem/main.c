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
    Date_And_Time Current_Date_And_Time = {35, 21, 2, 6, 28, 3, 24};
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

    u8 WrongFingerPrintCounter = 0;

    while (1)
    {
        /* Initial function
         * Press '*' for Attendance Confirmation
         * Press '#' for New Enrollment */
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
                    /* If ID is found, ask for the fingerprint scan */
                    APP_PutFingerToScan();
                    FingerPS_AuraLedConfig();
                    _delay_ms(3000);
                    is_Finger_Match = FingerPS_CheckOneToOneMatch((u16)EmpID);

                    if (MATCHED == is_Finger_Match)
                    {
                        /* If the fingerprint matches the employee's saved fingerprint, show success message */
                        APP_Confirm_Attendance_Success(EmpID, &Current_Date_And_Time);
                        g_SystemState = INITIAL_STATE;
                        WrongFingerPrintCounter = 0;
                        break;
                    }
                    else if (NOTMATCHED == is_Finger_Match)
                    {
                        while ((is_Finger_Match == NOTMATCHED) && (WrongFingerPrintCounter < MAX_TRIALS))
                        {
                            WrongFingerPrintCounter++;
                            H_LED_Void_LedOn(LED_RED);
                            H_Lcd_Void_LCDClear();
                            H_Lcd_Void_LCDWriteString((u8*)"Warning:");
                            H_Lcd_Void_LCDGoTo(1, 0);
                            H_Lcd_Void_LCDWriteString((u8*)"FINGERPRINT NOTFOUND");
                            H_Lcd_Void_LCDGoTo(2, 0);
                            H_Lcd_Void_LCDWriteString((u8*)"PLEASE TRY AGAIN");
                            _delay_ms(1000);
                            FingerPS_AuraLedConfig();
                            _delay_ms(2000);
                            is_Finger_Match = FingerPS_CheckOneToOneMatch((u16)EmpID);
                        }
                    }
                    WrongFingerPrintCounter = 0;
                    g_SystemState = INITIAL_STATE;
                }
                else if (g_SystemState == NOTFOUND_ID)
                {
                    if (APP_WarningHandler(ID_NOT_FOUND) == MAIN_MENU_BUTTON_PRESSED)
                    {
                        /* Go back to the main menu to choose new enrollment */
                        g_SystemState = INITIAL_STATE;
                    }
                    else if (APP_WarningHandler(ID_NOT_FOUND) == GOBACK_BUTTON_PRESSED)
                    {
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
                    /* If the ID is out of range, display a warning on the LCD */
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
                        /* Prompt for finger scan */
                        APP_PutFingerToScan();
                        _delay_ms(4000);
                        FingerPS_AuraLedConfig();
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
                    else if (APP_WarningHandler(ALREADY_TAKEN_ID) == GOBACK_BUTTON_PRESSED)
                    {
                        /* Go back to set new ID */
                        g_SystemState = NEW_ENROLLMENT_SET_NEW_ID;
                    }
                }
            }
            break; /* End of New Enrollment */
        }
    }
    return 0;
}
