/*
 * I2C_Private.h
 *
 * Created: 3/25/2024 10:03:04 PM
 *  Author: seif
 */ 


#ifndef I2C_PRIVATE_H_
#define I2C_PRIVATE_H_
#include "../../LIB/STD.h"

#define TWBR_REG *(volatile u8*)(0x20) 
#define TWCR_REG *(volatile u8*)(0x56)
#define TWSR_REG *(volatile u8*)(0x21) 
#define TWDR_REG *(volatile u8*)(0x23) 
#define TWAR_REG *(volatile u8*)(0x22) 

/*TWSR BITS*/
#define    TWS7        7
#define    TWS6        6
#define    TWS5        5
#define    TWS4        4
#define    TWS3        3
/* bit 2 preserved*/
#define    TWPS1       1
#define    TWPS0       0

/* TWAR bits */
/*from 2nd to 7th are slave address bits */
#define    TWGCE       0

/* TWCR bits */

#define    TWINT       7
#define    TWEA        6
#define    TWSTA       5
#define    TWSTO       4
#define    TWWC        3
#define    TWEN        2
/*preserved bit*/
#define    TWIE        0

#endif /* TWI_PRIVATE_H_ */
