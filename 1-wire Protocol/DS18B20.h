
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

#include "typedef.h"

#ifndef _DS18B20_
#define _DS18B20_
	
	//Define the 1-wire-bus Port pin from mcu
	#define Bus P11
	
	//DS18B20 ROM Commands
	#define CMD_ROM_SEARCH       					 0xF0
	#define CMD_ROM_READ         					 0x33
	#define CMD_ROM_MATCH        					 0x55
	#define CMD_ROM_SKIP         					 0xCC
	#define CMD_ROM_ALARMSEARCH  					 0xEC
	
	//DS18B20 Function Commands
	#define CMD_FUNC_CONVERTT              0x44
	#define CMD_FUNC_READSCRATCHPAD        0xBE
	#define CMD_FUNC_WRITESCRATCHPAD       0x4E
	#define CMD_FUNC_COPYSCRATCHPAD        0x48
	#define CMD_FUNC_RECALLEE              0xB8
	#define CMD_FUNC_READPWR               0xB4
	
	//DS18B20 Power mode response
	#define PWR_EXTERNAL 1
	#define PWR_PARASITE 0
	
	//DS18B20 Configuration Register sets
	#define Res_9_bits                     0x1F
	#define Res_10_bits                    0x3F
	#define Res_11_bits                    0x5F
	#define Res_12_bits                    0x7F
	
	//DS18B20 Temp H, Temp L Alarm trigger value sets
	//User defined, 2's Complement
	#define T_127_C                       0x7F
	#define T_0_C                         0x00
	
	//DS18B20 States
	typedef enum {
		ABSENCE=0,
		PRESENCE,
		EXI_UNKNOWN
	} DS18B20_EXISTENCE;

	typedef enum {
		EXTERNAL=0,
		PARASITE,
		PWR_UNKNOWN
	} DS18B20_POWER;
	
	extern DS18B20_EXISTENCE DS18B20_Reset(void);
	extern void DS18B20_CMD_ROM_SkipROM(void);
	extern void DS18B20_CMD_FUNC_ConvertT(void);
	extern void DS18B20_CMD_FUNC_ReadScratchpad(void);
	extern void DS18B20_SendCMD(u8 cmd);
	extern void DS18B20_ReadTemp(u8* const tempIntegerAddr,u16* const tempFloatAddr);
	extern DS18B20_POWER DS18B20_CheckPower(void);
	extern void DS18B20_WriteScratchPad(u8 bitNum,u8 TempH,u8 TempL);
	extern void DS18B20_Write0(void);
	extern void DS18B20_Write1(void);
	extern u8 DS18B20_ReadSlot(void);
	
#endif
