#include "PID.h"


s16 PID_Calculate(s16 error,Pid *pid){
	s16 output;
	
	pid->Integrarion += error;
	
	if(pid->Integrarion > MAX_INTEGRATION){
		pid->Integrarion = MAX_INTEGRATION;
	}else if(pid->Integrarion < -MAX_INTEGRATION){
		pid->Integrarion = -MAX_INTEGRATION;
	}
	
		output = pid->Kp * error + pid->Ki * pid->Integrarion + pid->Kd * (error - pid->PreError);
	
	if(output > MAX_OUTPUT){
		output = MAX_OUTPUT;
	}else if(output < -MAX_OUTPUT){
		output = -MAX_OUTPUT;
	}
	
	
	if(pid->transmition == _true){
		pid->error = error; 
		pid->error_preerror = error - pid->PreError;
		pid->output = output;
		pid->transmition = _false;
	}
	
	pid->PreError = error;
	
	return output;
}

