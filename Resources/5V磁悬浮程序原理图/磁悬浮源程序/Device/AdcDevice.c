#include "AdcDevice.h"

vu16 AdcValue[192];
vu16 AdcValue0,AdcValue1;

void GPIO_AdcConfig(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
}
void ADC_Config(void){
	ADC_InitTypeDef ADC_InitStruct;
	DMA_InitTypeDef DMA_InitStruct;
	
	GPIO_AdcConfig();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	ADC_DeInit(ADC1);
	
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;                         
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                      
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;         
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;                          
	ADC_InitStruct.ADC_NbrOfChannel = 16;                                    
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;                                
	ADC_Init(ADC1,&ADC_InitStruct);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,1,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,2,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,3,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,4,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,5,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_7,6,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,7,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,8,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,9,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,10,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,11,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_7,12,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,13,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,14,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,15,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_7,16,ADC_SampleTime_239Cycles5);
	
	ADC_DMACmd(ADC1,ENABLE);                                                
	
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);                                              
	while(ADC_GetResetCalibrationStatus(ADC1));                            
	ADC_StartCalibration(ADC1);                                            
	while(ADC_GetCalibrationStatus(ADC1));                                   
	
	
	DMA_InitStruct.DMA_BufferSize = 192;                                      
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;                          
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;                                 
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)&AdcValue;                       
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;          
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                     
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;                             
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;                   
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;                          
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel1);
	DMA_Init(DMA1_Channel1,&DMA_InitStruct);
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
}
void ADC_GetData(void){
	u8 i,j;
	
	ADC_SoftwareStartConvCmd(ADC1, DISABLE);
	
	AdcValue0 = 0;
	AdcValue1 = 0;

	
	for(j=0;j<12;){
		i = j * 16;
		AdcValue0 = AdcValue0 + (AdcValue[i]+AdcValue[i+3]+AdcValue[i+6]+AdcValue[i+9]+AdcValue[i+12])/5;
		AdcValue1 = AdcValue1 + (AdcValue[i+1]+AdcValue[i+4]+AdcValue[i+7]+AdcValue[i+10]+AdcValue[i+13])/5;
		j++;
	}
	AdcValue0 = AdcValue0/12;
	AdcValue1 = AdcValue1/12;


}
void ADC_StartRun(void){
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
}
