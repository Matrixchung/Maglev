/**
  ******************************************************************************
  * @file    bsp_bluetooth.c
  * @version V1.0
  * @author  fengweiguo
  * @brief   ����ͨ��Դ�ļ�
  ******************************************************************************
*/

#include "bsp_bluetooth.h"
#include "bsp_led.h"
#include "pid.h"
#include <math.h>




/**
  * @brief  ����2��ʼ��
  * @param  bound��������
  * @retval ��
  */
void uart2_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//USART2_TX  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
   
	//USART2_RX	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Usart2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //������
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound; //���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No; //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //�������ڽ����ж�
	USART_Cmd(USART2, ENABLE); //ʹ�ܴ���2 
}


