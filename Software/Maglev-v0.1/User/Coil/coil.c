#include "coil.h"

Coil_TypeDef Coil_X_Pos, Coil_X_Neg, Coil_Y_Pos, Coil_Y_Neg, Coil_LT, Coil_LB, Coil_RT, Coil_RB;

void Coil_InstanceInit(Coil_TypeDef *coil, TIM_TypeDef *TIMx, uint32_t channel, TIM_TypeDef *TIMxN, uint32_t channel_n, enum ADC_Channel adc_channel, uint8_t direction)
{
    coil->TIMx = TIMx;
    coil->channel = channel;
    coil->TIMxN = TIMxN;
    coil->channel_n = channel_n;
    coil->direction = direction; // 0 - neg, 1 - pos, to make sure: giving positive output will give positive results (x,y,z axis)
    coil->value = 0;
    coil->_set_compare = NULL;
    coil->_set_compare_n = NULL;
    // FuzzyPID_TypeDef pid = FuzzyPID_Init(0, 0, 0, AT8236_MAX_COMPARE, COIL_MAX_CURRENT_MA, 0, 0, 0);
    PID_TypeDef pid = PID_Init(0, 0, 0, AT8236_MAX_COMPARE);
    LPFilter_TypeDef curr_filter = LPFilter_Init(0);
    coil->current_pid = pid;
    coil->current_filter = curr_filter;
    coil->current_adc_channel = adc_channel;
    coil->_adc_current = ADC_GetCurrentmA(adc_channel);
    coil->set_current = 0;
    switch(channel)
    {
        case LL_TIM_CHANNEL_CH1:
            coil->_set_compare = LL_TIM_OC_SetCompareCH1;
            break;
        case LL_TIM_CHANNEL_CH2:
            coil->_set_compare = LL_TIM_OC_SetCompareCH2;
            break;
        case LL_TIM_CHANNEL_CH3:
            coil->_set_compare = LL_TIM_OC_SetCompareCH3;
            break;
        case LL_TIM_CHANNEL_CH4:
            coil->_set_compare = LL_TIM_OC_SetCompareCH4;
            break;
    }
    LL_TIM_CC_EnableChannel(TIMx, channel);
    LL_TIM_CC_EnableChannel(TIMxN, channel_n);
    __attribute__((unused)) uint32_t tmpsmcr = TIMx->SMCR & TIM_SMCR_SMS;
    coil->_set_compare(TIMx, 0);
    if(channel_n == LL_TIM_CHANNEL_CH1 || channel_n == LL_TIM_CHANNEL_CH2 || channel_n == LL_TIM_CHANNEL_CH3 || channel_n == LL_TIM_CHANNEL_CH4) // not complementary output channel
    {
        switch(channel_n)
        {
            case LL_TIM_CHANNEL_CH1:
                coil->_set_compare_n = LL_TIM_OC_SetCompareCH1;
                break;
            case LL_TIM_CHANNEL_CH2:
                coil->_set_compare_n = LL_TIM_OC_SetCompareCH2;
                break;
            case LL_TIM_CHANNEL_CH3:
                coil->_set_compare_n = LL_TIM_OC_SetCompareCH3;
                break;
            case LL_TIM_CHANNEL_CH4:
                coil->_set_compare_n = LL_TIM_OC_SetCompareCH4;
                break;
        }
        coil->_set_compare_n(TIMxN, 0);
        // if(TIMx->CR1 != TIMxN->CR1) // different timer
        // {
            tmpsmcr = TIMxN->SMCR & TIM_SMCR_SMS;
            LL_TIM_EnableAllOutputs(TIMxN);
            LL_TIM_EnableCounter(TIMxN);
        // }
    }
    LL_TIM_EnableAllOutputs(TIMx);
    LL_TIM_EnableCounter(TIMx);
}

