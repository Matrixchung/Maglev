#include "ClkDevice.h"

void SystemTick_Config(FunctionalState NewState){
	assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
		SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);                     
		SysTick_Config(SystemCoreClock/500);                                 
  }
  else
  {
		SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk); 
		SysTick->LOAD = 0;	                                                  
		SysTick->VAL = 0;                                                     
  }
}
void Clk_Server(void){
	App_Run();
}
