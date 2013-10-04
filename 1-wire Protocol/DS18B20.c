
//
//	DS18B20 Control Drivers
//  Tested with MPC82G516 @ fosc of 24MHz
//
//	Date:2013/6/3 (First Implemented)
//								 DS18B20_SendCMD();
//			 2013/6/6  
//                 DS18B20_ReadTemp();
//								 DS18B20_CheckPower();
//                 DS18B20_WriteScratchPad();
//
//************************************************************************

#include "reg_mpc82g516.h"
#include "DS18B20.h"
#include "IO.h"
#include "typedef.h"
#include "delaytime.h"

//the reset signal following by the presence signal from DS18B20
DS18B20_EXISTENCE DS18B20_Reset(void){
	
	Bus=1;									//make sure the output is 1 to avoid improper pulse
	IOConfig(1,1,3);				//P11 Open Collector (Active Low)	
	Bus=0;
	delay_us(700);					//Reset Signal to DS18B20, at least 480us, 700=about 500us
	IOConfig(1,1,2);				//P11 Hi-Z Input. Release bus, pull up by Resistor
	Bus=1;									//set the output reg back to 1
	
// 	delay_us(5);						//Prevent rise time being captured
// 	
// 	while(Bus==1);					//Wait for the presence response
// 	while(Bus==0);					//Wait for the end of presence response
// 	
// 	delay_us(5);						//Wait for a few us
	
	delay_us(110);						//Wait about 80us to the middle of presence response
	
	if(Bus==0){
		while(Bus==0);					//if the bus is low, then wait for the rising edge(end of presence response)
		delay_us(5);						//Wait for a few us
		return PRESENCE;
	}
	else{
		return ABSENCE;
	}
	
}

//ROM SkipROM:0xCC
void DS18B20_CMD_ROM_SkipROM(void){
	
	DS18B20_Write0();		//LSB
	DS18B20_Write0();
	DS18B20_Write1();
	DS18B20_Write1();
	DS18B20_Write0();
	DS18B20_Write0();
	DS18B20_Write1();
	DS18B20_Write1();		//MSB
	
}

//FUNC Convert T:0x44
void DS18B20_CMD_FUNC_ConvertT(void){
	
	DS18B20_Write0();		//LSB
	DS18B20_Write0();
	DS18B20_Write1();
	DS18B20_Write0();
	DS18B20_Write0();
	DS18B20_Write0();
	DS18B20_Write1();
	DS18B20_Write0();		//MSB
	
}

//FUNC Read Scratchpad:0xBE
void DS18B20_CMD_FUNC_ReadScratchpad(void){
	
	DS18B20_Write0();		//LSB
	DS18B20_Write1();
	DS18B20_Write1();
	DS18B20_Write1();
	DS18B20_Write1();
	DS18B20_Write1();
	DS18B20_Write0();
	DS18B20_Write1();		//MSB
	
}

//Command sending, shift bit sending (LSB to MSB). 8 bits total.
void DS18B20_SendCMD(u8 cmd){

	u8 shift=0;
	
	while(shift<8){
		//check if the bit is "0"
		//NOTICE: "==" is first proccesed if there's no "()" for the "&" bitwise operation?!?!How's that?
		if((cmd&(0x01<<shift))==0x00){
			DS18B20_Write0();
		}
		//the bit is not "0", then it's "1"
		else{
			DS18B20_Write1();
		}
		shift++;
	}
	
}

//Read the Temp out of scratchpad and put into the variables
void DS18B20_ReadTemp(u8* const tempIntegerAddr,u16* const tempFloatAddr){

	//clear the L,H Bytes
	u8 tempByteL=0x00,tempByteH=0x00;
	u8 shift=0;
	//Read the L temp Byte on the scratchpad
	for(shift=0;shift<8;shift++){
		tempByteL|=DS18B20_ReadSlot()<<shift;
	}
	//Read the H temp Byte on the scratchpad
	for(shift=0;shift<8;shift++){
		tempByteH|=DS18B20_ReadSlot()<<shift;
	}
	//Parse the Integer value of temp(8bit with sign bit on the MSB, 2's complement)
	*tempIntegerAddr=(tempByteL>>4)|(tempByteH<<4);
	//Parse the Float value of temp into Integer form(16bit) 625*0(=0)~625*15(=9375) 0.0625 C ~ 0.9375 C
	*tempFloatAddr=0x0271*(tempByteL&0x0F);
	
}

//Check the power mode of DS18B20
DS18B20_POWER DS18B20_CheckPower(void){
	
	if(DS18B20_ReadSlot()==PWR_EXTERNAL){
		return EXTERNAL;
	}
	else{
		return PARASITE;
	}
	
}

