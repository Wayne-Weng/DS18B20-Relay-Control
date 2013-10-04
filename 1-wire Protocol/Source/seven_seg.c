#include <reg_mpc82g516.h>
#include <typedef.h>
#include <delaytime.h>

char table[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7C,0x27,0x7F,0x67,0x77,0x7C,0x58,0x5E,0x79,0x71};  //0~9,A~F

u8 div_N0,div_N1,div_N2,div_N3;

void div(u16 num){
	div_N0=(num%10);
	div_N1=((num%100)-(num%10))/10;
	div_N2=((num%1000)-(num%100))/100;
	div_N3=((num%10000)-(num%1000))/1000;
}

void scan(u16 num){
	div(num);
	P4=(P4&0xF0)+ ~(0x01);
	P0=~table[div_N3];
	delay(2);
	P4=(P4&0xF0)+ ~(0x02);
	P0=~table[div_N2];
	delay(2);
	P4=(P4&0xF0)+ ~(0x04);
	P0=~table[div_N1];
	delay(2);
	P4=(P4&0xF0)+ ~(0x08);
	P0=~table[div_N0];
	delay(2);
}
