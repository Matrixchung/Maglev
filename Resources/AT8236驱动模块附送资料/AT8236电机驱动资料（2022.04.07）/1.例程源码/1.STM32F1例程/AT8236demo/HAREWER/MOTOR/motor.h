#ifndef __MOTOR_H
#define __MOTOR_H
#include "stm32f10x.h"
 /**************************************************************************
作者：轮趣科技（东莞）有限公司
日期：2022.4.2
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
#define PWMA1   TIM4->CCR1  
#define PWMA2   TIM4->CCR2 

#define PWMB1   TIM4->CCR3  
#define PWMB2   TIM4->CCR4

void Motor_PWM_Init(u16 arr,u16 psc);
void Set_Pwm(int motor_a,int motor_b);

#endif
