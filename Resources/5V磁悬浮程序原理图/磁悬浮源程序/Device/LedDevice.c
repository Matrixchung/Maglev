#include "LedDevice.h"

u16 LedFlag = 0x0000;
u32 SysTimer = 0;

void Led_SystemTickConfig(FunctionalState NewState){
	GPIO_InitTypeDef GPIO_InitStruct;
	assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOC, &GPIO_InitStruct);
		
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
  }
  else
  {
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC,ENABLE);
		GPIO_DeInit(GPIOC);
  }
}
void Led_SystemTick(void){
	if((LedFlag&LedSysTickMask)!=0){
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		LedFlag &= ~LedSysTickMask;
	}else{
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		LedFlag |= LedSysTickMask;
	} 	
}
void Led_SysTick(void){
	if(SysTimer <= (500/LedSysTickFrequency)){
		SysTimer++;
		return ;
	}
	SysTimer = 0;
	Led_SystemTick();
}
void Led_SystemStart(void){
	if(SysTimer <= (500/16)){
		SysTimer++;
		return ;
	}
	SysTimer = 0;
	Led_SystemTick();
}


void Led_Config(FunctionalState NewState){
	assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
		Led_SystemTickConfig(ENABLE);
  }
  else
  {
		Led_SystemTickConfig(DISABLE);
  }
}
