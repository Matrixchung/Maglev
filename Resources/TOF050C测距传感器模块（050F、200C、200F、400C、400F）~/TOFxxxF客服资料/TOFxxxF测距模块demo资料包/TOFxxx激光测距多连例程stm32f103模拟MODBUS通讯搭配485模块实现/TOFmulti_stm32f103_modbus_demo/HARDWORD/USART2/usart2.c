#include "sys.h"
#include "usart2.h"	
#include "stm32f10x.h"
            





void USART2_Init(u32 bound){
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART2��GPIOAʱ��
 	USART_DeInit(USART2);  //��λ����2
	//USART2_TX   PA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART2_RX	  PA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

   //Usart2 NVIC ����

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART2, &USART_InitStructure); //��ʼ������
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 

}



unsigned char ucRxFinish=0;
static unsigned char ucCnt=0,ucLen=0;
unsigned char ucRxData[100];
void USART2_IRQHandler(void)                	//����1�жϷ������
{
	unsigned char temp=0;
  u16 C=0;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  
		{
		  temp=USART_ReceiveData(USART2);
			
			if(ucCnt==0)
					ucRxData[ucCnt++]=temp;
		  else	if((ucCnt==1)&(temp==0x03))
					ucRxData[ucCnt++]=temp;
			else if(ucCnt==2)
					{ucRxData[ucCnt++]=temp; ucLen=ucRxData[2];}
			else if((ucCnt>2)&(ucCnt<=(ucLen+4)))
					ucRxData[ucCnt++]=temp;
			if(ucCnt==(ucLen+5))
			{  C=ModbusCRC(ucRxData,ucLen+3);
				if(C==((ucRxData[ucLen+3]<<8)|ucRxData[ucLen+4]))
					{ucRxFinish=1;	ucCnt=0;ucLen=0;}			
				else 
					{ucCnt=0;ucLen=0;}				
			}
			
		}
	  USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}   		 
 
	

void USART2_Send(unsigned char *data,uint8_t ucLen)
{
	uint8_t i;
	USART_ClearFlag(USART2,USART_FLAG_TC);
	for(i=0;i<ucLen;i++)
	{
		USART_SendData(USART2,*(data+i));
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
	}
}

uint16_t ModbusCRC(uint8_t *ptr,uint16_t ucLen)//CRCУ��
{
	uint8_t i;
	uint16_t j,crc=0xffff;
	
	i=i;	
	
	for(int n=0;n<ucLen;n++)
	{
		crc=ptr[n]^crc;
		for(int i=0;i<8;i++)
		if(crc&0x01)
		{
			crc=crc>>1;
			crc=crc^0xa001;
		}
		else
		{
			crc=crc>>1;	
		}		
	}

	j=crc>>8;
	j=j|(crc<<8);
	return j;

}