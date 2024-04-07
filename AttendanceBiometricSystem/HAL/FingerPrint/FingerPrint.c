
#include "FingerPrint_Interface.h" 
/*A function to store the Acknowledgment Bytes*/
u8 AckPack [12] = {0};
   /*
	A FUNCTION TO SEND THE WHOLE A FRAME AS FOLLOWS
 * 2 BYTE HEADER
 * 8 BYTE ADDRESS
 * 1 BYTE PACKAGE ID 
 * 2 PACKAGE LENGTH  */
void FingerP_send(u8 Package_ID,u16 length){
/*SEND 2BYTE HEADER*/
	UART_sendByte(HEADER_HIGH_BYTE);
	UART_sendByte(HEADER_LOW_BYTE);
/*SEND 8 BYTE ADDRESS*/
	UART_sendByte(ADDRESS_FOURTH_BYTE);
	UART_sendByte(ADDRESS_THIRD_BYTE);
	UART_sendByte(ADDRESS_SECOND_BYTE);
	UART_sendByte(ADDRESS_FIRST_BYTE); 
/*SEND 1 BYTE PACKAGE ID */
	UART_sendByte(Package_ID);
/*SEND 2 BYTE PACKAGE LENGTH*/
	UART_sendByte(length>>ONE_BYTE_SHIFT);
	UART_sendByte(length>>NO_BYTE_SHIFT); 
}
/* A FUNCTION TO SEND A HANDSHAKE */
void FingerPS_handShake(void){
	FingerP_send(PCK_ID_COMMAND_PACK,LENGTH_3BYTE);
	UART_sendByte(HAND_SHAKE);
	UART_sendByte((PCK_ID_COMMAND_PACK+LENGTH_3BYTE+HAND_SHAKE)>>ONE_BYTE_SHIFT);
	UART_sendByte((PCK_ID_COMMAND_PACK+LENGTH_3BYTE+HAND_SHAKE)>>NO_BYTE_SHIFT);
} 
void FingerPS_AuraLedConfig (void){
	FingerP_send(PCK_ID_COMMAND_PACK,LENGTH_7BYTE);
	UART_sendByte(AURA_LED_CONFIG);
	UART_sendByte(FLASHING_LIGHT);
	UART_sendByte(MEDIUM_SPEED);
	UART_sendByte(COLOR_INDEX_RED);
	UART_sendByte(LIGHT_COUNT);
	UART_sendByte((PCK_ID_COMMAND_PACK+LENGTH_7BYTE+AURA_LED_CONFIG+FLASHING_LIGHT+MEDIUM_SPEED+COLOR_INDEX_RED+LIGHT_COUNT)>>ONE_BYTE_SHIFT);
	UART_sendByte((PCK_ID_COMMAND_PACK+LENGTH_7BYTE+AURA_LED_CONFIG+FLASHING_LIGHT+MEDIUM_SPEED+COLOR_INDEX_RED+LIGHT_COUNT)>>NO_BYTE_SHIFT);
}
/* Detecting finger and store the detected finger image in ImageBuffer while
 * returning successful confirmation*/
void FingerPS_genImg(){
	FingerP_send(PCK_ID_COMMAND_PACK,LENGTH_3BYTE);
	UART_sendByte(COLLECT_FINGER_IMAGE);
	UART_sendByte((PCK_ID_COMMAND_PACK+LENGTH_3BYTE+COLLECT_FINGER_IMAGE)>>ONE_BYTE_SHIFT);
	UART_sendByte((PCK_ID_COMMAND_PACK+LENGTH_3BYTE+COLLECT_FINGER_IMAGE)>>NO_BYTE_SHIFT);
} 
/* Generate character file from the original finger image in ImageBuffer and
 * store the file in CharBuffer1 or CharBuffer2 
 * BufferID of CharBuffer1 and CharBuffer2 are 0x01 and 0x02 */
