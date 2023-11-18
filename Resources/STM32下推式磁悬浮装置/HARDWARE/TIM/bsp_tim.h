#ifndef __BSP_TIM_H
#define __BSP_TIM_H


#include "sys.h"


#define PWMX   TIM1->CCR1  //PA8
#define PWMY   TIM1->CCR4  //PA11



void coil_PWM_init(u16 arr,u16 psc);
void TIM3_Init(void);

#endif /*__BSP_TIM_H*/

