/*
 * I2C_Prog.c
 *
 * Created: 3/25/2024 10:02:05 PM
 *  Author: seif
 */ 
#include "I2C_Private.h"
#include "I2C_Interface.h" 
#include "F_CPU_Interface.h"
#define SCL_CLK 200000 /*Select SCL_CLK in Hz*/
#define BITRATE(TWSR_REG)	((F_CPU/SCL_CLK)-16)/(2 * (1 << (TWSR_REG*2)))
/*************************		INITIATE I2C	******************/
void M_Void_I2C_Init (){
	#if TWPS == TWPS_Value1
		CLR_BIT(TWSR_REG,TWPS0);
		CLR_BIT(TWSR_REG,TWPS1);
	#elif TWPS == TWPS_Value4
		SET_BIT(TWSR_REG,TWPS0);
		CLR_BIT(TWSR_REG,TWPS1); 
	#elif TWPS == TWPS_Value16 
		CLR_BIT(TWSR_REG,TWPS0);
		SET_BIT(TWSR_REG,TWPS1);
	#elif TWPS == TWPS_Value64 
		SET_BIT(TWSR_REG,TWPS0);
		SET_BIT(TWSR_REG,TWPS1);
	#endif
	TWBR_REG = BITRATE(TWSR_REG);	 
} 
/*************************		SEND START		*******************/
u8 M_U8_I2C_Start(){
	u8 transmit_status;		
	TWCR_REG =(1<<TWSTA)|(1<<TWEN)|(1<<TWINT); /*Enable TWI, generate START*/
	while(!(TWCR_REG & (1<<TWINT)));	/* Wait until TWI finish its current job*/
	transmit_status = (TWSR_REG & 0xF8);		/* Read TWI status register */
	if(transmit_status != TWI_START)		/* Check weather START transmitted or not? */
	transmit_status = START_FAILED;			/* Return 0 to indicate start condition fail*/ 
	return transmit_status;
} 
/*After Start we will send slave address with write*/
u8 M_U8_I2C_SendSlaveAddressWithWrite (u8 write_address){
	u8 transmit_status;
	/*SEND ADDRESS BYTE*/
	TWDR_REG = write_address;		    /* Write SLA+W in TWI data register */
	TWCR_REG = (1<<TWEN)|(1<<TWINT);	/* Enable TWI & clear interrupt flag */
	while(!(TWCR_REG&(1<<TWINT)));		/* Wait until TWI finish its current job */
	
	/*CHECK TRANSMIT WITH ACK*/
	transmit_status= (TWSR_REG & 0xF8);		/* Read TWI status register */
	if(transmit_status == TWI_MT_SLA_W_ACK)	/* Check for SLA+W transmitted & ack received */
	transmit_status = TWI_MT_SLA_W_ACK;		/* ACK RECIEVED */
	
	/*TRANSMIT WITH NO ACK*/
	else if(transmit_status == TWI_MT_SLA_NACK)		/* Check for SLA+W transmitted*/
	transmit_status = TWI_MT_SLA_NACK;				/* Nack received */  
	
	return transmit_status;
}
/**************************		REPEAT START FUNCTION		****************/
u8 M_U8_I2C_Repeated_Start(){
	u8 transmit_status;									
	TWCR_REG = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT); /* Enable TWI, generate start */
	while(!(TWCR_REG & (1<<TWINT)));			/* Wait until TWI finish its current job */
	transmit_status = TWSR_REG & 0xF8;					/* Read TWI status register */
	if(transmit_status!= TWI_REP_START)					/* Check for repeated start transmitted */
	transmit_status = REP_START_FAILED;					/* repeated start condition fail */
	
	return transmit_status; 
}
/**************************		SEND SLAVE ADDRESS WITH WRITE	**************/
u8 M_U8_I2C_SendSlaveAddressWithRead(u8 read_address){
	u8 transmit_status; 
	TWDR_REG = read_address;					/* Write SLA+R in TWI data register */
	TWCR_REG = (1<<TWEN)|(1<<TWINT);			/* Enable TWI and clear interrupt flag */
	while(!(TWCR_REG&(1<<TWINT)));				/* Wait until TWI finish its current job */
	transmit_status = TWSR_REG & 0xF8;					/* Read TWI status register */
	
	if(transmit_status == TWI_MT_SLA_R_ACK)				/* Check for SLA+R transmitted*/
	transmit_status = TWI_MT_SLA_R_ACK;					/* Ack received */
	else if(transmit_status == TWI_MR_SLA_R_NACK)				/* Check for SLA+R transmitted*/
	transmit_status = TWI_MR_SLA_R_NACK;					/* Nack received */
	
	return transmit_status; 
}
/**************************		WRITE DATA	FUNCTION	***********************/
u8 M_U8_I2C_Write(u8 data){
	u8 transmit_status;						
	TWDR_REG = data;				/* Copy data in TWI data register */
	TWCR_REG = (1<<TWEN)|(1<<TWINT);/* Enable TWI and clear interrupt flag */
	while(!(TWCR_REG&(1<<TWINT)));	/* Wait until TWI finish its current job */
	transmit_status = TWSR_REG & 0xF8;		/* Read TWI status register */
	if(transmit_status==TWI_MT_DATA_ACK)		/* Check for data transmitted*/
	transmit_status = TWI_MT_DATA_ACK;			/* Ack received */
	else if(transmit_status==TWI_MT_DATA_NACK)	/* Check for data transmitted*/
	transmit_status = TWI_MT_DATA_NACK;		/* Nack received */ 
	
	return transmit_status; 
}
u8 M_U8_I2C_ReadWithAck(){
	TWCR_REG = (1<<TWEN)|(1<<TWINT)|(1<<TWEA);	/* Enable TWI, generation of Ack */
    while(!(TWCR_REG&(1<<TWINT)));				/* Wait until TWI finish its current job */
    return TWDR_REG;							/* Return received data */
}
u8 M_U8_I2C_ReadWithNack(){
	TWCR_REG =(1<<TWEN)|(1<<TWINT);	/* Enable TWI and clear interrupt flag */
    while(!(TWCR_REG&(1<<TWINT)));			/* Wait until TWI finish its current job */
    return TWDR_REG;					/* Return received data */
}
void M_Void_I2C_STOP(){
    TWCR_REG=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);/* Enable TWI, generate stop */
    while(TWCR_REG&(1<<TWSTO));				 /* Wait until stop condition execution */
}