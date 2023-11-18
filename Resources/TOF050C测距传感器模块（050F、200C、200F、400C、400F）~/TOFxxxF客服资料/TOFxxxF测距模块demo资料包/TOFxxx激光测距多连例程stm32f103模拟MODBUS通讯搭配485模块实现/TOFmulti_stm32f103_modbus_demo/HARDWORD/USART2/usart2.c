#include "sys.h"
#include "usart2.h"	
#include "stm32f10x.h"
            





void USART2_Init(u32 bound){
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART2，GPIOA时钟
 	USART_DeInit(USART2);  //复位串口2
	//USART2_TX   PA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
    //USART2_RX	  PA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

   //Usart2 NVIC 配置

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART2, &USART_InitStructure); //初始化串口
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART2, ENABLE);                    //使能串口 

}



unsigned char ucRxFinish=0;
static unsigned char ucCnt=0,ucLen=0;
unsigned char ucRxData[100];
void USART2_IRQHandler(void)                	//串口1中断服务程序
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

uint16_t ModbusCRC(uint8_t *ptr,uint16_t ucLen)//CRC校验
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