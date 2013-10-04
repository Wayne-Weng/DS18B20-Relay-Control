#include <typedef.h>

void delay(u16 n){
	u16 j;
	for(;n>0;n--)
		for(j=0;j<2400;j++);
}