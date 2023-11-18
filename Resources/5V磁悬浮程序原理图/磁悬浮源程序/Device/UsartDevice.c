#include "UsartDevice.h"

//---------------------------------------�������´���,֧��printf����,������Ҫѡ��use MicroLIB------------------------------------------	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
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
	
	USART_InitStruct.USART_BaudRate = UsartBaudRate;      //����Usart1 ģʽ
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_Odd;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_9b;
	USART_Init(USART1, &USART_InitStruct);
	
	USART_Cmd(USART1, ENABLE);
	
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);       //�ر���usart1�������ж� ����������ж������ô�����
	
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;        //usart1�ж�ͨ��ʹ�ܲ������ж����ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = UsartPreemptionPriority;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = UsartSubPriority;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_DMACmd(USART1,USART_DMAReq_Tx|USART_DMAReq_Rx,ENABLE);
	//-----------------------------------------------------------------------
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_InitStruct.DMA_BufferSize = SendedBuff[50];                           //buffer��С
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;                           //�豸��ΪĿ��
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;                                 //��ֹ�洢�����洢��
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)&SendedBuff;                     //�洢����ַ
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;              //�洢���ֳ�
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                      //�洢����ַ�Զ�++ ����
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;                                //ʹ����ͨģʽ
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&USART1->DR;                 //�����ַ
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;      //����洢���ֳ�
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             //�����ַ�Զ�++ �ر�
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;                        //�������ȼ�Ϊ�е�

	DMA_DeInit(DMA1_Channel4);
  DMA_Init(DMA1_Channel4,&DMA_InitStruct);
	
	DMA_InitStruct.DMA_BufferSize = 100;                                      //buffer��С
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;                           //�豸��Ϊ����Դ
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;                                 //��ֹ�洢�����洢��
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)&ReceivedBuff;                   //�洢����ַ
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;              //�洢���ֳ�
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                      //�洢����ַ�Զ�++ ����
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;                              //ʹ��ѭ��ģʽ
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&USART1->DR;                 //�����ַ
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;      //����洢���ֳ�
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             //�����ַ�Զ�++ �ر�
	DMA_InitStruct.DMA_Priority = DMA_Priority_Low;                           //�������ȼ�Ϊ��

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
