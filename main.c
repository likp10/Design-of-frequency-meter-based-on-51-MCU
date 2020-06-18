#include <reg52.h>
#include <intrins.h>
#include "2402.h" 

#define uchar unsigned char
#define uint unsigned int

//时间(s)，地址，频率
uint second, addr, freq;

bit sear;							//逻辑值，表示是否正在查询

//存储的频率数组，将频率按位转换成数码管管码
uchar freqs[4];

//共阳极数码管码管码
uchar code segca[] = 
{0xc0, 0xf9, 0xa4, 0xb0, 0x99, 
	0x92, 0x82, 0xf8, 0x80, 0x90};

sbit wled = P1^6;			//超量程报警灯
sbit sled = P1^7;			//查询灯

//声明数码管显示位的控制位
sbit w1 = P2^0;
sbit w2 = P2^1;
sbit w3 = P2^2;
sbit w4 = P2^3;

//声明三个按键
sbit key1 = P1^2;			//存储键
sbit key2 = P1^3;			//查询加
sbit key3 = P1^4;			//查询减

//声明函数
void delay(uchar ms);
void key();
void disp();
void read();
void timer0();

/***************************************************
函数功能：主函数
***************************************************/
void main()
{	   
	TMOD = 0x51;    		//定时器设置
	EA = 1;         		//总中断
	ET0 = 1;
	TR0 = 1;
	ET1 = 1;
	TR1 = 1;
	while(1)
	{
		sled = ~sear;			//查询灯灭
		if(sear == 0)
			disp();
		else
			read();
		key();
	}	
}

/***************************************************
函数功能：延时一段时间
入口参数：ms
***************************************************/
void delay(uchar ms)
{
	uchar i ;
	while(ms--)
	{
		for(i = 0; i < 250; i++);
	}
}

/***************************************************
函数功能：响应按键
***************************************************/
void key()
{
	if(key1 == 0)			//保持
	{
		delay(5);
		sear = 0;
		WriteSet(addr * 2, freq / 256);			//存高位
		WriteSet(addr * 2 + 1, freq % 256);	//存低位
		addr++; 
		if(addr == 100) addr = 0;
		while(!key1);
	}
	if(key2 == 0)			//查询加
	{
		delay(5);
		sear = 1;
		if(addr < 100) addr++; 
		while(!key2);
	}
	if(key3 == 0)			//查询减
	{
		delay(5);
		sear = 1;
		if(addr > 0) addr--; 
		while(!key3);
	}  				 
}

/***************************************************
函数功能：在数码管上显示频率
***************************************************/
void disp()
{	
	P0 = freqs[0];
	w1 = 1;
	delay(1);
	w1 = 0;
	P0 = freqs[1];
	w2 = 1;
	delay(1);
	w2 = 0;
	P0 = freqs[2];
	w3 = 1;
	delay(1);
	w3 = 0;
	P0 = freqs[3];
	w4 = 1;
	delay(1);
	w4 = 0;
}

/***************************************************
函数功能：读取所存储的数据
***************************************************/
void read()
{
	freq = ReadSet(addr * 2) * 256 
		+ ReadSet(addr * 2 + 1);
	freqs[0] = segca[freq % 10000 / 1000];
	freqs[1] = segca[freq % 1000 / 100];
	freqs[2] = segca[freq % 100 / 10];
	freqs[3] = segca[freq % 10];  
	P0 = freqs[0];
	w1 = 1;
	delay(1);
	w1 = 0;
	P0 = freqs[1];
	w2 = 1;
	delay(1);
	w2 = 0;
	P0 = freqs[2];
	w3 = 1;
	delay(1);
	w3 = 0;
	P0 = freqs[3];
	w4 = 1;
	delay(1);
	w4 = 0;
}

/***************************************************
函数功能：定时器0中断服务程序
***************************************************/
void timer0() interrupt 1
{
	TH0 = (65536-50000) / 256;
	TL0 = (65536-50000) % 256;
	second++;
	if(second >= 20)		//1秒到
	{
		second = 0;				 
    freq = TH1*256 + TL1;		//从计数器中读出频率 
		TH1 = 0;
		TL1 = 0; 
		
		wled = 1;			//超量程报警灯灭
		freqs[0] = segca[freq % 10000 / 1000];
		freqs[1] = segca[freq % 1000 / 100];
		freqs[2] = segca[freq % 100/10];
		freqs[3] = segca[freq % 10]; 
		if(freq > 9999)
 		{
			wled = 0;		//超量程报警灯亮
			freqs[0] = 0xff;
		  	freqs[1] = 0xff;
		  	freqs[2] = 0xff;
		  	freqs[3] = 0xff; 
    	} 
	} 
}
