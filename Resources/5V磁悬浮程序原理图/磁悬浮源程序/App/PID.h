#ifndef PID_H
#define PID_H
#include "stm32f10x.h"

#define _true   ENABLE
#define _false  DISABLE

typedef struct _Pid{
	float Kp;
	float Ki;
	float Kd;
	s16 Integrarion;
	s16 PreError;
		
	FunctionalState transmition;
	
	s16 error;
	
	s16 error_preerror;
	
	s16 output;
	
}Pid;

#define MAX_OUTPUT         899
#define MAX_INTEGRATION    300

s16 PID_Calculate(s16 error,Pid *pid);
#endif
