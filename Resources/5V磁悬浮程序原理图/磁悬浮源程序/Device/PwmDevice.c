#include "PwmDevice.h"

void Pwm_PB_6_7_8_9_10_Config(FunctionalState NewState){
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
		TIM_Cmd(TIM4,DISABLE);
		TIM_Cmd(TIM2,DISABLE);
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_Init(GPIOB,&GPIO_InitStruct);
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2,ENABLE);
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);         
	
		TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInitStruct.TIM_Period = TIME_Period;            
		TIM_TimeBaseInitStruct.TIM_Prescaler = TIME_Prescaler;      
		TIM_TimeBaseInit(TIM2 ,&TIM_TimeBaseInitStruct);
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);        
	
		TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInitStruct.TIM_Period = TIME_Period;           
		TIM_TimeBaseInitStruct.TIM_Prescaler = TIME_Prescaler;      
		TIM_TimeBaseInit(TIM4 ,&TIM_TimeBaseInitStruct);
		
		TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC1Init(TIM4, &TIM_OCInitStruct);
		TIM_OC2Init(TIM4, &TIM_OCInitStruct);
		TIM_OC3Init(TIM4, &TIM_OCInitStruct);
		TIM_OC4Init(TIM4, &TIM_OCInitStruct);
		TIM_OC3Init(TIM2, &TIM_OCInitStruct);
	
		TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
		TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);
		TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
		TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);
		TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);
		TIM_ARRPreloadConfig(TIM4,ENABLE);
		TIM_ARRPreloadConfig(TIM2,ENABLE);
	
		TIM_SetCompare1(TIM4,0);
		TIM_SetCompare2(TIM4,0);
		TIM_SetCompare3(TIM4,0);
		TIM_SetCompare4(TIM4,0);
		TIM_SetCompare3(TIM2,0);
		
		TIM_Cmd(TIM2,ENABLE);
		TIM_Cmd(TIM4,ENABLE);
  }
  else
  {
		TIM_Cmd(TIM2,DISABLE);
		TIM_Cmd(TIM4,DISABLE);
		TIM_DeInit(TIM2);
		TIM_DeInit(TIM4);
  }
}
void PWM_PB_6_SetPower(u16 power){
	if(power > 899){power = 899;}
	TIM_SetCompare1(TIM4,power);
}
void PWM_PB_7_SetPower(u16 power){
	if(power > 899){power = 899;}
	TIM_SetCompare2(TIM4,power);
}
void PWM_PB_8_SetPower(u16 power){
	if(power > 899){power = 899;}
	TIM_SetCompare3(TIM4,power);
}
void PWM_PB_9_SetPower(u16 power){
	if(power > 899){power = 899;}
	TIM_SetCompare4(TIM4,power);
}
void PWM_PB_10_SetPower(u16 power){
	if(power > 899){power = 899;}
	TIM_SetCompare3(TIM2,power);
}
