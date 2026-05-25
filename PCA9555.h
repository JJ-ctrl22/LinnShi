#ifndef __PCA9555_H_
#define __PCA9555_H_

#include "stm32f10x.h"                  // Device header

#define PCA9555_ADDR_P0  0x20   // 7Œªµÿ÷∑£¨A2=A1=A0=0
#define PCA9555_ADDR_P1  0x20   // A2=0,A1=0,A0=1

#define PCA9555_WRITE_P0 (PCA9555_ADDR_P0 << 1 | 0)  // 0x40
#define PCA9555_READ_P0  (PCA9555_ADDR_P0 << 1 | 1)  // 0x41
#define PCA9555_WRITE_P1 (PCA9555_ADDR_P1 << 1 | 0)  // 0x48
#define PCA9555_READ_P1  (PCA9555_ADDR_P1 << 1 | 1)  // 0x49

void PCA9555_Init(void);

void PCA9555_WriteReg(uint8_t address, uint8_t reg, uint8_t data);

uint8_t PCA9555_ReadReg(uint8_t address,uint8_t reg);

void PCA9555_SetPinMode(uint8_t port, uint8_t pin, uint8_t mode);

void PCA9555_WritePin(uint8_t port, uint8_t pin, uint8_t level);

uint8_t PCA9555_ReadPin(uint8_t port, uint8_t pin) ;

#endif
