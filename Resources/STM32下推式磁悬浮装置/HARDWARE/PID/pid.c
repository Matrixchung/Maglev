#include "pid.h"

PID pid;

/**
  * @brief  PID初始化函数
  * @param 	无
  * @retval 0
  */
void pid_init(void)
{
	pid.x_target = 2100;
	pid.x_kp = 1.8;		
	pid.x_kd = 8;
	pid.x_ki = 0;
	
	pid.y_target = 2100;
	pid.y_kp = -1.8;
	pid.y_kd = -8;
	pid.y_ki = 0;
}


int x_pid_calc(u16 magnetic)
{
	float magnetic_bias=0;
	static float last_magnetic_bias=0, magnetic_integral=0;
	float x_pid=0;
	
	magnetic_bias = magnetic - pid.x_target;	
//	magnetic_integral *= 0.7;
//	magnetic_integral += magnetic_bias;
	
	x_pid = pid.x_kp * magnetic_bias + 
		pid.x_kd * (magnetic_bias - last_magnetic_bias) + pid.x_ki * magnetic_integral;
	
	last_magnetic_bias = magnetic_bias;
	
	return x_pid;
}


int y_pid_calc(u16 megnetic)
{
	float magnetic_bias=0;
	static float last_magnetic_bias=0, magnetic_integral=0;
	float y_pid=0;
	
	magnetic_bias = megnetic - pid.y_target;
//	magnetic_integral *= 0.7;
//	magnetic_integral += magnetic_bias;
	
	y_pid = pid.y_kp * magnetic_bias + 
		pid.y_kd * (magnetic_bias - last_magnetic_bias) + pid.y_ki * magnetic_integral;
	
	last_magnetic_bias = magnetic_bias;
	
	return y_pid;
}







