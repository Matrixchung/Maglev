#include "UsartDevice.h"

//---------------------------------------加入以下代码,支持printf函数,而不需要选择use MicroLIB------------------------------------------	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 
//----------------------------------------------------------------------------------------------------------------------------------------

u8 ReceivedBuff[100],SendedBuff[51];

void GPIO_Usart1Config(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void Usart1_Config(void){
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	DMA_InitTypeDef DMA_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_Usart1Config();
	
	USART_InitStruct.USART_BaudRate = UsartBaudRate;      //配置Usart1 模式
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_Odd;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_9b;
	USART_Init(USART1, &USART_InitStruct);
	
	USART_Cmd(USART1, ENABLE);
	
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);       //关闭了usart1到邮漳中断 所以下面的中断设置用处不大
	
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;        //usart1中断通道使能并设置中断优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = UsartPreemptionPriority;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = UsartSubPriority;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_DMACmd(USART1,USART_DMAReq_Tx|USART_DMAReq_Rx,ENABLE);
	//-----------------------------------------------------------------------
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_InitStruct.DMA_BufferSize = SendedBuff[50];                           //buffer大小
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;                           //设备作为目的
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;                                 //禁止存储器到存储器
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)&SendedBuff;                     //存储器地址
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;              //存储器字长
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                      //存储器地址自动++ 开启
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;                                //使用普通模式
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&USART1->DR;                 //外设地址
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;      //外设存储器字长
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             //外设地址自动++ 关闭
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;                        //设置优先级为中等

	DMA_DeInit(DMA1_Channel4);
  DMA_Init(DMA1_Channel4,&DMA_InitStruct);
	
	DMA_InitStruct.DMA_BufferSize = 100;                                      //buffer大小
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;                           //设备作为数据源
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;                                 //禁止存储器到存储器
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)&ReceivedBuff;                   //存储器地址
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;              //存储器字长
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                      //存储器地址自动++ 开启
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;                              //使用循环模式
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&USART1->DR;                 //外设地址
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;      //外设存储器字长
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             //外设地址自动++ 关闭
	DMA_InitStruct.DMA_Priority = DMA_Priority_Low;                           //设置优先级为低

	DMA_DeInit(DMA1_Channel5);
  DMA_Init(DMA1_Channel5,&DMA_InitStruct);
	DMA_Cmd(DMA1_Channel5,ENABLE);
}
void Usart1_SendData(void){
	DMA_Cmd(DMA1_Channel4,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel4,SendedBuff[50]);
	DMA_Cmd(DMA1_Channel4,ENABLE);
}
int SendManager(void){
	if(DMA_GetCurrDataCounter(DMA1_Channel4)==0x0000){
		Usart1_SendData();
		return 0;
	}
	return 1;
}


void Usart1_ReceivedManager(void){
	u8 i;
	for(i=0;i<100;i++){
		
	}
}
