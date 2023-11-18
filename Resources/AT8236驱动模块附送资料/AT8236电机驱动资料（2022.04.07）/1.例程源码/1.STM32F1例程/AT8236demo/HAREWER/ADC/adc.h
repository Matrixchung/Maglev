#ifndef __ADC_H
#define	__ADC_H

#include "stm32f10x.h"

void adc_Init(void);
u16  Get_adc(u8 ch); 
u16 Get_adc_Average(u8 ch,u8 times);

#endif

