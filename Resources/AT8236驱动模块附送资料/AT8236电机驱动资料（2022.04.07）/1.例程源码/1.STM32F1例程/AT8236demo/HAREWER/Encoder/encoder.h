#ifndef __ENCODER_H
#define __ENCODER_H
#include "stm32f10x.h"
 /**************************************************************************
���ߣ���Ȥ�Ƽ�����ݸ�����޹�˾
���ڣ�2022.4.2
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
#define ENCODER_TIM_PERIOD (u16)(65535)   //���ɴ���65535 ��ΪF103�Ķ�ʱ����16λ�ġ�
void Encoder_Init_TIM2(void);             //��TIM2��ʼ��Ϊ�������ӿ�ģʽ
void Encoder_Init_TIM3(void);             //��TIM3��ʼ��Ϊ�������ӿ�ģʽ
int Read_Encoder(u8 TIMX);               //��λʱ���ȡ����������
void TIM2_IRQHandler(void);               //��ʱ��3���жϴ�����
void TIM3_IRQHandler(void);               //��ʱ��2���жϴ�����
#endif
