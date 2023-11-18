#ifndef CLK_DEVICE_H
#define CLK_DEVICE_H
#include "stm32f10x.h"
#include "AdcDevice.h"
#include "App.h"

void SystemTick_Config(FunctionalState NewState);
void Clk_Server(void);
#endif
