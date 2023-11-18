#ifndef __ENCODER_H
#define __ENCODER_H
#include "stm32f10x.h"
 /**************************************************************************
作者：轮趣科技（东莞）有限公司
日期：2022.4.2
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
#define ENCODER_TIM_PERIOD (u16)(65535)   //不可大于65535 因为F103的定时器是16位的。
void Encoder_Init_TIM2(void);             //把TIM2初始化为编码器接口模式
void Encoder_Init_TIM3(void);             //把TIM3初始化为编码器接口模式
int Read_Encoder(u8 TIMX);               //单位时间读取编码器计数
void TIM2_IRQHandler(void);               //定时器3的中断处理函数
void TIM3_IRQHandler(void);               //定时器2的中断处理函数
#endif
