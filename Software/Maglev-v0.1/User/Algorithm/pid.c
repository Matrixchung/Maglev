#include "pid.h"

PID_TypeDef PID_Init(float p, float i, float d, float limit)
{
    PID_TypeDef pid;
    pid.P = p;
    pid.I = i;
    pid.D = d;
    pid.limit = limit;
    pid.enable_integral = 1;
    pid._error_prev = 0.0f;
    pid._integral_prev = 0.0f;
    pid._output_prev = 0.0f;
    pid._timestamp_prev = get_micros();
    return pid;
}

float PID_GetOutput(PID_TypeDef *pid, float error)
{
    uint32_t timestamp_now = get_micros();
    float Ts = (timestamp_now - (pid->_timestamp_prev)) * 1e-6f;
    if(Ts <= 0 || Ts > 0.5f) Ts = 1e-3f;
    float integral = 0.0f;
    if(pid->enable_integral)
    {
        integral = _constrain((pid->_integral_prev) + (pid->I * Ts * 0.5f * (error + pid->_error_prev)), -pid->limit, pid->limit);
    }
    else pid->_integral_prev = 0.0f;
    float output = ((pid->P) * error) + integral + ((pid->D) * (error - pid->_error_prev) / Ts);
    output = _constrain(output, -pid->limit, pid->limit);
    pid->_integral_prev = integral;
    pid->_output_prev = output;
    pid->_error_prev = error;
    pid->_timestamp_prev = timestamp_now;
    return output;
}