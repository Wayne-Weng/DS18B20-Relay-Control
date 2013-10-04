#include "reg_mpc82g516.h"
#include "typedef.h"
#include "delaytime.h"
#include "IO.h"
#include "seven_seg.h"
#include "DS18B20.h"

void main(void){
	
	u8 tempInteger=0x00;
	u16 tempFloat=0x0000;
	
	DS18B20_EXISTENCE SysExistance=EXI_UNKNOWN;
	DS18B20_POWER SysPwr=PWR_UNKNOWN;
	
	//
	SysExistance=DS18B20_Reset();
	P14=1;
	if(SysExistance==PRESENCE) P14=0;
	DS18B20_SendCMD(CMD_ROM_SKIP);
	DS18B20_SendCMD(CMD_FUNC_WRITESCRATCHPAD);
	DS18B20_WriteScratchPad(9,T_127_C,T_0_C);
	
	//
	SysExistance=DS18B20_Reset();
	P14=1;
	if(SysExistance==PRESENCE) P14=0;
	DS18B20_SendCMD(CMD_ROM_SKIP);
	DS18B20_SendCMD(CMD_FUNC_READPWR);
	SysPwr=DS18B20_CheckPower();
	P15=1;
	if(SysPwr==EXTERNAL) P15=0;
	
	
	while(1){
		
		//
		SysExistance=DS18B20_Reset();
		P14=1;
		if(SysExistance==PRESENCE) P14=0;
		DS18B20_SendCMD(CMD_ROM_SKIP);
		DS18B20_SendCMD(CMD_FUNC_CONVERTT);
		while(!DS18B20_ReadSlot());
		
		//
		SysExistance=DS18B20_Reset();
		P14=1;
		if(SysExistance==PRESENCE) P14=0;
		DS18B20_SendCMD(CMD_ROM_SKIP);
		DS18B20_SendCMD(CMD_FUNC_READSCRATCHPAD);
		DS18B20_ReadTemp(&tempInteger,&tempFloat);
		
		if(P20==0){
			scan(tempFloat);
		}
		if(P20!=0){
			scan(tempInteger);
		}
	}
}
