#include "TIMER.h"
#include "usart.h"

//通用定时器2初始化函数，arr:自动重装载值，psc:预分频系数，默认定时时钟为72MHZ时，两者共同决定定时中断时间
void TIM2_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue; //定义一个定时中断的结构体
	NVIC_InitTypeDef NVIC_InitStrue; //定义一个中断优先级初始化的结构体
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //使能通用定时器2时钟
	
	TIM_TimeBaseInitStrue.TIM_Period=arr; //计数模式为向上计数时，定时器从0开始计数，计数超过到arr时触发定时中断服务函数
	TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //预分频系数，决定每一个计数的时长
	TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //计数模式：向上计数
	TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //一般不使用，默认TIM_CKD_DIV1
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStrue); //根据TIM_TimeBaseInitStrue的参数初始化定时器TIM2
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //使能TIM2中断，中断模式为更新中断：TIM_IT_Update
	
	NVIC_InitStrue.NVIC_IRQChannel=TIM2_IRQn; //属于TIM2中断
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE; //中断使能
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1; //抢占优先级为1级，值越小优先级越高，0级优先级最高
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=1; //响应优先级为1级，值越小优先级越高，0级优先级最高
	NVIC_Init(&NVIC_InitStrue); //根据NVIC_InitStrue的参数初始化VIC寄存器，设置TIM2中断
	
	TIM_Cmd(TIM2, ENABLE); //使能定时器TIM2
}

//void TIM2_IRQHandler()
//{
//  if(TIM_GetITStatus(TIM2, TIM_IT_Update)==1) //当发生中断时状态寄存器(TIMx_SR)的bit0会被硬件置1
//	{
//	  PAout(4)=!PAout(4); //LED灯(A4引脚)状态取反，该函数封装在库函数"sys.h"中
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //状态寄存器(TIMx_SR)的bit0置0
//	}
//}

void TIM2_PWM_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义一个引脚初始化的结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue; //定义一个定时中断的结构体	
	TIM_OCInitTypeDef TIM_OCInitTypeStrue; //定义一个PWM输出的结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能GPIOA时钟，在STM32中使用IO口前都要使能对应时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //使能通用定时器2时钟，A0引脚对应TIM2CHN1
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;//引脚0
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; //复用推挽输出模式，定时器功能为A0引脚复用功能
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; //定义该引脚输出速度为50MHZ
  GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化引脚GPIOA0
	 
	TIM_TimeBaseInitStrue.TIM_Period=arr; //计数模式为向上计数时，定时器从0开始计数，计数超过到arr时触发定时中断服务函数
	TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //预分频系数，决定每一个计数的时长
	TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //计数模式：向上计数
	TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //一般不使用，默认TIM_CKD_DIV1
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStrue); //根据TIM_TimeBaseInitStrue的参数初始化定时器TIM2
	
	TIM_OCInitTypeStrue.TIM_OCMode=TIM_OCMode_PWM2; //PWM模式2，当定时器计数大于TIM_Pulse时，定时器对应IO输出有效电平
	TIM_OCInitTypeStrue.TIM_OCPolarity=TIM_OCNPolarity_High; //输出有效电平为高电平
	TIM_OCInitTypeStrue.TIM_OutputState=TIM_OutputState_Enable; //使能PWM输出
	TIM_OCInitTypeStrue.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OC1Init(TIM2, &TIM_OCInitTypeStrue); //根TIM_OCInitTypeStrue参数初始化定时器2通道1

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable); //CH1预装载使能
	
	TIM_ARRPreloadConfig(TIM2, ENABLE); //CH1预装载使能
	
	TIM_Cmd(TIM2, ENABLE); //使能定时器TIM2
}
TIM_ICInitTypeDef  TIM3_ICInitStructure; //定义一个定时器捕获输入初始化的结构体
//定时器3通道3输入捕获配置初始化函数
void TIM3_Cap_Init(u16 arr,u16 psc)
{	 
	//定义相关结构体
	GPIO_InitTypeDef GPIO_InitStructure; //定义一个GPIO初始化的结构体
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; //定义一个定时器初始化的结构体
	TIM_ICInitTypeDef  TIM3_ICInitStructure; //定义一个定时器捕获输入初始化的结构体
 	NVIC_InitTypeDef NVIC_InitStructure; //定义一个中断优先级初始化的结构体

	//使能相关时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能TIM3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIOB时钟
	
	//初始化GPIOB0
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  //PB0 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //下拉输入，默认低电平，可以被外部电压拉高为高电平
	GPIO_Init(GPIOB, &GPIO_InitStructure); //根据GPIO_InitStructure的参数初始化GPIOB0
	GPIO_ResetBits(GPIOB,GPIO_Pin_0); //初始化PB0为低电平
	
	//初始化定时器TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct的参数初始化定时器TIM3
  
	//初始化TIM3输入捕获参数
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3; //输入捕获通道3
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI3上
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频为不分频，分频决定几个捕获事件触发中断服务函数
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器为不滤波
	TIM_ICInit(TIM3, &TIM3_ICInitStructure); //根TIM3_ICInitStructure参数初始化定时器TIM3输入捕获
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级3级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //中断使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC3,ENABLE);//允许TIM3更新中断|允许TIM3通道3捕获中断	
  TIM_Cmd(TIM3, ENABLE ); 	//使能定时器3
}

u8 falling_flag=0; //下降沿捕获标志   				
u16	TIM3CH3_CAPTURE_VAL; //输入捕获计数值

//定时器5中断服务程序	 
//void TIM3_IRQHandler(void)
//{ 
//	if(TIM_GetITStatus(TIM3, TIM_IT_CC3)&&falling_flag==0) //发生捕获事件&&捕获事件为上升沿（按键按下，B0接通高电平）
//		{			
//			TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Falling); //设置为下降沿捕获
//			falling_flag=1; //更新标志
//			TIM3->CNT=0; //清除计数
//			TIM_ClearITPendingBit(TIM3, TIM_IT_CC3|TIM_IT_Update); //清除中断标志位，定时器重新计数	
//		}	
//  else if(TIM_GetITStatus(TIM3, TIM_IT_CC3)&&falling_flag==1) //发生捕获事件&&捕获事件为下降沿
//  	{		
//	    TIM3CH3_CAPTURE_VAL=TIM_GetCapture3(TIM3); //统计上升沿事件后的高电平持续时间（按键长按时间）
//			printf("TIM3CH3_CAPTURE_VAL:%d ms\r\n",TIM3CH3_CAPTURE_VAL/10); //向串口调试助手发送统计时间，注意添加时间上限为1秒，超过1秒会重新计数
//      TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Rising); //设置为上升沿捕获
//			falling_flag=0; //更新标志		
//			TIM3->CNT=0; //清除计数
//      TIM_ClearITPendingBit(TIM3, TIM_IT_CC3|TIM_IT_Update); //清除中断标志位		
//	  }
//}
