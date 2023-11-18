#include "motor.h"
 /**************************************************************************
作者：轮趣科技（东莞）有限公司
日期：2022.4.2
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
void Motor_PWM_Init(u16 arr,u16 psc)        
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
  GPIO_InitTypeDef GPIO_InitStruct;                             
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);   //使能定时器4时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //使能GPIOB的时钟
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;          //复用推挽输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;   //PB6 7 8 9
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	TIM_TimeBaseInitStruct.TIM_Period = arr;              //设定计数器自动重装值 
	TIM_TimeBaseInitStruct.TIM_Prescaler  = psc;          //设定预分频器
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;         //设置时钟分割
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);       //初始化定时器
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //选择PWM1模式
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStruct.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //设置输出极性
	TIM_OC1Init(TIM4,&TIM_OCInitStruct);                       //初始化输出比较参数
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //选择PWM1模式
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStruct.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //设置输出极性
	TIM_OC2Init(TIM4,&TIM_OCInitStruct);                       //初始化输出比较参数
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //选择PWM1模式
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStruct.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //设置输出极性
	TIM_OC3Init(TIM4,&TIM_OCInitStruct);                       //初始化输出比较参数
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //选择PWM1模式
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStruct.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //设置输出极性
	TIM_OC4Init(TIM4,&TIM_OCInitStruct);                       //初始化输出比较参数
	
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);   //CH1使能预装载寄存器
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);   //CH2使能预装载寄存器
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);   //CH3使能预装载寄存器
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);   //CH4使能预装载寄存器
	
	TIM_ARRPreloadConfig(TIM4, ENABLE);                //使能TIM4在ARR上的预装载寄存器
	
	TIM_Cmd(TIM4,ENABLE);                              //使能定时器4
}

void Set_Pwm(int motor_a,int motor_b)
{
    	if(motor_a<0)			PWMA1=7200,PWMA2=7200+motor_a;
			else 	            PWMA2=7200,PWMA1=7200-motor_a;
		
		  if(motor_b<0)			PWMB1=7200,PWMB2=7200+motor_b;
			else 	            PWMB2=7200,PWMB1=7200-motor_b;	
}
