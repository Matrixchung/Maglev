#ifndef _LOWPASS_FILTER_H
#define _LOWPASS_FILTER_H

#include "stdint.h"

typedef struct LPFilter_TypeDef
{
    float Tf;
    float output_prev;
    float _last_dt;
    uint32_t timestamp_prev;
}LPFilter_TypeDef;

// #include "main.h"
#include "delay.h"

LPFilter_TypeDef LPFilter_Init(float time_constant);
float LPFilter_GetOutput(LPFilter_TypeDef *filter, float input);

#endif