// void Coil_CurrentPIDInit(Coil_TypeDef *coil, float p, float i, float d, float dp_limit, float di_limit, float dd_limit, float filterTs)
void Coil_CurrentPIDInit(Coil_TypeDef *coil, float p, float i, float d, float filterTs, float limit)
{
    // FuzzyPID_TypeDef pid = FuzzyPID_Init(p, i, d, AT8236_MAX_COMPARE, COIL_MAX_CURRENT_MA, dp_limit, di_limit, dd_limit);
    PID_TypeDef pid = PID_Init(p, i, d, limit);
    coil->current_pid = pid;
    LPFilter_TypeDef filter = LPFilter_Init(filterTs);
    coil->current_filter = filter;
}

void Coil_UpdateCurrent(Coil_TypeDef *coil)
{
    coil->_adc_current = LPFilter_GetOutput(&coil->current_filter, ADC_GetCurrentmA(coil->current_adc_channel));
}

// called in loop, update current PID
void Coil_CalCurrentPID(Coil_TypeDef *coil)
{
    // coil->_adc_current = ADC_GetCurrentmA(coil->current_adc_channel);
    Coil_SetOutput(coil, (int16_t)_constrain((PID_GetOutput(&coil->current_pid, (coil->set_current - coil->_adc_current))), -AT8236_MAX_COMPARE, AT8236_MAX_COMPARE));
}

void Coil_SetOutput(Coil_TypeDef *coil, int16_t value)
{
    coil->value = _constrain(value > 0 ? value + AT8236_DEADZONE : value - AT8236_DEADZONE, -AT8236_MAX_COMPARE, AT8236_MAX_COMPARE);
    value = coil->direction ? value : -value;
    if(value > 0)
    {
        // LL_TIM_OC_SetPolarity(coil->TIMx, coil->channel, LL_TIM_OCPOLARITY_HIGH);
        // LL_TIM_OC_SetPolarity(coil->TIMx, coil->channel_n, LL_TIM_OCPOLARITY_LOW);
        LL_TIM_CC_DisableChannel(coil->TIMxN, coil->channel_n); // OUT2 low
        LL_TIM_CC_EnableChannel(coil->TIMx, coil->channel);
        // LL_TIM_OC_SetCompareCH2(coil->TIMx, coil->compare_value);
        coil->_set_compare(coil->TIMx, (uint32_t)value);
    }
    else if(value == 0)
    {
        // LL_TIM_OC_SetPolarity(coil->TIMx, coil->channel, LL_TIM_OCPOLARITY_HIGH);
        // LL_TIM_OC_SetPolarity(coil->TIMx, coil->channel_n, LL_TIM_OCPOLARITY_LOW);
        LL_TIM_CC_EnableChannel(coil->TIMxN, coil->channel_n);
        LL_TIM_CC_EnableChannel(coil->TIMx, coil->channel);
        // LL_TIM_OC_SetCompareCH2(coil->TIMx, 0);
        coil->_set_compare(coil->TIMx, 0);
        if(coil->_set_compare_n != NULL) coil->_set_compare_n(coil->TIMxN, 0);
    }
    else
    {
        // LL_TIM_OC_SetPolarity(coil->TIMx, coil->channel, LL_TIM_OCPOLARITY_LOW);
        // LL_TIM_OC_SetPolarity(coil->TIMx, coil->channel_n, LL_TIM_OCPOLARITY_HIGH);
        // coil->compare_value = -(coil->compare_value);
        LL_TIM_CC_DisableChannel(coil->TIMx, coil->channel);  // OUT1 low
        LL_TIM_CC_EnableChannel(coil->TIMxN, coil->channel_n);
        // LL_TIM_OC_SetCompareCH2(coil->TIMx, coil->compare_value);
        if(coil->_set_compare_n != NULL) coil->_set_compare_n(coil->TIMxN, (uint32_t)(-value));
        else coil->_set_compare(coil->TIMx, (uint32_t)(AT8236_MAX_COMPARE+(value)));
    }
}