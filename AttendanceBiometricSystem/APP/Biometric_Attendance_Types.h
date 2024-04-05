/******************************************************************************
 *
 * Project: Biometric Attendance System for companies
 *
 * File Name: Biometric_Attendance_Types.h
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



#ifndef APP_BIOMETRIC_ATTENDANCE_TYPES_H_
#define APP_BIOMETRIC_ATTENDANCE_TYPES_H_


/*****************************************************************************
 *
 * 							Types*Definition
 *
 ******************************************************************************/


/* This value is specific to the employee , The value would be chosen by employee
 * for either enrollment or attendance confirmation.
 * Specification from the EEPROM and Fingerprint internal EEPROM size.*/
typedef u8 APP_EmpID;


/*  This variable describes the current system state in the runtime so
 * that application can navigate through the different logical conditions
 * These values can be incremented upon the upcoming version of the system if required.*/
typedef enum
{
	INITIAL_STATE,
	NEW_ENROLLMENT_SET_NEW_ID,
	NEW_ENROLLMENT_ID_SUCCESS,
	NEW_ENROLLMENT_ID_TAKEN,
	NEW_ENROLLMENT_ID_OUT_OF_RANGE,
	NEW_ENROLLMENT_SET_NEW_FINGERPRINT,
	NEW_ENROLLMENT_SUCCESS,
	CONFIRM_ATTENDANCE_ENTER_ID,
	NOTFOUND_ID,
	FOUND_ID,
	CONFIRM_ATTENDANCE_ENTER_FINGERPRINT,
	CONFIRM_ATTENDANCE_MISMATCH_FINGERPRINT,
	CONFIRM_ATTENDANCE_MATCH_FINGERPRINT,
	CONFIRM_ATTENDANCE_SUCCESS,

}APP_CurrentState;

/*This variable describes the current error done by the user on runtime and display it on
 * LCD to inform user with the error. */
typedef enum{
	ALREADY_TAKEN_ID,
	ID_NOT_FOUND,
	FINGERPRINT_NOT_FOUND,
	OUT_OF_RANGE_ID=0xFE,
}APP_Warnings;


#endif /* APP_BIOMETRIC_ATTENDANCE_TYPES_H_ */
