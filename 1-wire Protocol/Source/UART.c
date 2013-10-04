#include <reg_mpc82g516.h>
#include <typedef.h>
#include <delaytime.h>

void UART_Reg_Init(void);
void UART_WriteByte(u8 inputByte);		//Byte Transmit process
u8 UART_GetByte(void);								//Byte Receive process

void UART_Reg_Init(void){
	//Timer Setup
	TMOD|=0x20;										//set T1 in 8bit Auto-Reload Mode for use of Baud Rate Generator
	AUXR2|=0x40;									//set T1 clock source from Fosc
	TL1=0xF4;											//set TL1 initial value, for 115200BR
	TH1=0xF4;											//set reload value, for 115200BR
	
	//Serial Port Setup
	SCON|=0x50;										//set Serial Port in Mode 1, enable serial reception
	PCON|=0x80;										//to get non-divided clocks from T1 for Baud Rate Generator
	
	/*
	 *NOTE: Fosc must be 22.1184MHz for above config.
	*/
	
	//Run Baud Rate Generator
	TR1=1;
}

void UART_WriteByte(u8 inputByte){
	SBUF=inputByte;
	TI=0;
	while(TI!=1);
}

u8 UART_GetByte(void){
	RI=0;
	while(RI!=1);
	
	return SBUF;
}
