#include "App.h"
u32 SystemTimer;
u32 AdcValue0Sum,AdcValue1Sum;
extern vu16 AdcValue0,AdcValue1;
extern Pid xpid,ypid;
void App_Init(void){
	SystemTimer = 0;
}
void Step_1_Start(void){
	if(SystemTimer > STEP_1_TIMER){	
		return;
	}
	if(SystemTimer == STEP_1_TIMER){
		AdcValue0Sum = AdcValue0Sum/STEP_1_TIMER;
		AdcValue1Sum = AdcValue1Sum/STEP_1_TIMER;
		
		SystemTimer++;
		return;
	}	
	if(SystemTimer == 0){
		AdcValue0Sum = 0;
		AdcValue1Sum = 0;
	}
	AdcValue0Sum += AdcValue0;
	AdcValue1Sum += AdcValue1;	
	SystemTimer++;	
	Led_SystemStart();
}
void Step_2_Stablilization(void){
	s16 output0,output1;	
	if(SystemTimer <= STEP_1_TIMER){
		return;
	}	
	Led_SysTick();	
	output0 = PID_Calculate(AdcValue0- AdcValue0Sum,&xpid);
	output1 = PID_Calculate(AdcValue1- AdcValue1Sum,&ypid);
	

	
	if(output0 > 0){
		PWM_PB_6_SetPower(output0);
		PWM_PB_7_SetPower(0);
	}else{
		PWM_PB_6_SetPower(0);
		PWM_PB_7_SetPower(-output0);		
	}
	
	if(output1 > 0){
		PWM_PB_8_SetPower(output1);
		PWM_PB_9_SetPower(0);
	}else{
		PWM_PB_8_SetPower(0);
		PWM_PB_9_SetPower(-output1);		
	}
}

void App_Run(void){
	ADC_GetData();
	ADC_StartRun();
	Step_1_Start();
	Step_2_Stablilization();
}
