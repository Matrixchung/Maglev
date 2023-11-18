#include "usart.h"
#include "control.h"
#include "bsp_adc.h"
#include "pid.h"
#include "bsp_drive.h"
#include "bsp_tim.h"


float ADC_ConvertedValueLocal[3] = {0, 0, 0};
int COIL_X=0, COIL_Y=0, COIL_Z=0;
int ADC_VALUE[3] = {0,0,0};

void limiting_PWM(void)
{
	int amplitude=3500; //PWM满幅是3600 限制在3500
    
	if(COIL_X < -amplitude) 
		COIL_X = -amplitude;	
	if(COIL_X > amplitude)  
		COIL_X = amplitude;	
	
	if(COIL_Y < -amplitude) 
		COIL_Y = -amplitude;	
	if(COIL_Y > amplitude)  
		COIL_Y = amplitude;		
}



int myabs(int num)
{
	int temp;
	if(num < 0)
		temp = -num;  
	else 
		temp = num;
	return temp;
}


/**
  * @brief  电机PWM幅值
  * @param  moto1：电机1PWM
  * @param  moto2：电机2PWM
  * @retval 无
  */
void set_PWM(int coil1,int coil2)
{	
	if(coil1 > 0)			
	{
		XIN1=0;
		XIN2=1;			
	}
	else 	          
	{
		XIN1=1;
		XIN2=0;
	}
	PWMX = myabs(coil1);
	
	if(coil2<0)	
	{
		YIN1=0;
		YIN2=1;
	}
	else 
	{
		YIN1=1;
		YIN2=0;
	}
	PWMY = myabs(coil2);
}


/**
  * @brief  控制函数，PID整合函数
  * @param 	无
  * @retval 0
  */

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除中断
		
		//传递ADC值
		ADC_VALUE[0] = ADC_ConvertedValue[0];
		ADC_VALUE[1] = ADC_ConvertedValue[1];
		ADC_VALUE[2] = ADC_ConvertedValue[2];
		
		//PID计算PWM
		COIL_X = x_pid_calc(ADC_VALUE[0]);
		COIL_Y = y_pid_calc(ADC_VALUE[1]);
		limiting_PWM(); //限幅
		
		//没有浮子时关闭线圈
		if(ADC_VALUE[2] > 3660)
		{
			COIL_X = 0;
			COIL_Y = 0;
		}
		
		//设置线圈PWM
		set_PWM(COIL_X, COIL_Y);
		
	}
}