//Write scratchpad Bytes(TH, TL and Configuration bits(Convert resolution))
void DS18B20_WriteScratchPad(u8 bitNum,u8 tempH,u8 tempL){
	
	u8 shift=0;
	
	//Write Temp H trigger value
	for(shift=0;shift<8;shift++){
		//check if the bit is "0"
		//NOTICE: "==" is first proccesed if there's no "()" for the "&" bitwise operation?!?!How's that?
		if((tempH&(0x01<<shift))==0x00){
			DS18B20_Write0();
		}
		//the bit is not "0", then it's "1"
		else{
			DS18B20_Write1();
		}
	}
	
	//Write Temp L trigger value
	for(shift=0;shift<8;shift++){
		//check if the bit is "0"
		//NOTICE: "==" is first proccesed if there's no "()" for the "&" bitwise operation?!?!How's that?
		if((tempL&(0x01<<shift))==0x00){
			DS18B20_Write0();
		}
		//the bit is not "0", then it's "1"
		else{
			DS18B20_Write1();
		}
	}
	
	if(bitNum==9){
		//Write Configuration bits(Convert resolution:9)
		for(shift=0;shift<8;shift++){
			//check if the bit is "0"
			//NOTICE: "==" is first proccesed if there's no "()" for the "&" bitwise operation?!?!How's that?
			if((Res_9_bits&(0x01<<shift))==0x00){
				DS18B20_Write0();
			}
			//the bit is not "0", then it's "1"
			else{
				DS18B20_Write1();
			}
		}
	}
	else if(bitNum==10){
		//Write Configuration bits(Convert resolution:10)
		for(shift=0;shift<8;shift++){
			//check if the bit is "0"
			//NOTICE: "==" is first proccesed if there's no "()" for the "&" bitwise operation?!?!How's that?
			if((Res_10_bits&(0x01<<shift))==0x00){
				DS18B20_Write0();
			}
			//the bit is not "0", then it's "1"
			else{
				DS18B20_Write1();
			}
		}
	}
	else if(bitNum==11){
		//Write Configuration bits(Convert resolution:11)
		for(shift=0;shift<8;shift++){
			//check if the bit is "0"
			//NOTICE: "==" is first proccesed if there's no "()" for the "&" bitwise operation?!?!How's that?
			if((Res_11_bits&(0x01<<shift))==0x00){
				DS18B20_Write0();
			}
			//the bit is not "0", then it's "1"
			else{
				DS18B20_Write1();
			}
		}
	}
	else{
		//Write Configuration bits(Convert resolution:12) as a default set
		for(shift=0;shift<8;shift++){
			//check if the bit is "0"
			//NOTICE: "==" is first proccesed if there's no "()" for the "&" bitwise operation?!?!How's that?
			if((Res_12_bits&(0x01<<shift))==0x00){
				DS18B20_Write0();
			}
			//the bit is not "0", then it's "1"
			else{
				DS18B20_Write1();
			}
		}
	}
	
}


//Write bit "0" to DS18B20
void DS18B20_Write0(void){

	//Bus=1;									//make sure the output is 1 to avoid improper pulse
	IOConfig(1,1,3);				//P11 Open Collector (Active Low)	
	Bus=0;									//Initiates a write slot
	delay_us(90);						//90=about 6x us for entire writing time slot duration
	Bus=1;									//Release the bus into recovery time
// 	delay_us(1);						//1=about 1.9us recovery time
	delay_us(2);
	
}

//Write bit "1" to DS18B20
void DS18B20_Write1(void){
	
	//Bus=1;									//make sure the output is 1 to avoid improper pulse
	IOConfig(1,1,3);				//P11 Open Collector (Active Low)	
	Bus=0;									//Initiates a write slot
	delay_us(1);						//1=about 1.9us
	Bus=1;									//Release the bus, pullup to 1
	delay_us(90);						//90=about 6x us for entire writing time slot duration
	
}

//Read bit "0" or "1" to Master from DS18B20
u8 DS18B20_ReadSlot(void){

	u8 Bit=0;
	
	//Bus=1;									//make sure the output is 1 to avoid improper pulse
	IOConfig(1,1,3);				//P11 Open Collector (Active Low)
	Bus=0;									//Initiates a read slot
	delay_us(1);						//1=about 1.9us
	IOConfig(1,1,2);				//P11 Hi-Z Input. Release the bus
	delay_us(10);						//10=about us
	Bit=Bus;								//Capture the bit signal
	delay_us(80);						//80=about us
	Bus=1;									//set the output reg back to 1
	
	return Bit;

}

