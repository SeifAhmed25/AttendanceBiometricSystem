/*******************************************************************************************
 *
 * Module: RTC (Real Time Clock)
 *
 * File Name: RTC_Prog.c
 *
 * Description: program file for the RTC driver
 *
 * Author: Youssef Abdelaziz
 *
 * *******************************************************************************************/


#include "RTC_Inter.h"

u8 RTC_Day_str_display[7][4]={"SAT","SUN","MON","TUE","WEN","THU","FRI"};

void RTC_Init()
{
	M_Void_I2C_Init();
}
void RTC_Set_Time(Date_And_Time *Config_Date)
{
	M_U8_I2C_Start();/*send the start bit*/
	/*send the slave address and write bit*/
	M_U8_I2C_SendSlaveAddressWithWrite(RTC_SLAVE_ADDRESS_WRITE);
	/*write the first address 0x00 in RTC to write the seconds*/
	M_U8_I2C_Write(RTC_SECONDS_REG_ADDRESS);
	/*send the seconds and the CH bit is cleared*/
		/*send the units of seconds in the least 4 bits then the tens seconds in the higher 4 bits   */
	M_U8_I2C_Write((((Config_Date -> sec / 10) << 4)|((Config_Date -> sec) % 10)) & (~(1<<7)));

	/*send the units of mins in the least 4 bits then the tens mins in the higher 4 bits   */
	M_U8_I2C_Write(((Config_Date -> min / 10) << 4)|((Config_Date -> min) % 10));
#if (_12_or_24_MODE == _12_Mode && PM_OR_AM == AM)
	/*send the units of hours in the least 4 bits then the tens hours in the higher 4 bits  ++  set the 6th bit for the 12 hours mode and clear the 5th bit for AM */
		M_U8_I2C_Write((((Config_Date -> hours / 10) << 4)|((Config_Date -> hours) % 10) | (1<<6))&(~(1<<5)));
#elif (_12_or_24_MODE == _12_Mode && PM_OR_AM == PM)
		/*send the units of hours in the least 4 bits then the tens hours in the higher 4 bits  ++  set the 6th bit for the 12 hours mode and the 5th bit for PM */
		M_U8_I2C_Write((((Config_Date -> hours / 10) << 4)|((Config_Date -> hours) % 10) | (1<<6))| ((1<<5)));
#elif _12_or_24_MODE == _24_Mode
		/*send the units of hours in the least 4 bits then the tens hours in the higher 4 bits  ++  clear the 6th bit for the 24 hours mode*/
	    M_U8_I2C_Write((((Config_Date -> hours / 10) << 4)|((Config_Date -> hours) % 10)) &(~(1<<6)));
#endif

	    M_Void_I2C_STOP();
}


void RTC_Set_Date(Date_And_Time *Config_Date)
{
	M_U8_I2C_Start();/*send the start bit*/
	/*send the slave address and write bit*/
	M_U8_I2C_SendSlaveAddressWithWrite(RTC_SLAVE_ADDRESS_WRITE);
	/*write the first address 0x03 in RTC to write the day*/
	M_U8_I2C_Write(RTC_DAYS_REG_ADDRESS);
		/*send the units of seconds in the least 4 bits then the tens seconds in the higher 4 bits   */
	M_U8_I2C_Write(Config_Date -> day);

	/*send the units of date in the least 4 bits then the tens date in the higher 4 bits   */
	M_U8_I2C_Write(((Config_Date -> date / 10) << 4)|((Config_Date -> date) % 10));
	/*send the units of month in the least 4 bits then the tens month in the higher 4 bits   */
	M_U8_I2C_Write(((Config_Date -> month / 10) << 4)|((Config_Date -> month) % 10));
	/*send the units of year in the least 4 bits then the tens year in the higher 4 bits   */
	M_U8_I2C_Write(((Config_Date -> year / 10) << 4)|((Config_Date -> year) % 10));
	M_Void_I2C_STOP();
}

void RTC_Get_Time(Date_And_Time *Get_Time_And_Date)
{
	u8 temp_sec =0;
	u8 temp_min =0;
	u8 temp_hours =0;

	M_U8_I2C_Start();/*send the start bit*/
	/*send the slave address and write bit*/
	M_U8_I2C_SendSlaveAddressWithWrite(RTC_SLAVE_ADDRESS_WRITE);
	/*write the first address 0x00 in RTC to write the day*/
	M_U8_I2C_Write(RTC_SECONDS_REG_ADDRESS);
	M_U8_I2C_Repeated_Start();
	/*send the slave address and read bit*/
	M_U8_I2C_SendSlaveAddressWithRead(RTC_SLAVE_ADDRESS_READ);
	temp_sec = M_U8_I2C_ReadWithAck();
	/*get the seconds units + the tens seconds by >>4 then multiplied by 10*/
	Get_Time_And_Date -> sec = (temp_sec & 0x0F)+(((temp_sec & 0xF0)>>4)*10);
	/*get the mins units + the tens mins by >>4 then multiplied by 10*/
	temp_min = M_U8_I2C_ReadWithAck();
	Get_Time_And_Date -> min = (temp_min & 0x0F)+(((temp_min & 0xF0)>>4)*10);
	/*get the hours units + the tens hours by >>4 then multiplied by 10*/
	temp_hours = M_U8_I2C_ReadWithNack();
	Get_Time_And_Date -> hours = (temp_hours & 0x0F)+(((temp_hours & 0xF0)>>4)*10);
	M_Void_I2C_STOP();
}



void RTC_Get_Date(Date_And_Time *Get_Time_And_Date)
{
	u8 temp_date =0;
	u8 temp_month =0;
	u8 temp_year =0;

	M_U8_I2C_Start();/*send the start bit*/
	/*send the slave address and write bit*/
	M_U8_I2C_SendSlaveAddressWithWrite(RTC_SLAVE_ADDRESS_WRITE);
	/*write the first address 0x03 in RTC to write the day*/
	M_U8_I2C_Write(RTC_DAYS_REG_ADDRESS);
	M_U8_I2C_Repeated_Start();
	/*send the slave address and read bit*/
	M_U8_I2C_SendSlaveAddressWithRead(RTC_SLAVE_ADDRESS_READ);
	Get_Time_And_Date -> day  = M_U8_I2C_ReadWithAck();

	temp_date = M_U8_I2C_ReadWithAck();
	/*get the seconds units + the tens seconds by >>4 then multiplied by 10*/
	Get_Time_And_Date -> date = (temp_date & 0x0F)+(((temp_date>>4)& 0x0F)*10);
	/*get the mins units + the tens mins by >>4 then multiplied by 10*/
	temp_month = M_U8_I2C_ReadWithAck();
	Get_Time_And_Date -> month = (temp_month & 0x0F)+(((temp_month>>4)& 0x0F)*10);
	/*get the hours units + the tens hours by >>4 then multiplied by 10*/
	temp_year = M_U8_I2C_ReadWithNack();
	Get_Time_And_Date -> year = (temp_year & 0x0F)+(((temp_year>>4)& 0x0F)*10);
	M_Void_I2C_STOP();
}

