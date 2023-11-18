#ifndef __CONTROL_H
#define __CONTROL_H


#include "sys.h"

extern float ADC_ConvertedValueLocal[3];
extern int COIL_X, COIL_Y, COIL_Z;
extern int ADC_VALUE[3];



void set_PWM(int coil1,int coil2);
void limiting_PWM(void);
int myabs(int num);


#endif /*__CONTROL_H*/

