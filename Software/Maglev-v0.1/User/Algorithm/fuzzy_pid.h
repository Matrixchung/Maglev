#ifndef _FUZZY_PID_H
#define _FUZZY_PID_H

#include "stdint.h"
#include "fuzzy_rules.h"

typedef struct FuzzyPID_TypeDef
{
    uint32_t _timestamp_prev;
    uint8_t enable_integral;
    float P, I, D;
    float d_p_limit, d_i_limit, d_d_limit;
    float limit;
    float error_limit, ec_limit;
    float _err_limit_int[FUZZY_SEG_NUMS];
    float _ec_limit_int[FUZZY_SEG_NUMS];
    float _d_p_limit_int[FUZZY_SEG_NUMS];
    float _d_i_limit_int[FUZZY_SEG_NUMS];
    float _d_d_limit_int[FUZZY_SEG_NUMS];
    float _err_seg_len, _ec_seg_len, _d_p_seg_len, _d_i_seg_len, _d_d_seg_len;
    float _error_prev;
    float _output_prev;
    float _integral_prev;
    float _d_p;
    float _d_i;
    float _d_d;
    float _percent_error[FUZZY_SEG_NUMS];
    float _percent_ec[FUZZY_SEG_NUMS];
    float _percent_kp[FUZZY_SEG_NUMS];
    float _percent_ki[FUZZY_SEG_NUMS];
    float _percent_kd[FUZZY_SEG_NUMS];
}FuzzyPID_TypeDef;

#include "algorithm_utils.h"
#include "main.h"
#include "delay.h"

FuzzyPID_TypeDef FuzzyPID_Init(float p, float i, float d, float limit, float error_limit, float d_p_limit, float d_i_limit, float d_d_limit);
float FuzzyPID_GetOutput(FuzzyPID_TypeDef *pid, float error);

#endif