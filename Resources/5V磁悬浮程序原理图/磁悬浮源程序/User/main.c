#include "stm32f10x.h"
#include "AdcDevice.h"
#include "ClkDevice.h"
#include "UsartDevice.h"
#include "PwmDevice.h"
#include "LedDevice.h"
#include "PID.h"
#include "App.h"

Pid xpid,ypid;
void Delay(u32 count){
   u32 i=0;
   for(;i<count;i++);
 }

int main(void){
	Usart1_Config();                     
	ADC_Config();                        
	Pwm_PB_6_7_8_9_10_Config(ENABLE);    
	Delay(10000);                        
	xpid.Kp = 1.0;  
	xpid.Ki = 0.0005;
	xpid.Kd = 3.5;   
	ypid.Kp = 1.0;  
	ypid.Ki = 0.0005;
	ypid.Kd = 3.5;   	
	App_Init();                          
	Led_Config(ENABLE);                  
	SystemTick_Config(ENABLE);           
		
	while(1){
		if((xpid.transmition == _false)&&(ypid.transmition == _false)){
			printf("%5d|%5d\r\n",xpid.error_preerror,ypid.error_preerror);
			xpid.transmition = _true;		
			ypid.transmition = _true;		

		}
	}
}

