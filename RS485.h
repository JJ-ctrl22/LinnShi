#ifndef __RS485_H_
#define __RS485_H_

#include "serial.h"
#include "stm32f10x.h"                  // Device header

#define RS485_send()    GPIO_SetBits(GPIOA,GPIO_Pin_8)      //发送使能
#define RS485_receive() GPIO_ResetBits(GPIOA,GPIO_Pin_8)    //接收使能

void RS485_Init(void);

void RS485_SendByte(uint8_t Byte);                       //发送单个字节

void RS485_SendArray(uint8_t *Array, uint16_t Length);    //发送数组式字节

void RS485_recevie(void);

#endif
