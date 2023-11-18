#ifndef _COIL_H
#define _COIL_H
#include "pid.h"
#include "lowpass_filter.h"
#include "main.h"
#include "adc_handler.h"

#define AT8236_MAX_COMPARE 4200
#define AT8236_DEADZONE 0
#define COIL_MAX_CURRENT_MA 2000

typedef void (*_Coil_SetCompare)(TIM_TypeDef *TIMx, uint32_t CompareValue);
typedef struct Coil_TypeDef
{
    TIM_TypeDef *TIMx;
    uint32_t channel;
    TIM_TypeDef *TIMxN;
    uint32_t channel_n;
    uint8_t direction;
    int16_t value;
    _Coil_SetCompare _set_compare;
    _Coil_SetCompare _set_compare_n;
    enum ADC_Channel current_adc_channel;
    float _adc_current;
    float set_current;
    PID_TypeDef current_pid;
    // FuzzyPID_TypeDef current_pid;
    LPFilter_TypeDef current_filter;
}Coil_TypeDef;

extern Coil_TypeDef Coil_X_Pos, Coil_X_Neg, Coil_Y_Pos, Coil_Y_Neg, Coil_LT, Coil_LB, Coil_RT, Coil_RB;

void Coil_InstanceInit(Coil_TypeDef *coil, TIM_TypeDef *TIMx, uint32_t channel, TIM_TypeDef *TIMxN, uint32_t channel_n, enum ADC_Channel adc_channel, uint8_t direction);
void Coil_SetOutput(Coil_TypeDef *coil, int16_t value);
// void Coil_CurrentPIDInit(Coil_TypeDef *coil, float p, float i, float d, float limit, float filterTs);
// void Coil_CurrentPIDInit(Coil_TypeDef *coil, float p, float i, float d, float dp_limit, float di_limit, float dd_limit, float filterTs);
void Coil_CurrentPIDInit(Coil_TypeDef *coil, float p, float i, float d, float filterTs, float limit);
void Coil_CalCurrentPID(Coil_TypeDef *coil);
void Coil_UpdateCurrent(Coil_TypeDef *coil);

#endif