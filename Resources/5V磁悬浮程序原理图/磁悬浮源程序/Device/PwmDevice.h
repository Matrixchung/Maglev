#ifndef PWM_DEVICE_H
#define PWM_DEVICE_H
#include "stm32f10x.h"
#define TIME_Period       899
#define TIME_Prescaler      1

void Pwm_PB_6_7_8_9_10_Config(FunctionalState NewState);
void PWM_PB_6_SetPower(u16 power);
void PWM_PB_7_SetPower(u16 power);
void PWM_PB_8_SetPower(u16 power);
void PWM_PB_9_SetPower(u16 power);
void PWM_PB_10_SetPower(u16 power);
#endif
