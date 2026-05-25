#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Serial.h"
#include "MyI2C.h"
#include "RS485.h"
#include "PCA9555.h"
#include "RS485ControlPCA9555.h"
#include "MyCAN.h"

uint32_t TxID = 0x555;
uint8_t TxLength = 4;
uint8_t TxData[4] = {0x11, 0x22, 0x33, 0x44};

uint32_t RxID;
uint8_t RxLength;
uint8_t RxData[4];

int main(void)
{
	MyCAN_Init();
	Serial_Init();
	MyI2C_Init();
	RS485_Init();
	PCA9555_Init();
	RxBuffer[0] = 0x99;
//	while (1)
//	{
//		RS485_CtrlWrite_PCA();
////		MyCAN_Transmit(TxID, TxLength, TxData);
//		Delay_ms(500);
////		if (MyCAN_ReceiveFlag())
////		{
////			MyCAN_Receive(&RxID, &RxLength, RxData);
////			RS485_SendArray(RxData, sizeof(RxData));
////		}
//		
//	}
while (1)
{
    RS485_recevie();     // 打开接收处理
    
    static uint16_t count = 0;
    if(count++ % 2000 == 0)   // 每2秒发一次测试帧（可选）
    {
        uint8_t test[] = {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55};
        RS485_SendArray(test, 6);
        printf("Sent test: AA 55...\r\n");
    }
    
    Delay_ms(1);
}
}