void FingerPS_convertImg1CharFile(){
	/*CharBuffer1*/
		UART_sendByte(0xef);
		UART_sendByte(0x01);
		UART_sendByte(0xff);
		UART_sendByte(0xff);
		UART_sendByte(0xff);
		UART_sendByte(0xff);
		UART_sendByte(0x01);
		UART_sendByte(0x00);
		UART_sendByte(0x04);
		UART_sendByte(0x02);
		UART_sendByte(0x01);
		UART_sendByte(0x00);
		UART_sendByte(0x08);
} 
void FingerPS_convertImg2CharFile(){	
	/*CharBuffer2*/
		UART_sendByte(0xef);
		UART_sendByte(0x01);
		UART_sendByte(0xff);
		UART_sendByte(0xff);
		UART_sendByte(0xff);
		UART_sendByte(0xff);
		UART_sendByte(0x01);
		UART_sendByte(0x00);
		UART_sendByte(0x04);
		UART_sendByte(0x02);
		UART_sendByte(0x02);
		UART_sendByte(0x00);
		UART_sendByte(0x09);
}
/* Combine information of character files from CharBuffer1 and CharBuffer2 and
 * generate a template which is stored back in both CharBuffer1 and CharBuffer2*/
void FingerPS_genTemplate(){
	UART_sendByte(0xef);
	UART_sendByte(0x01);
	UART_sendByte(0xff);
	UART_sendByte(0xff);
	UART_sendByte(0xff);
	UART_sendByte(0xff);
	UART_sendByte(0x01);
	UART_sendByte(0x00);
	UART_sendByte(0x03);
	UART_sendByte(0x05);
	UART_sendByte(0x00);
	UART_sendByte(0x09);
} 
/* Store the template of specified buffer (Buffer1/Buffer2)
 * at the designated location of Flash library
 * PageID?Flash location of the Template				
 */
void FingerPS_strTemplate(u16 address){
	FingerP_send(PCK_ID_COMMAND_PACK,LENGTH_6BYTE);
	UART_sendByte(STR_TEMPLATE);
	UART_sendByte(CHAR_BUFFER_1);
	UART_sendByte(address >> ONE_BYTE_SHIFT);
	UART_sendByte(address >> NO_BYTE_SHIFT); 
	UART_sendByte((PCK_ID_COMMAND_PACK+LENGTH_6BYTE+STR_TEMPLATE+CHAR_BUFFER_1+address)>>ONE_BYTE_SHIFT);
	UART_sendByte((PCK_ID_COMMAND_PACK+LENGTH_6BYTE+STR_TEMPLATE+CHAR_BUFFER_1+address)>>NO_BYTE_SHIFT);
} 

/* Search the whole finger library for the template that matches the one in CharBuffer1 or CharBuffer2, 
   We need to call two function [FingerPS_genImg() and FingerPS_convertImg2CharFile()]
   Input Parameter: BufferID, StartPage (searching start address), PageNum?searching numbers*/
