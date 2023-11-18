#include "delay.h"
#ifdef USE_DWT_COUNTER
uint8_t DWT_Delay_Init(void)
{
	/* Disable TRC */
	CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
	/* Enable TRC */
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;
	/* Disable Clock Cycle Counter */
	DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; // ~0x00000001;
	/* Enable Clock Cycle Counter */
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; // 0x00000001;
	/* Reset the Clock Cycle Counter value */
  	DWT->CYCCNT = 0;
	/* 3 NOPs */
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	/* Check if Clock Cycle Counter has started */
    if(DWT->CYCCNT)
    {
    	return 0; /* Clock Cycle Counter started */
    }
    else
  	{
    	return 1; /* Clock Cycle Counter not started */
  	}
}
#else
void delay_us(uint32_t us){
	uint32_t delay = (HAL_RCC_GetHCLKFreq() / 8000000 * us);
	do{__NOP();}while(delay--);
}
#endif
void delay_ms(uint32_t ms){
    while(ms--){delay_us(1000);}
}
