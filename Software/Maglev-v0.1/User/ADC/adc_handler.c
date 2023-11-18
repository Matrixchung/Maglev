#include "adc_handler.h"
uint16_t adc_value[ADC_CHANNEL_COUNT] = {0};

void ADC_HandlerInit(void)
{
    LL_DMA_SetMemoryAddress(DMA2, LL_DMA_STREAM_0, (uint32_t)&adc_value);
    LL_DMA_SetPeriphAddress(DMA2, LL_DMA_STREAM_0, LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA));
    LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_0, ADC_CHANNEL_COUNT);
    LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_0);
    LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
    LL_ADC_Enable(ADC1);
    LL_ADC_REG_StartConversionSWStart(ADC1);
}