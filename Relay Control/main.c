#include "reg_mpc82g516.h"
#include "delaytime.h"
#include "typedef.h"
#include "IO.h"
#include "UART.h"

#define stdbyBut P20


typedef enum {
	EN = 0,
	DIS,
} STATE;

void main(void){
	
	STATE outputState=DIS;
	STATE powerState=DIS;
	
	outputState=DIS;
	powerState=DIS;
	
	P00=0;
	P01=0;
	
	IOConfig(0,0,1);		//P00 PP
	IOConfig(0,1,1);		//P01 PP
	IOConfig(2,0,2);		//P20 Hi-Z
	
	P10=0;
	
	while(1){
		
		if(stdbyBut==0){
			if(powerState==DIS){
				delay(1500);
				if(stdbyBut==0){
					P00=1;
					P01=1;
					P10=1;
					delay(1500);
					powerState=EN;
					outputState=EN;
				}
			}
			else if(powerState==EN){
				delay(1500);
				if(stdbyBut==0){
					P01=0;
					delay(200);
					P00=0;
					P10=0;
					powerState=DIS;
					outputState=DIS;
				}
			}
			while(stdbyBut==0);
			
		}
		
	}
}

