#ifndef _PID_H
#define _PID_H

#include "stdint.h"

typedef struct PID_TypeDef
{
    uint32_t _timestamp_prev;
    uint8_t enable_integral;
    float P, I, D;
    float limit;
    float _error_prev;
    float _output_prev;
    float _integral_prev;
}PID_TypeDef;

#include "algorithm_utils.h"
#include "main.h"
#include "delay.h"

PID_TypeDef PID_Init(float p, float i, float d, float limit);
float PID_GetOutput(PID_TypeDef *pid, float error);

#endif