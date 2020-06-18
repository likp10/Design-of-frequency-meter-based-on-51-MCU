#ifndef __2402_H__
#define __2402_H__

#include<reg52.h>    //  包含51单片机寄存器定义

#include <intrins.h>      //包含_nop_()函数定义的头文件


#define	OP_READ	0xa1		// 器件地址以及读取操作,0xa1即为1010 0001B
#define	OP_WRITE 0xa0		// 器件地址以及写入操作,0xa1即为1010 0000B


sbit SDA=P1^1;          //将串行数据总线SDA位定义在为P3.4引脚
sbit SCL=P1^0;         //将串行时钟总线SDA位定义在为P3.3引脚

extern void WriteSet(unsigned char add, unsigned char dat);
extern unsigned char ReadSet(unsigned char set_addr);

#endif