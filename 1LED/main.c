/*
#include "reg52.h"

sbit LED1 = P2^0;//P2端口第0位,也就是P2.0引脚

void Delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 123; j++); // 内层循环耗时，用于微调
}

void main(){
	//LED1 = 0;//0点亮//
	while(1)
	{
		//将 P2.0 引脚定义为一个名为 LED1 的位变量，方便之后用 LED1 来控制这个引脚。//
		//HAL_Delay(1000);//
		Delay_ms(1000);
		LED1 = 0;
		//HAL_Delay(1000);//
		Delay_ms(1000);
		LED1 = 1;
	}
}

*/
//下面进行一个实验，8位的流水灯时，用uchar，这个类型正好是8位，正好对应8个灯，循环移位时一个个点亮。
//那如果是12个灯呢，这个时候就要用16位的unsigned int，这时候位数才够，但是0000 1000 0000 0000对应第12个灯
//那0001 0000 0000 0000时呢，0010 0000 0000 0000时呢，这些对应不存在的第13,14,15,16灯。
//如果只有12个灯，但定义了一个总位数位16的数字，在0001 0000 0000 0000，0010 0000 0000 0000，0100 0000 0000 0000，
//1000 0000 0000 0000这些时候并没有任何一盏灯会亮，而循环到0000 0000 0000 0001时，第一盏灯亮是吗
//用8个灯来实验，不定义正好8位的uchar，而定义16位的uint，造成0000 0000 1000 0000后再左移，0000 0001 0000 0000没有灯对应的情况
//每次移位都延时，那么如果亮完一轮，从第8灯到第1灯中间隔了移动8位所需的时间，则在0001 0000 0000 0000，0010 0000 0000 0000，0100 0000 0000 0000，
//1000 0000 0000 0000这些时候并没有任何一盏灯会亮，而循环到0000 0000 0000 0001时，第一盏灯亮

#include "reg52.h"
#include "intrins.h"

#define uchar unsigned char
#define uint unsigned int




void Delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 123; j++); // 内层循环耗时，用于微调
}

void main(){
	uint led = 0000000000000001;
	while(1)
	{
		//将 P2.0 引脚定义为一个名为 LED1 的位变量，方便之后用 LED1 来控制这个引脚。//
		P2 = ~led;
		Delay_ms(1000);
		led = _irol_(led,1);//第一次实验用了crol（charrol导致失败）
	}
}


//8灯和1灯之间并没有停顿.二次实验更正：确实停了，之前因为用了crol（char左移，可能是自动转成8位char了？）改成irol（uint 左移后成功实现中间停顿）。
/*罪  魁  祸  首
extern unsigned char _cror_    (unsigned char, unsigned char);
extern unsigned int  _iror_    (unsigned int,  unsigned char);
extern unsigned long _lror_    (unsigned long, unsigned char);
extern unsigned char _crol_    (unsigned char, unsigned char);
extern unsigned int  _irol_    (unsigned int,  unsigned char);
extern unsigned long _lrol_    (unsigned long, unsigned char);
罪  魁  祸  首
*/

/*
#include <reg52.h>
#include <intrins.h>  // 用于 _nop_()

#define uint unsigned int
#define uchar unsigned char

// 延时函数（毫秒级）
void Delay_ms(uint ms) {
    uint i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 123; j++);
}

// 左循环移位函数（8位）
uchar crol(uchar value, uchar n) {
    return (value << n) | (value >> (8 - n));
}

void main() {
    uchar led = 0x01;  // 最初点亮最低位（P2.0）

    while (1) {
        P2 = ~led;        // P2 输出：取反是因为低电平点亮LED
        Delay_ms(200);    // 延时200ms
        led = crol(led, 1); // 左循环移位
    }
}
*/