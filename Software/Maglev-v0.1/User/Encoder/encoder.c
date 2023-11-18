#include "encoder.h"

Encoder_TypeDef EC11_Encoder;

void Encoder_Init(Encoder_TypeDef *encoder, TIM_TypeDef *TIMx, uint32_t channel)
{
    encoder->TIMx = TIMx;
    encoder->channel = channel;
    encoder->callback = NULL;
    encoder->direction = 0;
    encoder->rawValue = 32768;
    encoder->rawValue_prev = 32768;
    encoder->value = 0;
    LL_TIM_SetCounter(TIMx, 32768);
    LL_TIM_EnableIT_CC1(TIMx);
    LL_TIM_CC_EnableChannel(TIMx, channel);
    LL_TIM_ClearFlag_CC1(TIMx);
    LL_TIM_DisableIT_UPDATE(TIMx);
    LL_TIM_EnableCounter(TIMx);
}

void Encoder_IT_Handler(Encoder_TypeDef *encoder)
{
    if(LL_TIM_IsActiveFlag_CC1(encoder->TIMx))
    {
        LL_TIM_ClearFlag_CC1(encoder->TIMx);
        LL_TIM_DisableIT_CC1(encoder->TIMx);
        encoder->rawValue = LL_TIM_GetCounter(encoder->TIMx);
        if((encoder->rawValue_prev >= 65531 && encoder->rawValue <= 4) || (encoder->rawValue > encoder->rawValue_prev)) // CCW
        {
            encoder->direction = 0;
            encoder->value--;
            if(encoder->callback != NULL) encoder->callback();
        }
        else if((encoder->rawValue_prev <= 4 && encoder->rawValue >= 65531) || (encoder->rawValue < encoder->rawValue_prev)) // CW
        {
            encoder->direction = 1;
            encoder->value++;
            if(encoder->callback != NULL) encoder->callback();
        }
        encoder->rawValue_prev = encoder->rawValue;
        LL_TIM_EnableIT_CC1(encoder->TIMx);
    }
}