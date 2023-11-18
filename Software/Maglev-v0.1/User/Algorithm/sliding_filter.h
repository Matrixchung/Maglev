#ifndef _SLIDING_FILTER_H
#define _SLIDING_FILTER_H

#include <stdint.h>
#include <string.h>
#include "algorithm_utils.h"

#define MAX_WINDOW_SIZE 30

typedef struct SlidingFilter_TypeDef
{
    uint8_t window_size;
    float buffer[MAX_WINDOW_SIZE];
    uint8_t _data_count;
}SlidingFilter_TypeDef;

SlidingFilter_TypeDef SlidingFilter_Init(uint8_t window_size);
float SlidingFilter_GetOutput(SlidingFilter_TypeDef *filter, float input);

#endif