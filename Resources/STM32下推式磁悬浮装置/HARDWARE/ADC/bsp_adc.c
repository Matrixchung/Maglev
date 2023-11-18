#include "bsp_adc.h"



//value = (float)ADC_ConvertedValue[x] /4096 * 3.3
__IO uint16_t ADC_ConvertedValue[3] = {0,0,0};


/**
  * @brief  ADC1��ʼ��
  * @param  ��
  * @retval ��
  */
static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
	
	//PA1 PA4 PA5 ��Ϊģ��ͨ����������                       
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}		


static void ADC1_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_InitStructure.DMA_BufferSize = 3;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M = DISABLE;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) ADC_ConvertedValue; //��Ҫʹ�õĴ洢����ַ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //16λ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&(ADC1 -> DR)); //�����ַ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1, ENABLE);
}


static void ADC1_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //�����ɼ�
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //�����Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //�ر��ⲿ����
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //����ģʽ
	ADC_InitStructure.ADC_NbrOfChannel = 3; //ͨ����Ŀ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; //����ɨ��ģʽ
	
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//����ADCʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	//����ͨ��ת��˳��Ͳɼ�ʱ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SampleTime_55Cycles5);
	
	
	ADC_Cmd(ADC1, ENABLE);
	
	//ADC DMA ����
	ADC_DMACmd(ADC1, ENABLE);
	
	//У׼
	ADC_StartCalibration(ADC1);
	//�ȴ�У׼���
	while( ADC_GetCalibrationStatus(ADC1) );
	//�������
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
}

void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_DMA_Config();
	ADC1_Mode_Config();
}