void FingerPS_searchFinger(){
	FingerP_send(PCK_ID_COMMAND_PACK,LENGTH_8BYTE);
	UART_sendByte(SEARCH_FINGER);
	UART_sendByte(CHAR_BUFFER_1);
	UART_sendByte(FIRST_PAGE_ID>>ONE_BYTE_SHIFT);
	UART_sendByte(FIRST_PAGE_ID>>NO_BYTE_SHIFT);
	
	UART_sendByte(NUM_OF_PAGES_TO_SEARCH>>ONE_BYTE_SHIFT);
	UART_sendByte(NUM_OF_PAGES_TO_SEARCH>>NO_BYTE_SHIFT);
	
	UART_sendByte((PCK_ID_COMMAND_PACK+LENGTH_8BYTE+SEARCH_FINGER+CHAR_BUFFER_1+FIRST_PAGE_ID+NUM_OF_PAGES_TO_SEARCH)>>ONE_BYTE_SHIFT);
	UART_sendByte((PCK_ID_COMMAND_PACK+LENGTH_8BYTE+SEARCH_FINGER+CHAR_BUFFER_1+FIRST_PAGE_ID+NUM_OF_PAGES_TO_SEARCH)>>NO_BYTE_SHIFT);
}  
void FingerPS_LoadCharFile(u16 address){
	UART_sendByte(0xef);
	UART_sendByte(0x01);
	UART_sendByte(0xff);
	UART_sendByte(0xff);
	UART_sendByte(0xff);
	UART_sendByte(0xff);
	UART_sendByte(0x01);
	UART_sendByte(0x00);
	UART_sendByte(0x06);
	UART_sendByte(0x07);
	UART_sendByte(0x02); /*load in the char file 2*/
	UART_sendByte(address>>ONE_BYTE_SHIFT);
	UART_sendByte(address>>NO_BYTE_SHIFT);
	UART_sendByte((0x01+0x00+0x06+0x07+0x02+address)>>ONE_BYTE_SHIFT);
	UART_sendByte((0x01+0x00+0x06+0x07+0x02+address)>>NO_BYTE_SHIFT);
} 
void FingerPS_match(){
	UART_sendByte(0xef);
	UART_sendByte(0x01);
	UART_sendByte(0xff);
	UART_sendByte(0xff);
	UART_sendByte(0xff);
	UART_sendByte(0xff);
	UART_sendByte(0x01);
	UART_sendByte(0x00);
	UART_sendByte(0x03);
	UART_sendByte(0x03);
	UART_sendByte(0x00);
	UART_sendByte(0x07);
}
/********************************************** ISR	***************************************/
void StoreAckBytes (u8 RecivedByte){
	static u8 i = 0;
	AckPack [i] = RecivedByte; 
	if (i==11){
		i = 0;
	}
	else i++;
}
/********************************************* FUNCTION TO CHECK THE SUM OF EACH ACKNOWLEDGMENT PACK	***************/
u8 FingerPS_CheckAck (){
	u8 CheckResult;
	if (AckPack[11] == 0x0A){
		CheckResult = true ;
		AckPack [11] = 0x00; 
	}
	else{
		CheckResult = false;
	}
	return CheckResult;
} 
void FingerPS_SetNewFingerPrint (u16 address){
	/*********************************************		FIRST IMAGE GENERATION	******************************************/
	RepeatGenImag1:
	FingerPS_genImg();
	_delay_ms(100);
	if (FingerPS_CheckAck() == false) goto RepeatGenImag1; /*WAIT FOR ACK*/
	/*********************************************	FIRST CHARACTER FILE GENERATION	**************************************/
	RepeatConvertFile1:
	FingerPS_convertImg1CharFile();
	_delay_ms(100);
	if (FingerPS_CheckAck() == false) goto RepeatConvertFile1; /*WAIT FOR ACK*/
	/*********************************************		SECOND IMAGE GENERATION		**************************************/
	RepeatGenImag2:
	FingerPS_genImg();
	_delay_ms(100);
	if (FingerPS_CheckAck() == false) goto RepeatGenImag2;/*WAIT FOR ACK*/
	/*********************************************		SECONED CHAR FILE GENERATED		**********************************/
	RepeatConvertFile2:
	FingerPS_convertImg2CharFile();
	_delay_ms(100);
	if (FingerPS_CheckAck() == false) goto RepeatConvertFile2;/*WAIT FOR ACK*/
	/********************************************		Generate Tempelate			**********************************/
	FingerPS_genTemplate();
	_delay_ms(500);
	if (FingerPS_CheckAck() == false) goto RepeatGenImag1;/*WAIT FOR ACK*/
	/*******************************************		Tempelate Storing			***********************************/
	RepeatStrTemp:
	FingerPS_strTemplate(address);
	_delay_ms(100);
	if (FingerPS_CheckAck() == false) goto RepeatStrTemp;/*WAIT FOR ACK*/
}
u8 FingerPS_CheckOneToOneMatch(u16 address){
	u8 match_result; 
	/*******************************************		Generate Image to check one to one match	********************/
	RepeatCheckImagGen:
	FingerPS_genImg();
	_delay_ms(100);
	if (FingerPS_CheckAck() == false) goto RepeatCheckImagGen; /*WAIT FOR ACK*/ 
	/******************************************			Generate char from Check Image	****************************************/
	RepeatCharCheckFileGen:
	FingerPS_convertImg1CharFile();
	_delay_ms(100);
	if (FingerPS_CheckAck() == false) goto RepeatCharCheckFileGen; /*WAIT FOR ACK*/
	/******************************************			Load Char in char file 2	***********************************/
	RepeatLoadCharFile:
	FingerPS_LoadCharFile(address);
	_delay_ms(100);
	if (FingerPS_CheckAck() == false) goto RepeatLoadCharFile; /*Wait for ACK*/
	/******************************************			CHECK MATCH				****************************************/
	FingerPS_match();
	_delay_ms(100); 
	if (AckPack[9] == 0x00){
		match_result = MATCHED;
	}
	else {
		match_result = NOTMATCHED; 
	} 
	return match_result; 
}

