#ifndef _ADC_HANDLER_H
#define _ADC_HANDLER_H

#include "stdint.h"

#define ADC_CHANNEL_COUNT 15
#define AMP_SENSE_GAIN 50 // INA240A2 Gain = 50V/V, with mid-ref voltage

enum ADC_Channel
{
    CHANNEL_AMP_Y_NEG = 0,
    CHANNEL_AMP_X_NEG = 1,
    CHANNEL_AMP_Y_POS = 2,
    CHANNEL_AMP_X_POS = 3,
    CHANNEL_HALL_Z = 4,
    CHANNEL_HALL_Y = 5,
    CHANNEL_HALL_X = 6,
    CHANNEL_ADC_IN_14 = 7,
    CHANNEL_VIN = 8,
    CHANNEL_TEMP = 9,
    CHANNEL_VREFINT = 10,
    CHANNEL_AMP_LT = 11,
    CHANNEL_AMP_LB = 12,
    CHANNEL_AMP_RT = 13,
    CHANNEL_AMP_RB = 14,
};

#include "adc.h"
extern uint16_t adc_value[ADC_CHANNEL_COUNT];
void ADC_HandlerInit(void);
__STATIC_INLINE float ADC_GetRaw(enum ADC_Channel channel)
{
    return (float)adc_value[channel];
}
// result in mV
__STATIC_INLINE float ADC_GetVoltage(enum ADC_Channel channel)
{
    return (float)(__LL_ADC_CALC_DATA_TO_VOLTAGE(__LL_ADC_CALC_VREFANALOG_VOLTAGE(adc_value[CHANNEL_VREFINT], LL_ADC_RESOLUTION_12B), adc_value[channel], LL_ADC_RESOLUTION_12B));
}
// result in mV
__STATIC_INLINE float ADC_GetVin()
{
    return (float)(11.0f * (float)__LL_ADC_CALC_DATA_TO_VOLTAGE(__LL_ADC_CALC_VREFANALOG_VOLTAGE(adc_value[CHANNEL_VREFINT], LL_ADC_RESOLUTION_12B), adc_value[CHANNEL_VIN], LL_ADC_RESOLUTION_12B));
}
// result in mV
__STATIC_INLINE float ADC_GetVDD()
{
    return (float)(__LL_ADC_CALC_VREFANALOG_VOLTAGE(adc_value[CHANNEL_VREFINT], LL_ADC_RESOLUTION_12B));
}
// result in degree
__STATIC_INLINE float ADC_GetCoreTemp()
{
    return (float)(__LL_ADC_CALC_TEMPERATURE(__LL_ADC_CALC_VREFANALOG_VOLTAGE(adc_value[CHANNEL_VREFINT], LL_ADC_RESOLUTION_12B), adc_value[CHANNEL_TEMP], LL_ADC_RESOLUTION_12B));
}
// results in A, I = U / (R * gain) because U = I * R * gain
// R = 5 mOhm, gain = AMP_SENSE_GAIN, mV / mOhm = A
__STATIC_INLINE float ADC_GetCurrent(enum ADC_Channel channel)
{
    // Get mid-ref voltage first
    return (float)((ADC_GetVoltage(channel) - 0.5f * ADC_GetVDD()) / (5 * AMP_SENSE_GAIN));
}
__STATIC_INLINE float ADC_GetCurrentmA(enum ADC_Channel channel)
{
    // Get mid-ref voltage first
    return (float)(1000.0f * ADC_GetCurrent(channel));
}
#endif