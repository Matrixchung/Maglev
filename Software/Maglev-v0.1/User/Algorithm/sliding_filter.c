#include "sliding_filter.h"
SlidingFilter_TypeDef SlidingFilter_Init(uint8_t window_size)
{
    SlidingFilter_TypeDef filter;
    filter.window_size = _constrain(window_size, 1, MAX_WINDOW_SIZE);
    for(uint8_t i = 0; i < filter.window_size; i++)
    {
        filter.buffer[i] = 0.0f;
    }
    return filter;
}

float SlidingFilter_GetOutput(SlidingFilter_TypeDef *filter, float input)
{
    if(filter->_data_count < filter->window_size)
    {
        filter->buffer[filter->_data_count++] = input;
        return input;
    }
    float sum = 0.0f;
    memcpy(&(filter->buffer[0]), &(filter->buffer[1]), (filter->window_size - 1) * sizeof(float));
    filter->buffer[filter->window_size-1] = input;
    for(uint8_t i = 0; i < filter->window_size; i++) sum += filter->buffer[i];
    return sum / filter->window_size;
}
