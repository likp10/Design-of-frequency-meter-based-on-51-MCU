#include <reg52.h>
#include <intrins.h>
#include "2402.h" 

#define uchar unsigned char
#define uint unsigned int

//ʱ��(s)����ַ��Ƶ��
uint second, addr, freq;

bit sear;							//�߼�ֵ����ʾ�Ƿ����ڲ�ѯ

//�洢��Ƶ�����飬��Ƶ�ʰ�λת��������ܹ���
uchar freqs[4];

//����������������
uchar code segca[] = 
{0xc0, 0xf9, 0xa4, 0xb0, 0x99, 
	0x92, 0x82, 0xf8, 0x80, 0x90};

sbit wled = P1^6;			//�����̱�����
sbit sled = P1^7;			//��ѯ��

//�����������ʾλ�Ŀ���λ
sbit w1 = P2^0;
sbit w2 = P2^1;
sbit w3 = P2^2;
sbit w4 = P2^3;

//������������
sbit key1 = P1^2;			//�洢��
sbit key2 = P1^3;			//��ѯ��
sbit key3 = P1^4;			//��ѯ��

//��������
void delay(uchar ms);
void key();
void disp();
void read();
void timer0();

/***************************************************
�������ܣ�������
***************************************************/
void main()
{	   
	TMOD = 0x51;    		//��ʱ������
	EA = 1;         		//���ж�
	ET0 = 1;
	TR0 = 1;
	ET1 = 1;
	TR1 = 1;
	while(1)
	{
		sled = ~sear;			//��ѯ����
		if(sear == 0)
			disp();
		else
			read();
		key();
	}	
}

/***************************************************
�������ܣ���ʱһ��ʱ��
��ڲ�����ms
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
�������ܣ���Ӧ����
***************************************************/
void key()
{
	if(key1 == 0)			//����
	{
		delay(5);
		sear = 0;
		WriteSet(addr * 2, freq / 256);			//���λ
		WriteSet(addr * 2 + 1, freq % 256);	//���λ
		addr++; 
		if(addr == 100) addr = 0;
		while(!key1);
	}
	if(key2 == 0)			//��ѯ��
	{
		delay(5);
		sear = 1;
		if(addr < 100) addr++; 
		while(!key2);
	}
	if(key3 == 0)			//��ѯ��
	{
		delay(5);
		sear = 1;
		if(addr > 0) addr--; 
		while(!key3);
	}  				 
}

/***************************************************
�������ܣ������������ʾƵ��
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
�������ܣ���ȡ���洢������
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
�������ܣ���ʱ��0�жϷ������
***************************************************/
void timer0() interrupt 1
{
	TH0 = (65536-50000) / 256;
	TL0 = (65536-50000) % 256;
	second++;
	if(second >= 20)		//1�뵽
	{
		second = 0;				 
    freq = TH1*256 + TL1;		//�Ӽ������ж���Ƶ�� 
		TH1 = 0;
		TL1 = 0; 
		
		wled = 1;			//�����̱�������
		freqs[0] = segca[freq % 10000 / 1000];
		freqs[1] = segca[freq % 1000 / 100];
		freqs[2] = segca[freq % 100/10];
		freqs[3] = segca[freq % 10]; 
		if(freq > 9999)
 		{
			wled = 0;		//�����̱�������
			freqs[0] = 0xff;
		  	freqs[1] = 0xff;
		  	freqs[2] = 0xff;
		  	freqs[3] = 0xff; 
    	} 
	} 
}
