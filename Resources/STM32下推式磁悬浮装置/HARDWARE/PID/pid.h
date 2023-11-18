#ifndef __PID_H
#define __PID_H

#include "sys.h"


typedef struct 
{
	int x_target;   //x��Ŀ��ֵ
	float x_kp;		//X�������ϵ��
	float x_kd;		//X��΢����ϵ��
	float x_ki;     //X�������ϵ��
	
	int y_target;	//y��Ŀ��ֵ
	float y_kp;		//Y�������ϵ��
	float y_kd;		//Y��΢����ϵ��
	float y_ki;     //Y�������ϵ��
}PID;
extern PID pid;


void pid_init(void);
int x_pid_calc(u16 magnetic);
int y_pid_calc(u16 megnetic);


#endif /*__PID_H*/


