#ifndef _ENCODER_H
#define _ENCODER_H

#include "main.h"

typedef void (*Encoder_Callback)();
typedef struct Encoder_TypeDef
{
    TIM_TypeDef *TIMx;
    uint32_t channel;
    uint32_t rawValue;
    uint32_t rawValue_prev;
    int32_t value;
    uint8_t direction; // 0 - CCW, 1 - CW
    Encoder_Callback callback;
}Encoder_TypeDef;

__STATIC_INLINE void Encoder_RegisterCallback(Encoder_TypeDef *encoder, Encoder_Callback cb)
{
    encoder->callback = cb;
}

extern Encoder_TypeDef EC11_Encoder;

void Encoder_Init(Encoder_TypeDef *encoder, TIM_TypeDef *TIMx, uint32_t channel);
void Encoder_IT_Handler(Encoder_TypeDef *encoder);

#endif