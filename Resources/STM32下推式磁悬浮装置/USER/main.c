/**
  ******************************************************************************
  * @file   main.c
  * @version V1.0
  * @author  fengweiguo
			https://blog.csdn.net/qq_46554815?spm=1000.2115.3001.5343
  * @brief   STM32下推式磁悬浮装置主函数
			工程代码仅供参考，请结合自己的硬件。
  ******************************************************************************
*/
#include "sys.h"
#include "delay.h"
#include "usart.h"

#include "bsp_adc.h"
#include "bsp_bluetooth.h"
#include "bsp_drive.h"
#include "bsp_led.h"
#include "bsp_oled_iic.h"
#include "bsp_tim.h"
#include "control.h"
#include "pid.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(void)
{
	u16 t;  
	u16 len;	
	u8 flag_len=0;
	
	delay_init(); 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200); //串口1初始化
	
	LED_Init(); //系统板板载LED初始化
	ADC1_Init(); //ADC1初始化
	coil_PWM_init(3599, 0);
	pid_init();
	motor_init();
	OLED_Init(); //OLED初始化
	OLED_ShowString(35 ,0, "Magnetic", 16);
	OLED_ShowString(0, 16, "HC-05 support", 12);
	OLED_ShowString(0, 52, "by fengweiguo", 12);
	OLED_Refresh_Gram();
	printf("ready\r\n");
	TIM3_Init();//控制PID周期

	while(1)
	{	
		
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			flag_len ++;
			printf("\r\n您发送的消息为:\r\n\r\n");
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			}
			printf("\r\n\r\n");//插入换行
			USART_RX_STA=0;
		}
		
	} /*while(1)*/
} /*main*/



