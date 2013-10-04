#include <typedef.h>

/*
*		_UART_
*/

#ifndef _UART_
#define _UART_
	
	void UART_Reg_Init(void);
	void UART_WriteByte(u8 inputByte);		//Byte Transmit process
	u8 UART_GetByte(void);								//Byte Receive process

#endif