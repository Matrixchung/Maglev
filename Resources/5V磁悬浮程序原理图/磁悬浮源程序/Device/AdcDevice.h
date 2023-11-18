#ifndef ADC_DRIVER_H
#define ADC_DRIVER_H
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "AdcDevice.txt"

void ADC_Config(void);
void ADC_GetData(void);
void ADC_StartRun(void);
#endif
