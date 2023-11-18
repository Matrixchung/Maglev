#ifndef LED_DEVICE_H
#define LED_DEVICE_H
#include "stm32f10x_gpio.h"
#define  LedSysTickMask        0x0001
#define  LedSysTickFrequency        3

void Led_SysTick(void);
void Led_SystemStart(void);
void Led_Config(FunctionalState NewState);
#endif
