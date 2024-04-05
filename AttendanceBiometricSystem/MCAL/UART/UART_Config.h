/*
 * UART_Config.h
 *
 *  Created on: Mar 19, 2024
 *      Author: tarek
 */

#ifndef UART_CONFIG_H_
#define UART_CONFIG_H_
/*******CLK SOURCE MHZ_8 OR MHZ_16***/ 
#define F_CPU 16000000 
#define BAUDRATE 57600
/*******STOPBIT_MODE 1 OR 2  ********/
#define STOPBIT_MODE 1 
/*******PARITY_MODE DISABLE OR ENABLE_E OR ENABLE_O**/ 
#define PARITY_MODE DISABLE
/******* RECIEVING USING INTERRUPT MODE**/ 
/******* ENABLE - DISABLE ***************/
#define UART_INT_MODE ENABLE

#endif /* UART_CONFIG_H_ */
