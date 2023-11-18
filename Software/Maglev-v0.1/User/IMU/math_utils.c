#include "math_utils.h"
float fast_sqrt(float x) 
{
    uint32_t i = 0;
    float y;
    i = *(uint32_t *) &x;            // 按照整型方法操作浮点数
    i = 0x1fbd1df5 + (i >> 1);       // Magic!
    y = *(float *) &i;               // 获得平方根近似值
    y = 0.5f * (y + x / y);           // 一次牛顿迭代
    return y;
}

float fast_invsqrt(float x)
{
    uint32_t i = 0;
    float y, halfx;
    halfx = 0.5f * x;
    i = *(uint32_t*) &x;             // 按照整型方法操作浮点数
    i = 0x5f3759df - (i >> 1);       // Magic!
    y = *(float*) &i;                // 获得平方根倒数近似值
    y = y * (1.5 - halfx * y * y);   // 一次牛顿迭代
    return y;
}