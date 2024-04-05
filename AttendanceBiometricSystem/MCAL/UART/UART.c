/*
 * UART.c
 *
 *  Created on: Mar 19, 2024
 *      Author: tarek
 */

#include "UART_Private.h"
#include "UART_Interface.h"
 
void (*ptr_func) (u8) = '\0'; 
void UART_Init(void){
u8 UCSRB_REG_VALUE = 0; 
u8 UCSRC_REG_VALUE = 0; 
u8 UBRR_Value = 0;  
/********SET URSELC TO 1 TO WRITE TO THE UCSRC REG*/
SET_BIT (UCSRC_REG_VALUE, URSEL);
/*ENABLE RX AND TX BIT4 AND BIT3 IN UCSRB REG*/ 
	SET_BIT (UCSRB_REG_VALUE, RXEN); 
	SET_BIT (UCSRB_REG_VALUE, TXEN);  	
/****************DATA MODE UCSRC **/
/********DEFINING 8 BITS DATA MODE*/ 
	CLR_BIT (UCSRB_REG_VALUE, UCSZ2); 
	SET_BIT (UCSRC_REG_VALUE, UCSZ1); 
	SET_BIT (UCSRC_REG_VALUE, UCSZ0); 
/*DEFINING PARITY MODE*/ 
	#if PARITY_MODE == DISABLE
		CLR_BIT (UCSRC_REG_VALUE, UPM0); 
		CLR_BIT (UCSRC_REG_VALUE, UPM1); 
	#elif PARITY_MODE == ENABLE_E  
		CLR_BIT (UCSRC_REG_VALUE, UPM0); 
		SET_BIT (UCSRC_REG_VALUE, UPM1); 
	#elif PARITY_MODE == ENABLE_O 
		SET_BIT (UCSRC_REG_VALUE, UPM0);
		SET_BIT (UCSRC_REG_VALUE, UPM1);
	#endif 
/*DEFINING STOP MODE*/
	#if STOPBIT_MODE == 1  
		CLR_BIT (UCSRC_REG_VALUE, USBS); 
	#elif STOPBIT_MODE == 2 
		SET_BIT (UCSRC_REG_VALUE, USBS);
	#endif   
/*DEFINING BAUDRATE*/ 
	UBRR_Value = (u16)(((F_CPU) / ((u32) 16*BAUDRATE)) -1) ; 	
	CLR_BIT(UBRRH, URSEL); 
	UBRRH = (UBRR_Value >> 8); 
	UBRRL = UBRR_Value;
	UBRRH |=  0x00;
/*INTERRUPT MODE ENABLE*/ 
	#if UART_INT_MODE == ENABLE  
		SET_BIT(UCSRB_REG_VALUE,RXCIE); 
	#elif UART_INT_MODE == DISABLE 
		CLR_BIT(UCSRB_REG_VALUE,RXCIE);
	#endif 
/*ASSIGN VALUES OF CONIFIGURATION TO THE UCSRB REG*/
UCSRB = UCSRB_REG_VALUE; 
/********SET URSELC TO 1 TO WRITE TO THE UCSRC REG*/
SET_BIT (UCSRC, URSEL); 
UCSRC = UCSRC_REG_VALUE;  
}
void UART_sendByte(u8 Copy_U8_Data){ 
	/*The UDRE Flag indicates if the transmit buffer (UDR) is ready to receive new data. If
	UDRE is one, the buffer is empty, and therefore ready to be written. */
	while (!GET_BIT(UCSRA,UDRE)){
		/*MAKE SURE THAT UDRE BIT IS ONE*/
	} 
	
		UDR = Copy_U8_Data; 
	/*This flag bit is set when the entire frame in the transmit Shift Register has been shifted
	   out and there are no new data currently present in the transmit buffer (UDR). */
	while (!GET_BIT(UCSRA,TXC)){
	/*WAIT UNTIL THE TRANSMIT FLAG IS UP*/
	}
}
void UART_sendString(u8* str){ 
	u8 i=0; 
	while (*(str+i)!= '\0'){
		UART_sendByte(*(str+i)); 
		i++;
	}   
}
u8 UART_receiveByteSynch(void){
/*THIS FUNCTION IS TO READ BYTE USING POLLING*/ 
	while (!GET_BIT(UCSRA,RXC)){
		/*WAIT UNTIL THE RECIEVER FLAG IS ONE*/
	}
	return (UDR);
}
void UART_receiveByteAsynchCallBack(void(*CpyPtr)(u8)){
	ptr_func = CpyPtr; 
} 
ISR(USART_RXC_vect){
	ptr_func(UDR);
}
