#include "RS485.h"
#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void RS485_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;           //控制数据的接收、发送
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);					 //默认打开接收模式
}

void RS485_SendByte(uint8_t Byte)                        //发送单个字节
{
//	RS485_send();                                         //开启发送模式，关闭接收模式
	Serial_SendByte(Byte);
//	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//	RS485_receive();                                        //默认打开接收机
}

void RS485_SendArray(uint8_t *Array, uint16_t Length)
{
    if(Length == 0) return;

    // 进入发送模式
    GPIO_SetBits(GPIOA, GPIO_Pin_8);
    Delay_ms(10);                    // 大延时，确保DE稳定

    // 关闭接收中断
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);

    // 发送数据
    for(uint16_t i = 0; i < Length; i++)
    {
        Serial_SendByte(Array[i]);
    }

    // 等待发送完成
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

    Delay_ms(10);                    // 重要延时

    // 切换回接收模式
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    
    Delay_ms(5);
}
void RS485_recevie(void)
{
    if (RxBufferIndex > 0)
    {
        printf("Buffer: %d bytes | First=0x%02X\r\n", RxBufferIndex, RxBuffer[0]);
        
        RS485_CtrlWrite_PCA();   // 处理命令
        
        // 处理完强制清空
        RxBufferIndex = 0;
    }
}