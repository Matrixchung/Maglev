#ifndef APP_H
#define APP_H
#include "stm32f10x.h"
#include "AdcDevice.h"
#include "PwmDevice.h"
#include "UsartDevice.h"
#include "LedDevice.h"
#include "PID.h"

#define  STEP_1_TIMER    500

void App_Init(void);
void App_Run(void);
#endif
