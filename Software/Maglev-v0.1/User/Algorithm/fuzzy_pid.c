#include "fuzzy_pid.h"
FuzzyPID_TypeDef FuzzyPID_Init(float p, float i, float d, float limit, float error_limit, float d_p_limit, float d_i_limit, float d_d_limit)
{
    FuzzyPID_TypeDef pid;
    pid.P = p;
    pid.I = i;
    pid.D = d;
    pid.limit = limit;
    pid.error_limit = error_limit;
    pid.ec_limit = pid.error_limit * 0.5f;
    pid.d_p_limit = d_p_limit;
    pid.d_i_limit = d_i_limit;
    pid.d_d_limit = d_d_limit;
    pid.enable_integral = 1;
    pid._error_prev = 0.0f;
    pid._integral_prev = 0.0f;
    pid._output_prev = 0.0f;
    pid._timestamp_prev = get_micros();
    // init fuzzy pid intervals
    pid._err_seg_len = 2.0f * pid.error_limit / (FUZZY_SEG_NUMS - 1);
    pid._ec_seg_len = 2.0f * pid.ec_limit / (FUZZY_SEG_NUMS - 1);
    pid._d_p_seg_len = 2.0f * pid.d_p_limit / (FUZZY_SEG_NUMS - 1);
    pid._d_i_seg_len = 2.0f * pid.d_i_limit / (FUZZY_SEG_NUMS - 1);
    pid._d_d_seg_len = 2.0f * pid.d_d_limit / (FUZZY_SEG_NUMS - 1);
    for(uint8_t i = 0; i < FUZZY_SEG_NUMS; i++)
    {
        pid._percent_error[i] = 0.0f;
        pid._percent_ec[i] = 0.0f;
        pid._percent_kp[i] = 0.0f;
        pid._percent_ki[i] = 0.0f;
        pid._percent_kd[i] = 0.0f;
        pid._err_limit_int[i] = -pid.error_limit + i * pid._err_seg_len;
        pid._ec_limit_int[i] = -pid.ec_limit + i * pid._ec_seg_len;
        pid._d_p_limit_int[i] = -pid.d_p_limit + i * pid._d_p_seg_len;
        pid._d_i_limit_int[i] = -pid.d_i_limit + i * pid._d_i_seg_len;
        pid._d_d_limit_int[i] = -pid.d_d_limit + i * pid._d_d_seg_len;
    }
    pid._d_p = 0.0f;
    pid._d_i = 0.0f;
    pid._d_d = 0.0f;
    return pid;
}

void _fuzzify(float input, float *interval, float segment_len, float *percent)
{
    if(input <= interval[0]) // <= 0
    {
        percent[0] = 1.0f;
        return;
    }
    else if(input <= interval[1]) // [0, 1]
    {
        percent[1] = (input - interval[0]) / segment_len;
        percent[0] = 1.0f - percent[1];
        return;
    }
    else if(input <= interval[2]) // [1, 2]
    {
        percent[2] = (input - interval[1]) / segment_len;
        percent[1] = 1.0f - percent[2];
        return;
    }
    else if(input <= interval[3]) // [2, 3]
    {
        percent[3] = (input - interval[2]) / segment_len;
        percent[2] = 1.0f - percent[3];
        return;
    }
    else if(input <= interval[4]) // [3, 4]
    {
        percent[4] = (input - interval[3]) / segment_len;
        percent[3] = 1.0f - percent[4];
        return;
    }
    else if(input <= interval[5]) // [4, 5]
    {
        percent[5] = (input - interval[4]) / segment_len;
        percent[4] = 1.0f - percent[5];
        return;
    }
    else if(input <= interval[6]) // [5, 6]
    {
        percent[6] = (input - interval[5]) / segment_len;
        percent[5] = 1.0f - percent[6];
        return;
    }
    else // > 6
    {
        percent[6] = 1.0f;
        return;
    }
}

void _apply_fuzzy_rule(float *percent_error, float *percent_ec, float *percent_output, const uint8_t rule[FUZZY_SEG_NUMS][FUZZY_SEG_NUMS])
{
    for(uint8_t i = 0; i < FUZZY_SEG_NUMS; i++)
    {
        if(percent_error[i] == 0.0f) continue;
        for(uint8_t j = 0; j < FUZZY_SEG_NUMS; j++)
        {
            if(percent_ec[j] == 0.0f) continue;
            percent_output[rule[i][j]] += percent_error[i] * percent_ec[j];
        }
    }
}

float _defuzzify(float *interval, float *percent)
{
    float ret = 0;
    for(uint8_t i = 0; i < FUZZY_SEG_NUMS; i++)
    {
        ret += percent[i] * interval[i];
    }
    return ret;
}

float FuzzyPID_GetOutput(FuzzyPID_TypeDef *pid, float error)
{
    _fuzzify(error, pid->_err_limit_int, pid->_err_seg_len, pid->_percent_error);
    _fuzzify(error - pid->_error_prev, pid->_ec_limit_int, pid->_ec_seg_len, pid->_percent_ec);
    _apply_fuzzy_rule(pid->_percent_error, pid->_percent_ec, pid->_percent_kp, Kp_FUZZY_RULE);
    _apply_fuzzy_rule(pid->_percent_error, pid->_percent_ec, pid->_percent_ki, Ki_FUZZY_RULE);
    _apply_fuzzy_rule(pid->_percent_error, pid->_percent_ec, pid->_percent_kd, Kd_FUZZY_RULE);
    pid->_d_p = _defuzzify(pid->_d_p_limit_int, pid->_percent_kp);
    pid->_d_i = _defuzzify(pid->_d_i_limit_int, pid->_percent_ki);
    pid->_d_d = _defuzzify(pid->_d_d_limit_int, pid->_percent_kd);
    uint32_t timestamp_now = get_micros();
    float Ts = (timestamp_now - (pid->_timestamp_prev)) * 1e-6f;
    if(Ts <= 0 || Ts > 0.5f) Ts = 1e-3f;
    float integral = 0;
    if(pid->enable_integral)
    {
        integral = _constrain((pid->_integral_prev) + ((pid->I + pid->_d_i) * Ts * 0.5f * (error + pid->_error_prev)), -pid->limit, pid->limit);
    }
    float output = ((pid->P + pid->_d_p) * error) + integral + ((pid->D + pid->_d_d) * (error - pid->_error_prev) / Ts);
    output = _constrain(output, -pid->limit, pid->limit);
    pid->_integral_prev = integral;
    pid->_output_prev = output;
    pid->_error_prev = error;
    pid->_timestamp_prev = timestamp_now;
    return output;
}