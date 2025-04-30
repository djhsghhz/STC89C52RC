//按下时是低电平,包含按一次只出一个字母功能，防抖动功能。
//因为之前数码管实验懒惰没有编写程序，并且这个按键反馈不太明显，所以结合一下，将按下按键显示在数码管上
#include "reg52.h"

typedef unsigned int u16;
typedef unsigned char u8;
	
//由硬件图按键与端口连接图定义,低电平有效(K4-K8未定义)
sbit KEY1 = P3^1;
sbit KEY2 = P3^0;
sbit KEY3 = P3^2;
sbit LED  = P2^0;

//宏定义键盘按下的值，如果这里不定义，下面按下时return的值一个一个改也行，这里宏定义的话可以在这里直接一键改
#define KEY1PRESS 1
#define KEY2PRESS 2
#define KEY3PRESS 3
#define KEYUNPRESS 0
#define SMG_A_DP_PORT	P0	//使用宏定义数码管段码口

//数码管位选确定哪一个管点亮（参考硬件图连接确定P22等用哪一个端口，参考数据手册真值表确定值为多少时对应哪一个端口）

sbit LEDA = P2^2;
sbit LEDB = P2^3;
sbit LEDC = P2^4;



//数码管段选显示数字

//gsmg_code数组保存了数字0~F对应的段选编码。比如：0x3f对应0（abcdef段点亮，g段熄灭）0x06对应1（bc段点亮，其余熄灭）
//举例说明：在超市收银机上的数码管显示数字8，就是将a,b,c,d,e,f,g七段全部点亮，那么对应段码就是0x7F
u8 gsmg_code[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};



//延时函数
u8 delay(u8 tenus){
	while(tenus--);{}
	}

//键盘检测函数
u8 keyboard_scan(){
	if(KEY1 == 0||KEY2 == 0||KEY3 == 0){    //按键按下（低电平有效）
		if(KEY1 == 0)
			{ return KEY1PRESS;}
		else if(KEY2 == 0)
			{ return KEY2PRESS;}
		else if(KEY3 == 0)
		  {	return KEY3PRESS;}
}
	else if(KEY1 == 1&&KEY2 == 1&&KEY3 == 1){
		return KEYUNPRESS;
	}
}

//点亮数码管函数，这里将不同数字只显示在第一个数码管上（真值表000LLL位选对应第一个数码管）
void display(u8 key){
	LEDA = 1;
	LEDB = 1;
	LEDC = 1;
	SMG_A_DP_PORT = gsmg_code[key];//eg按下1键返回1，对应到这个数组里面第二个值0x06，即数码管显示1
	
}
void main(){
	u8 keyval;
	while(1){
		keyval = keyboard_scan();
		display(keyval);
		delay(100000000);
		
	}
}

