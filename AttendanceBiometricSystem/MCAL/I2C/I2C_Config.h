/*
 * I2C_Config.h
 *
 * Created: 3/25/2024 10:02:45 PM
 *  Author: seif
 */ 


#ifndef I2C_CONFIG_H_
#define I2C_CONFIG_H_

/* I2C Status Bits in the TWSR Register */
#define TWI_START         			0x08 /* start has been sent */
#define TWI_REP_START     			0x10 /* repeated start */
#define START_FAILED				0x00 /* START FAILED */ 
#define REP_START_FAILED			0x00 /* Repeat Start failed */
#define TWI_MT_SLA_W_ACK  			0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_NACK	  			0X20 /*Master transmit ( slave address + Write request ) to slave but didn't receive ACK from slave*/
#define TWI_MT_DATA_ACK   			0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MT_DATA_NACK  			0X30 /*Master transmit data to slave but didn't receive ACK from slave*/
#define TWI_MT_SLA_OR_DATA_ARB_LOST	0X38 /*Master send slave address + write or data but lost arbitration*/
#define TWI_MT_SLA_R_ACK  			0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */

#define TWI_MR_DATA_ACK   			0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  			0x58 /* Master received data but doesn't send ACK to slave. */
#define TWI_MR_SLA_R_ARB_LOST		0X38 /*Master send slave address + write or data but lost arbitration*/
#define TWI_MR_SLA_R_ACK			0X40 /*Master receive ACk after sending slave address + read*/
#define TWI_MR_SLA_R_NACK			0x48 /*Master didn't receive ACk after sending slave address + read*/
/*TWPS (Pre Scaler Vlaues)
  TWPS1		TWPS0		PS Value
	0		  0				1
	0		  1				4 
	1		  0				16
	1		  1				64 
	*/ 
#define TWPS_Value1 1
#define TWPS_Value4 4 
#define TWPS_Value16 16 
#define TWPS_Value64 64 
/*Pre define TWPS => PS Value 1*/
#define TWPS TWPS_Value1

#endif /* I2C_CONFIG_H_ */