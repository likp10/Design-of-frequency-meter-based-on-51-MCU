#ifndef __2402_H__
#define __2402_H__

#include<reg52.h>    //  ����51��Ƭ���Ĵ�������

#include <intrins.h>      //����_nop_()���������ͷ�ļ�


#define	OP_READ	0xa1		// ������ַ�Լ���ȡ����,0xa1��Ϊ1010 0001B
#define	OP_WRITE 0xa0		// ������ַ�Լ�д�����,0xa1��Ϊ1010 0000B


sbit SDA=P1^1;          //��������������SDAλ������ΪP3.4����
sbit SCL=P1^0;         //������ʱ������SDAλ������ΪP3.3����

extern void WriteSet(unsigned char add, unsigned char dat);
extern unsigned char ReadSet(unsigned char set_addr);

#endif