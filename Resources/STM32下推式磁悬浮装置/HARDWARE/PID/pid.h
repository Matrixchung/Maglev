#ifndef __PID_H
#define __PID_H

#include "sys.h"


typedef struct 
{
	int x_target;   //x轴目标值
	float x_kp;		//X轴比例项系数
	float x_kd;		//X轴微分项系数
	float x_ki;     //X轴积分项系数
	
	int y_target;	//y轴目标值
	float y_kp;		//Y轴比例项系数
	float y_kd;		//Y轴微分项系数
	float y_ki;     //Y轴积分项系数
}PID;
extern PID pid;


void pid_init(void);
int x_pid_calc(u16 magnetic);
int y_pid_calc(u16 megnetic);


#endif /*__PID_H*/


