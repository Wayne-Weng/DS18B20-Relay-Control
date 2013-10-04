#include "reg_mpc82g516.h"
#include "typedef.h"
#include "IO.h"

//PortX, PinX, Mode(0:Bi-dir,1:PP,2:Hi-Z,3:Open Collector)
void IOConfig(u8 port,u8 pin,u8 mode){
	
	if(mode==0){								//Bi-dir
		
		if(port==0){
			P0M0&=~(1<<pin);
			P0M1&=~(1<<pin);
		}
		else if(port==1){
			P1M0&=~(1<<pin);
			P1M1&=~(1<<pin);;
		}
		else if(port==2){
			P2M0&=~(1<<pin);
			P2M1&=~(1<<pin);
		}
		else if(port==3){
			P3M0&=~(1<<pin);
			P3M1&=~(1<<pin);
		}
		
	}
	else if(mode==1){						//PP
		
		if(port==0){
			P0M0&=~(1<<pin);
			P0M1|=1<<pin;
		}
		else if(port==1){
			P1M0&=~(1<<pin);
			P1M1|=1<<pin;
		}
		else if(port==2){
			P2M0&=~(1<<pin);
			P2M1|=1<<pin;
		}
		else if(port==3){
			P3M0&=~(1<<pin);
			P3M1|=1<<pin;
		}
		
	}
	else if(mode==2){						//Hi-Z
		
		if(port==0){
			P0M0|=1<<pin;
			P0M1&=~(1<<pin);
		}
		else if(port==1){
			P1M0|=1<<pin;
			P1M1&=~(1<<pin);
		}
		else if(port==2){
			P2M0|=1<<pin;
			P2M1&=~(1<<pin);
		}
		else if(port==3){
			P3M0|=1<<pin;
			P3M1&=~(1<<pin);
		}
		
	}
	else if(mode==3){						//Open Collector
		
		if(port==0){
			P0M0|=1<<pin;
			P0M1|=1<<pin;
		}
		else if(port==1){
			P1M0|=1<<pin;
			P1M1|=1<<pin;
		}
		else if(port==2){
			P2M0|=1<<pin;
			P2M1|=1<<pin;
		}
		else if(port==3){
			P3M0|=1<<pin;
			P3M1|=1<<pin;
		}
		
	}
	
}
