#include "lowpass_filter.h"

LPFilter_TypeDef LPFilter_Init(float time_constant)
{
    LPFilter_TypeDef filter;
    filter.Tf = time_constant;
    filter.timestamp_prev = get_micros();
    filter.output_prev = 0.0f;
    filter._last_dt = 0.0f;
    return filter;
}

float LPFilter_GetOutput(LPFilter_TypeDef *filter, float input)
{
    uint32_t timestamp = get_micros();
    float dt = (timestamp - filter->timestamp_prev) * 1e-6f;
    if(dt < 0.0f) dt = filter->_last_dt;
    else if(dt > 0.3f) // delta time too long for low pass filter
    {
        filter->output_prev = input;
        filter->timestamp_prev = timestamp;
        filter->_last_dt = dt;
        return input;
    }
    float alpha = filter->Tf / (filter->Tf + dt);
    // float output = alpha * filter->output_prev + (1.0f - alpha) * input;
    // filter->output_prev = output;
    filter->output_prev = alpha * filter->output_prev + (1.0f - alpha) * input;
    filter->timestamp_prev = timestamp;
    filter->_last_dt = dt;
    // return output;
    return filter->output_prev;
}