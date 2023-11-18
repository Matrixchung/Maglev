#include "motor.h"
 /**************************************************************************
���ߣ���Ȥ�Ƽ�����ݸ�����޹�˾
���ڣ�2022.4.2
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
void Motor_PWM_Init(u16 arr,u16 psc)        
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
  GPIO_InitTypeDef GPIO_InitStruct;                             
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);   //ʹ�ܶ�ʱ��4ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //ʹ��GPIOB��ʱ��
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;          //�����������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;   //PB6 7 8 9
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	TIM_TimeBaseInitStruct.TIM_Period = arr;              //�趨�������Զ���װֵ 
	TIM_TimeBaseInitStruct.TIM_Prescaler  = psc;          //�趨Ԥ��Ƶ��
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIM���ϼ���ģʽ
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;         //����ʱ�ӷָ�
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);       //��ʼ����ʱ��
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //ѡ��PWM1ģʽ
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStruct.TIM_Pulse = 0;                            //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //�����������
	TIM_OC1Init(TIM4,&TIM_OCInitStruct);                       //��ʼ������Ƚϲ���
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //ѡ��PWM1ģʽ
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStruct.TIM_Pulse = 0;                            //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //�����������
	TIM_OC2Init(TIM4,&TIM_OCInitStruct);                       //��ʼ������Ƚϲ���
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //ѡ��PWM1ģʽ
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStruct.TIM_Pulse = 0;                            //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //�����������
	TIM_OC3Init(TIM4,&TIM_OCInitStruct);                       //��ʼ������Ƚϲ���
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //ѡ��PWM1ģʽ
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStruct.TIM_Pulse = 0;                            //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //�����������
	TIM_OC4Init(TIM4,&TIM_OCInitStruct);                       //��ʼ������Ƚϲ���
	
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);   //CH1ʹ��Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);   //CH2ʹ��Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);   //CH3ʹ��Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);   //CH4ʹ��Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM4, ENABLE);                //ʹ��TIM4��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM4,ENABLE);                              //ʹ�ܶ�ʱ��4
}

void Set_Pwm(int motor_a,int motor_b)
{
    	if(motor_a<0)			PWMA1=7200,PWMA2=7200+motor_a;
			else 	            PWMA2=7200,PWMA1=7200-motor_a;
		
		  if(motor_b<0)			PWMB1=7200,PWMB2=7200+motor_b;
			else 	            PWMB2=7200,PWMB1=7200-motor_b;	
}
