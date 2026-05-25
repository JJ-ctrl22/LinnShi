#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "PCA9555.h"
 
 //address 从机地址 reg：寄存器地址 data：数据 
 // PA0 = 0  PA1 = 0  PA2 = 0  控制P0
 //从机地址
 //写操作：0x20 << 1 | 0 = 0x40
 //读操作：0x20 << 1 | 1 = 0x41
 
 // PA0 = 1  PA1 = 0  PA2 = 0  控制P1
 //从机地址
 //写操作：0x24 << 1 | 0 = 0x48
 //读操作：0x24 << 1 | 1 = 0x49
 
 void PCA9555_Init(void)
 {
	uint8_t n = 0;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//开启GPIOB的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					
	
	/*设置默认电平*/
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);			//设置PB1引脚初始化后默认为低电平
	for(n = 0;n < 8 ; n++)
	{
		PCA9555_SetPinMode(0,n,0);
		PCA9555_WritePin(0,n,0);
	}
	for(n = 0;n < 8 ; n++)
	{
		PCA9555_SetPinMode(1,n,0);
		PCA9555_WritePin(1,n,0);
	}
 }
 
void PCA9555_WriteReg(uint8_t address, uint8_t reg, uint8_t data)        //写寄存器函数
{
    MyI2C_Start();
    MyI2C_SendByte(address);       // 从机地址
    MyI2C_ReceiveAck();            // 忽略应答（或检查）
    MyI2C_SendByte(reg);           // 寄存器地址
    MyI2C_ReceiveAck();
    MyI2C_SendByte(data);          // 数据
    MyI2C_ReceiveAck();
    MyI2C_Stop();
}

//address ：0x40 读P0的值  0x48 读P1的值
uint8_t PCA9555_ReadReg(uint8_t address,uint8_t reg)                      //读寄存器函数
{
    uint8_t data;
    MyI2C_Start();
    MyI2C_SendByte(address);	   // 写地址
    MyI2C_ReceiveAck();
    MyI2C_SendByte(reg);           // 寄存器地址
    MyI2C_ReceiveAck();
    MyI2C_Start();                 // 重复起始
    MyI2C_SendByte(address + 1);   // 读地址
    MyI2C_ReceiveAck();
    data = MyI2C_ReceiveByte();    // 读取数据
    MyI2C_SendAck(1);              // 最后发送 NACK
    MyI2C_Stop();
    return data;
}

// 设置单个引脚方向（0=输出, 1=输入）
void PCA9555_SetPinMode(uint8_t port, uint8_t pin, uint8_t mode) 
{
	if(port == 0)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	}
	else
	{
//		GPIO_SetBits(GPIOB, GPIO_Pin_1);
	}
    uint8_t writeAddr = (port == 0) ? PCA9555_WRITE_P0 : PCA9555_WRITE_P1;
    uint8_t configReg = (port == 0) ? 0x06 : 0x07;
    uint8_t config = PCA9555_ReadReg(writeAddr, configReg);
    if (mode == 0)
	{
        config &= ~(1 << pin);   // 输出
    }
	else
	{
        config |= (1 << pin);    // 输入
    }
    PCA9555_WriteReg(writeAddr, configReg, config);
}

// 写单个引脚输出电平（必须先配置为输出）
void PCA9555_WritePin(uint8_t port, uint8_t pin, uint8_t level) 
{
	if(port == 0)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	}
	else
	{
//		GPIO_SetBits(GPIOB, GPIO_Pin_1);
	}
    uint8_t writeAddr = (port == 0) ? PCA9555_WRITE_P0 : PCA9555_WRITE_P1;
    uint8_t outReg = (port == 0) ? 0x02 : 0x03;
    uint8_t out = PCA9555_ReadReg(writeAddr, outReg);
    if (level) 
	{
        out |= (1 << pin);
    } 
	else 
	{
        out &= ~(1 << pin);
    }
    PCA9555_WriteReg(writeAddr, outReg, out);
}

//读取引脚电平的函数
uint8_t PCA9555_ReadPin(uint8_t port, uint8_t pin) 
{
    uint8_t writeAddr = (port == 0) ? PCA9555_WRITE_P0 : PCA9555_WRITE_P1;
    uint8_t inputReg = (port == 0) ? 0x00 : 0x01;
    uint8_t val = PCA9555_ReadReg(writeAddr, inputReg);
    return (val >> pin) & 0x01;
}
