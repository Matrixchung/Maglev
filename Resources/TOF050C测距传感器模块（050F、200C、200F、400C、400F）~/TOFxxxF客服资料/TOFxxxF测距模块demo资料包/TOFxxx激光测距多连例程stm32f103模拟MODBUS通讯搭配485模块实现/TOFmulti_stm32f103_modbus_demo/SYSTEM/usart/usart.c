#include "sys.h"
#include "usart.h"	  
            
 struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
//_sys_exit(int x) 
//{ 
//	x = x; 
//} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
} 	


void UART1_Init(u32 bound){
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
 	USART_DeInit(USART1);  //复位串口1
	//USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
    //USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

   //Usart1 NVIC 配置

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART1, ENABLE);                    //使能串口 

}

uint8_t RX_buffer[100];
uint8_t counter=0;
uint8_t num=0;
uint8_t RX_flag=0;

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
		{
//			if(counter==0)
//			{
//				Res =USART_ReceiveData(USART1);
//				if(Res==0x50) 
//				{
//					RX_buffer[counter]=Res;
//					counter++;		
//				}
//			}
//			else if(counter==1)
//						{
//							RX_buffer[counter]=USART_ReceiveData(USART1);
//							counter++;
//						}
//			else if(counter==2)
//						{
//							RX_buffer[counter]=USART_ReceiveData(USART1);
//							num=RX_buffer[counter];
//							counter++;
//						}
//			else if(counter<=(num+5))
//						{
//							RX_buffer[counter]=USART_ReceiveData(USART1);
//							counter++;
//				      if(counter==(17))
//							{
//								if(((RX_buffer[15]<<8)|RX_buffer[16])==ModbusCRC( RX_buffer, 15))
//								{RX_flag=1; counter=0;}
//								else
//								{counter=0;}							
//							}
//						}

		}
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}   		 
 
	

//void USART1_Send(uint8_t *data,uint8_t len)
//{
//	uint8_t i;
//	USART_ClearFlag(USART1,USART_FLAG_TC);
//	for(i=0;i<len;i++)
//	{
//		USART_SendData(USART1,*(data+i));
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
//	}
//}

uint16_t ModbusCRC(uint8_t *ptr,uint16_t len)
{
	uint8_t i;
	uint16_t j,crc=0xffff;
	
	i=i;	
	
	for(int n=0;n<len;n++)
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


