/*********************************************************************************
	*Copyright(C),20xx-20xx,

	*Filename:mcu_hal.c

	*Author:
			wwyyy
			Q1046685883

	*Version:
			V1.0

	*Data:
			2019/11/27

	*Description:


	*Other:


	*Function List:
		1.
		2.
		3.
		4.
	*History modify:
		1.Data:
		Author:
		Log:

**********************************************************************************/
#include "mcu_hal.h"
#include "public.h"
#include "string.h"
#include "timer.h"




//////////////////////////////////////////mcu///////////////////////////////////////////////////
/***********************************************************************************************
*函数名: void mcu_init(void)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_init(void)
{
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}

/***********************************************************************************************
*函数名: void mcu_disable_irq(void)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_disable_irq(void)
{
	__disable_irq();
}

/***********************************************************************************************
*函数名: void mcu_enable_irq(void)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_enable_irq(void)
{
	__enable_irq();
}

/***********************************************************************************************
*函数名: void mcu_kick_dog(void)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_kick_dog(void)
{
}

/***********************************************************************************************
*函数名: void mcu_soft_reset(void)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_soft_reset(void)
{
	NVIC_SystemReset();
}





//////////////////////////////////////////////配置tim//////////////////////////////////////////////
static void(*pfun_timIsr)(void) = NULL;
/***********************************************************************************************
*函数名: void mcu_tim_init(uint16 intvTime)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_tim_init(uint16 intvTime)
{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
//	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	TIM_TimeBaseStructure.TIM_Period = 1000*intvTime;
//	TIM_TimeBaseStructure.TIM_Prescaler = 71;
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
//	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
//	TIM_Cmd(TIM6, ENABLE);
	SysTick_Config(SystemCoreClock / 1000 * intvTime);
}

/***********************************************************************************************
*函数名: void mcu_tim_init_call(uint16 intvTime, void(*pfun_callBack)(void))
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_tim_init_call(uint16 intvTime, void(*pfun_callBack)(void))
{
	pfun_timIsr = pfun_callBack;
	mcu_tim_init(intvTime);
}

/***********************************************************************************************
*函数名: void mcu_tim_delayus(uint16 delayCount)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_tim_delayus(uint16 delayCount)
{
#define TIM_DELAY_MAX_COUNT		(SystemCoreClock/1000-1)
	uint8 overRun = 0;
	uint32 startCount = 0, endCount = 0, count = 0;
	uint32 delayCount1 = delayCount*(SystemCoreClock/1000/1000);
	startCount = SysTick->VAL;
	//向下计数
	if(startCount >= delayCount1)
	{
		endCount = startCount - delayCount1;
		overRun = 0;
	}
	else
	{
		endCount = startCount + TIM_DELAY_MAX_COUNT - delayCount1;
		overRun = 1;
	}
	while(1)
	{
		count = SysTick->VAL;
		if(0 == overRun)
		{
			if(count <= endCount || count > startCount)
				break;
		}
		else
		{
			if(count <= endCount && count > startCount)
				break;
		}
	}
//	//向上计数
//	if(startCount+delayCount1>=TIM_DELAY_MAX_COUNT)
//	{
//		overRun = 1;
//	}
//	else
//	{
//		overRun = 0;
//	}
//	endCount = (startCount+delayCount1)%TIM_DELAY_MAX_COUNT;
//	while(1)
//	{
//		count = SysTick->VAL;
//		if(0 == overRun)
//		{
//			if(count >= endCount || count < startCount)
//				break;
//		}
//		else
//		{
//			if(count >= endCount && count < startCount)
//				break;
//		}
//	}
}

/***********************************************************************************************
*函数名: void mcu_tim_isr(void)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_tim_isr(void)
{
	if(NULL != pfun_timIsr)
		pfun_timIsr();
}





//////////////////////////////////////////////ioin//////////////////////////////////////////////
static uint8 mcuIoInTableNum = 0;
static MCU_IOIN_TAB *p_mcuIoInTable = NULL;
/***********************************************************************************************
*函数名: void mcu_ioin_init(MCU_IOIN_TAB *p_table, uint8 tableNum)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_ioin_init(MCU_IOIN_TAB *p_table, uint8 tableNum)
{
	uint8 i = 0;
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	mcuIoInTableNum = tableNum;
	p_mcuIoInTable = p_table;
	for(i = 0; i < mcuIoInTableNum; i++)
	{
		p_mcuIoInTable[i].filterCnt = 0;
		p_mcuIoInTable[i].rollCount = 0;
		p_mcuIoInTable[i].status = 0;
		p_mcuIoInTable[i].lastStatus = 0;
		RCC_APB2PeriphClockCmd(p_mcuIoInTable[i].portClk, ENABLE);
		GPIO_InitStructure.GPIO_Pin = p_mcuIoInTable[i].gpioPin;
		if(IN_PU == p_mcuIoInTable[i].mode)//上拉
		{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		}
		else if(IN_PD == p_mcuIoInTable[i].mode)//下拉
		{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		}
		else
		{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		}
		GPIO_Init(p_mcuIoInTable[i].p_gpioPort, &GPIO_InitStructure);//初始化管脚
	}
}

/***********************************************************************************************
*函数名: uint16 mcu_ioin_get_ready(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint16 mcu_ioin_get_ready(uint8 channel)
{
	return p_mcuIoInTable[channel].rollCount;
}

/***********************************************************************************************
*函数名: void mcu_ioin_clr_ready(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_ioin_clr_ready(uint8 channel)
{
	p_mcuIoInTable[channel].rollCount = 0;
}

/***********************************************************************************************
*函数名: uint16 mcu_ioin_get_status(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 mcu_ioin_get_status(uint8 channel)
{
	return p_mcuIoInTable[channel].status;
}

/***********************************************************************************************
*函数名: void mcu_ioin_main(void)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_ioin_main(void)
{
	uint8 i = 0;
	uint8 re = 0;
	for(i = 0; i < mcuIoInTableNum; i++)
	{
		re = GPIO_ReadInputDataBit(p_mcuIoInTable[i].p_gpioPort, p_mcuIoInTable[i].gpioPin) ? 1 : 0;
		re = p_mcuIoInTable[i].flip == re ? 0 : 1;
		if(p_mcuIoInTable[i].lastStatus == re)
		{
			p_mcuIoInTable[i].filterCnt++;
			if(p_mcuIoInTable[i].filterCnt >= p_mcuIoInTable[i].filterLen)
			{
				p_mcuIoInTable[i].filterCnt = p_mcuIoInTable[i].filterLen;//=0;
				p_mcuIoInTable[i].status = re;
				updata_roll_count16(&p_mcuIoInTable[i].rollCount);
			}
		}
		else
		{
			p_mcuIoInTable[i].filterCnt = 0;
			p_mcuIoInTable[i].lastStatus = re;
		}
	}
}





///////////////////////////////////////////ioout////////////////////////////////////////////////
static uint8 mcuIoOutTableNum = 0;
static MCU_IOOUT_TAB * p_mcuIoOutTable = NULL;
/***********************************************************************************************
*函数名: void mcu_ioout_init(MCU_IOOUT_TAB *p_table, uint8 tableNum)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_ioout_init(MCU_IOOUT_TAB *p_table, uint8 tableNum)
{
	uint8 i = 0;
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	mcuIoOutTableNum = tableNum;
	p_mcuIoOutTable = p_table;
	for(i = 0; i < mcuIoOutTableNum; i++)
	{
		p_mcuIoOutTable[i].rollCount = 0;
		p_mcuIoOutTable[i].status = p_mcuIoOutTable[i].initStatus ? 1 : 0;
		RCC_APB2PeriphClockCmd(p_mcuIoOutTable[i].portClk, ENABLE);
		GPIO_InitStructure.GPIO_Pin = p_mcuIoOutTable[i].gpioPin;
		if(OUT_OD == p_mcuIoOutTable[i].mode)//开漏
		{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
		}
		else
		{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		}
		GPIO_Init(p_mcuIoOutTable[i].p_gpioPort, &GPIO_InitStructure);
		if(p_mcuIoOutTable[i].flip)
		{
			GPIO_WriteBit(p_mcuIoOutTable[i].p_gpioPort, p_mcuIoOutTable[i].gpioPin, (BitAction)!p_mcuIoOutTable[i].status);
		}
		else
		{
			GPIO_WriteBit(p_mcuIoOutTable[i].p_gpioPort, p_mcuIoOutTable[i].gpioPin, (BitAction)p_mcuIoOutTable[i].status);
		}
	}
}

/***********************************************************************************************
*函数名: uint16 mcu_ioout_get_ready(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint16 mcu_ioout_get_ready(uint8 channel)
{
	return p_mcuIoOutTable[channel].rollCount;
}

/***********************************************************************************************
*函数名: void mcu_ioout_clr_ready(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_ioout_clr_ready(uint8 channel)
{
	p_mcuIoOutTable[channel].rollCount = 0;
}

/***********************************************************************************************
*函数名: void mcu_ioout_set_status(uint8 channel, uint8 status)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_ioout_set_status(uint8 channel, uint8 status)
{
	p_mcuIoOutTable[channel].status = status;
}

/***********************************************************************************************
*函数名: uint8 mcu_ioout_get_status(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 mcu_ioout_get_status(uint8 channel)
{
	return p_mcuIoOutTable[channel].status;
}

/***********************************************************************************************
*函数名: void mcu_ioout_toggle(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_ioout_toggle(uint8 channel)
{
	p_mcuIoOutTable[channel].status = !p_mcuIoOutTable[channel].status;
}

/***********************************************************************************************
*函数名: void mcu_ioout_set_status_hard(uint8 channel, uint8 status)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_ioout_set_status_hard(uint8 channel, uint8 status)
{
	p_mcuIoOutTable[channel].status = status;
	if(p_mcuIoOutTable[channel].flip)
	{
		GPIO_WriteBit(p_mcuIoOutTable[channel].p_gpioPort, p_mcuIoOutTable[channel].gpioPin, (BitAction)!p_mcuIoOutTable[channel].status);
	}
	else
	{
		GPIO_WriteBit(p_mcuIoOutTable[channel].p_gpioPort, p_mcuIoOutTable[channel].gpioPin, (BitAction)p_mcuIoOutTable[channel].status);
	}
	updata_roll_count16(&p_mcuIoOutTable[channel].rollCount);
}

/***********************************************************************************************
*函数名: uint8 mcu_ioout_get_status_hard(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 mcu_ioout_get_status_hard(uint8 channel)
{
	uint8 re = 0;
	re = GPIO_ReadOutputDataBit(p_mcuIoOutTable[channel].p_gpioPort, p_mcuIoOutTable[channel].gpioPin) ? 1 : 0;
	return (re == p_mcuIoOutTable[channel].flip ? 0 : 1);
}

/***********************************************************************************************
*函数名: void mcu_ioout_toggle_hard(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_ioout_toggle_hard(uint8 channel)
{
	p_mcuIoOutTable[channel].status = !p_mcuIoOutTable[channel].status;
	if(p_mcuIoOutTable[channel].flip)
	{
		GPIO_WriteBit(p_mcuIoOutTable[channel].p_gpioPort, p_mcuIoOutTable[channel].gpioPin, (BitAction)!p_mcuIoOutTable[channel].status);
	}
	else
	{
		GPIO_WriteBit(p_mcuIoOutTable[channel].p_gpioPort, p_mcuIoOutTable[channel].gpioPin, (BitAction)p_mcuIoOutTable[channel].status);
	}
	updata_roll_count16(&p_mcuIoOutTable[channel].rollCount);
}

/***********************************************************************************************
*函数名: void mcu_ioout_main(void)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_ioout_main(void)
{
	uint8 i = 0;
	for(i = 0; i < mcuIoOutTableNum; i++)
	{
		if(p_mcuIoOutTable[i].flip)
		{
			GPIO_WriteBit(p_mcuIoOutTable[i].p_gpioPort, p_mcuIoOutTable[i].gpioPin, (BitAction)!p_mcuIoOutTable[i].status);
		}
		else
		{
			GPIO_WriteBit(p_mcuIoOutTable[i].p_gpioPort, p_mcuIoOutTable[i].gpioPin, (BitAction)p_mcuIoOutTable[i].status);
		}
		updata_roll_count16(&p_mcuIoOutTable[i].rollCount);
	}
}





////////////////////////////////////////////配置dio/////////////////////////////////////////////
static uint8 mcuDioTableNum = 0;
static MCU_DIO_TAB * p_mcuDioTable = NULL;
/***********************************************************************************************
*函数名: void mcu_dio_init(MCU_DIO_TAB *p_table, uint8 tableNum)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_dio_init(MCU_DIO_TAB *p_table, uint8 tableNum)
{
	uint8 i = 0;
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	mcuDioTableNum = tableNum;
	p_mcuDioTable = p_table;
	for(i = 0; i < mcuDioTableNum; i++)
	{
		RCC_APB2PeriphClockCmd(p_mcuDioTable[i].portClk, ENABLE);
		GPIO_InitStructure.GPIO_Pin = p_mcuDioTable[i].gpioPin;
		if(DI_FT == p_mcuDioTable[i].mode)
		{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		}
		else if(DI_PU == p_mcuDioTable[i].mode)
		{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		}
		else if(DI_PD == p_mcuDioTable[i].mode)
		{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		}
		else if(DO_PP == p_mcuDioTable[i].mode)
		{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		}
		else if(DO_OD == p_mcuDioTable[i].mode)
		{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
		}
		else
		{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		}
		GPIO_Init(p_mcuDioTable[i].p_gpioPort, &GPIO_InitStructure);
		if(DO_PP == p_mcuDioTable[i].mode ||\
			DO_OD == p_mcuDioTable[i].mode)
		{
			if(p_mcuDioTable[i].flip)
			{
				GPIO_WriteBit(p_mcuDioTable[i].p_gpioPort, p_mcuDioTable[i].gpioPin, (BitAction)!p_mcuDioTable[i].initStatus);
			}
			else
			{
				GPIO_WriteBit(p_mcuDioTable[i].p_gpioPort, p_mcuDioTable[i].gpioPin, (BitAction)p_mcuDioTable[i].initStatus);
			}
		}
	}
}

/***********************************************************************************************
*函数名: void mcu_dio_set_status(uint8 channel, uint8 status)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_dio_set_status(uint8 channel, uint8 status)
{
	if(DO_PP == p_mcuDioTable[channel].mode ||\
		DO_OD == p_mcuDioTable[channel].mode)
	{
		if(p_mcuDioTable[channel].flip)
		{
			GPIO_WriteBit(p_mcuDioTable[channel].p_gpioPort, p_mcuDioTable[channel].gpioPin, (BitAction)!status);
		}
		else
		{
			GPIO_WriteBit(p_mcuDioTable[channel].p_gpioPort, p_mcuDioTable[channel].gpioPin, (BitAction)status);
		}
	}
}

/***********************************************************************************************
*函数名: void mcu_dio_toggle(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_dio_toggle(uint8 channel)
{
	if(DO_PP == p_mcuDioTable[channel].mode ||\
		DO_OD == p_mcuDioTable[channel].mode)
	{
		GPIO_FlipBits(p_mcuDioTable[channel].p_gpioPort, p_mcuDioTable[channel].gpioPin);
	}
}

/***********************************************************************************************
*函数名: uint8 mcu_dio_get_status(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 mcu_dio_get_status(uint8 channel)
{
//	if(DO_PP == p_mcuDioTable[channel].mode ||\
//		DO_OD == p_mcuDioTable[channel].mode)
//	{
//		return ((GPIO_ReadOutputDataBit(p_mcuDioTable[channel].p_gpioPort, p_mcuDioTable[channel].gpioPin) ? 1 : 0) ==\
//			p_mcuDioTable[channel].flip) ? 0 : 1;
//	}
//	else
//	{
		return ((GPIO_ReadInputDataBit(p_mcuDioTable[channel].p_gpioPort, p_mcuDioTable[channel].gpioPin) ? 1 : 0) ==\
			p_mcuDioTable[channel].flip) ? 0 : 1;
//	}
}





////////////////////////////////////////////配置adc/////////////////////////////////////////////
static uint8 mcuAdcTableNum = 0;
static MCU_ADC_TAB * p_mcuAdcTable = NULL;
static volatile uint16 mcuAdcUserChIdx = 0;
static uint32 mcuAdcOverTimeStamp = 0;
static void (*pfun_mcuAdcMuxCallback)(uint16 *p_userChannel, uint16 codeValue, MCU_ADC_TAB *p_curTable) = NULL;
/***********************************************************************************************
*函数名: void mcu_adc_init(MCU_ADC_TAB *p_table, uint8 tableNum)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_adc_init(MCU_ADC_TAB *p_table, uint8 tableNum)
{
	uint8 i = 0;
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	mcuAdcTableNum = tableNum;
	p_mcuAdcTable = p_table;
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_DeInit(ADC1);
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	for(i = 0; i < mcuAdcTableNum; i++)
	{
		GPIO_InitStructure.GPIO_Pin = p_mcuAdcTable[i].gpioPin;
		GPIO_Init(p_mcuAdcTable[i].p_gpioPort, &GPIO_InitStructure);
	}
	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	mcuAdcUserChIdx = 0;
	ADC_RegularChannelConfig(ADC1, p_mcuAdcTable[mcuAdcUserChIdx].adcChannel, 1, ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/***********************************************************************************************
*函数名: uint16 mcu_adc_get_ready(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint16 mcu_adc_get_ready(uint8 channel)
{
	return p_mcuAdcTable[channel].rollCount;
}

/***********************************************************************************************
*函数名: void mcu_adc_clr_ready(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_adc_clr_ready(uint8 channel)
{
	p_mcuAdcTable[channel].rollCount = 0;
}

/***********************************************************************************************
*函数名: uint32 mcu_adc_get_voltage(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint32 mcu_adc_get_voltage(uint8 channel)
{
	return p_mcuAdcTable[channel].voltage;
}

/***********************************************************************************************
*函数名: void mcu_adc_main(void)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_adc_main(void)
{
	uint8 i = 0;
	if(mcuAdcUserChIdx >= mcuAdcTableNum)
	{
		for(i = 0; i < mcuAdcTableNum; i++)
		{
			p_mcuAdcTable[i].filterCnt++;
			p_mcuAdcTable[i].adcCodeSum += p_mcuAdcTable[i].adcCode;
			if(p_mcuAdcTable[i].filterCnt >= p_mcuAdcTable[i].filterLen)
			{
				p_mcuAdcTable[i].voltage = p_mcuAdcTable[i].adcCodeSum * 33000 / 4095 / p_mcuAdcTable[i].filterCnt *\
				p_mcuAdcTable[i].ratioFenzi / p_mcuAdcTable[i].ratioFenmu;
				updata_roll_count16(&p_mcuAdcTable[i].rollCount);
				p_mcuAdcTable[i].filterCnt = 0;
				p_mcuAdcTable[i].adcCodeSum = 0;
			}
		}
		mcuAdcUserChIdx = 0;
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		ADC_RegularChannelConfig(ADC1, p_mcuAdcTable[mcuAdcUserChIdx].adcChannel, 1, ADC_SampleTime_55Cycles5);
		mcuAdcOverTimeStamp = tim_get_count();
	}
	else
	{
		if(tim_check_timenow(mcuAdcOverTimeStamp, 200))
		{
			mcuAdcUserChIdx = 0;
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);
			ADC_RegularChannelConfig(ADC1, p_mcuAdcTable[mcuAdcUserChIdx].adcChannel, 1, ADC_SampleTime_55Cycles5);
			mcuAdcOverTimeStamp = tim_get_count();
		}
	}
}

/***********************************************************************************************
*函数名: void mcu_adc_isr(ADC_TypeDef *p_adc)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_adc_isr(ADC_TypeDef *p_adc)
{
	uint16 adcCode = 0;
	if(p_adc == p_mcuAdcTable[mcuAdcUserChIdx].p_adc/* &&\
		p_mcuAdcTable[mcuAdcUserChIdx].adcChannel == */)
	{
		if(SET == ADC_GetITStatus(p_adc, ADC_IT_EOC))
		{
			ADC_ClearITPendingBit(p_adc, ADC_IT_EOC);
			adcCode = ADC_GetConversionValue(p_adc);
			if(NULL != pfun_mcuAdcMuxCallback)
			{
				pfun_mcuAdcMuxCallback((uint16 *)&mcuAdcUserChIdx, adcCode, &p_mcuAdcTable[mcuAdcUserChIdx]);
			}
			else
			{
				p_mcuAdcTable[mcuAdcUserChIdx].adcCode = adcCode;
				mcuAdcUserChIdx++;
			}
			if(mcuAdcUserChIdx < mcuAdcTableNum)
			{
				ADC_RegularChannelConfig(p_mcuAdcTable[mcuAdcUserChIdx].p_adc, p_mcuAdcTable[mcuAdcUserChIdx].adcChannel, 1, ADC_SampleTime_55Cycles5);
				ADC_SoftwareStartConvCmd(p_mcuAdcTable[mcuAdcUserChIdx].p_adc, ENABLE);
				mcuAdcOverTimeStamp = tim_get_count();
			}
		}
	}
}

/***********************************************************************************************
*函数名: void mcu_adc_stop_cycle(void)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_adc_stop_cycle(void)
{
}

/***********************************************************************************************
*函数名: void mcu_adc_start_cycle(void)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_adc_start_cycle(void)
{
}

/***********************************************************************************************
*函数名: uint32 mcu_adc_trgget_voltage(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint32 mcu_adc_trgget_voltage(uint8 channel)
{
	return 0;
}

/***********************************************************************************************
*函数名: void mcu_adc_set_mux_callback(void(*pfun_callback)(uint16 *p_userChannel, uint16 codeValue, MCU_ADC_TAB *p_curTable))
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_adc_set_mux_callback(void(*pfun_callback)(uint16 *p_userChannel, uint16 codeValue, MCU_ADC_TAB *p_curTable))
{
	pfun_mcuAdcMuxCallback = pfun_callback;
}


//////////////////////////////////////////////配置pwmo///////////////////////////////////////////
static uint8 mcuPwmoTableNum = 0;
static MCU_PWMO_TAB *p_mcuPwmoTable = NULL;
/***********************************************************************************************
*函数名: void mcu_pwmo_init(MCU_PWMO_TAB *p_table, uint8 tableNum)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_pwmo_init(MCU_PWMO_TAB *p_table, uint8 tableNum)
{
	uint8 i = 0;
	mcuPwmoTableNum = tableNum;
	p_mcuPwmoTable = p_table;
	for(i = 0; i < mcuPwmoTableNum; i++)
	{
	
	}
}

/***********************************************************************************************
*函数名: void mcu_pwmo_set_freq_duty(uint8 channel, PWM_STATUS *p_status)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_pwmo_set_freq_duty(uint8 channel, PWM_STATUS *p_status)
{
	p_mcuPwmoTable[channel].status.duty = p_status->duty;
	p_mcuPwmoTable[channel].status.freq = p_status->freq;
	p_mcuPwmoTable[channel].periodTime = MATH_DIV_ROUND(10000000UL, p_mcuPwmoTable[channel].status.freq) - 1;
	p_mcuPwmoTable[channel].highTime = MATH_DIV_ROUND(p_mcuPwmoTable[channel].status.duty*p_mcuPwmoTable[channel].periodTime, 1000)+1;
}

/***********************************************************************************************
*函数名: void mcu_pwmo_set_freq(uint8 channel, uint32 freq01)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_pwmo_set_freq(uint8 channel, uint32 freq01)
{
	p_mcuPwmoTable[channel].status.freq = freq01;
	p_mcuPwmoTable[channel].periodTime = MATH_DIV_ROUND(10000000UL, p_mcuPwmoTable[channel].status.freq) - 1;
	p_mcuPwmoTable[channel].highTime = MATH_DIV_ROUND(p_mcuPwmoTable[channel].status.duty*p_mcuPwmoTable[channel].periodTime, 1000)+1;

}

/***********************************************************************************************
*函数名: void mcu_pwmo_set_duty(uint8 channel, uint16 duty01)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_pwmo_set_duty(uint8 channel, uint16 duty01)
{
	p_mcuPwmoTable[channel].status.duty = duty01;
	p_mcuPwmoTable[channel].highTime = MATH_DIV_ROUND(p_mcuPwmoTable[channel].status.duty*p_mcuPwmoTable[channel].periodTime, 1000)+1;
}

/***********************************************************************************************
*函数名: void mcu_pwmo_get(uint8 channel, PWM_STATUS *p_status)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_pwmo_get(uint8 channel, PWM_STATUS *p_status)
{
	memcpy(p_status, &p_mcuPwmoTable[channel].status, sizeof(PWM_STATUS));
}

/***********************************************************************************************
*函数名: PWM_STATUS * mcu_pwmo_get_src(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
PWM_STATUS *mcu_pwmo_get_src(uint8 channel)
{
	return &p_mcuPwmoTable[channel].status;
}





//////////////////////////////////////////////配置pwmi///////////////////////////////////////////
static uint8 mcuPwmiTableNum = 0;
static MCU_PWMI_TAB *p_mcuPwmiTable = NULL;
/***********************************************************************************************
*函数名: void mcu_pwmi_init(MCU_PWMI_TAB *p_table, uint8 tableNum)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_pwmi_init(MCU_PWMI_TAB *p_table, uint8 tableNum)
{
	uint8 i = 0;
	mcuPwmiTableNum = tableNum;
	p_mcuPwmiTable = p_table;
	for(i = 0; i < mcuPwmiTableNum; i++)
	{
	
	}
}

/***********************************************************************************************
*函数名: uint16 mcu_pwmi_get_ready(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint16 mcu_pwmi_get_ready(uint8 channel)
{
	return p_mcuPwmiTable[channel].rollCount;
}

/***********************************************************************************************
*函数名: void mcu_pwmi_clr_ready(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_pwmi_clr_ready(uint8 channel)
{
	p_mcuPwmiTable[channel].rollCount = 0;
}

/***********************************************************************************************
*函数名: void mcu_pwmi_get(uint8 channel, PWM_STATUS *p_status)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_pwmi_get(uint8 channel, PWM_STATUS *p_status)
{
	memcpy(p_status, &p_mcuPwmiTable[channel].status, sizeof(PWM_STATUS));
}

/***********************************************************************************************
*函数名: PWM_STATUS *mcu_pwmi_get_src(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
PWM_STATUS *mcu_pwmi_get_src(uint8 channel)
{
	return &p_mcuPwmiTable[channel].status;
}

/***********************************************************************************************
*函数名: void mcu_pwmi_main(void)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_pwmi_main(void)
{
	uint8 i = 0;
	uint32 periodTime = 0, highTime = 0;
	PWM_STATUS pwmiStatus;
	for(i = 0; i < mcuPwmiTableNum; i++)
	{
		if(p_mcuPwmiTable[i].ready)
		{//mcu捕获到pwmi
			p_mcuPwmiTable[i].lostCnt = 0;
			do{//临界区
				periodTime = p_mcuPwmiTable[i].periodTime;
				highTime = p_mcuPwmiTable[i].highTime;
			}while(periodTime != p_mcuPwmiTable[i].periodTime || highTime != p_mcuPwmiTable[i].highTime);
			p_mcuPwmiTable[i].ready = 0;
			if(highTime > periodTime)
			{
				highTime = periodTime;
			}
			//pwmiStatus.freq = MATH_DIV_ROUND(10000000UL, periodTime);
			//pwmiStatus.duty = MATH_DIV_ROUND(highTime*1000, periodTime);
			if(p_mcuPwmiTable[i].flip)
			{
				pwmiStatus.duty = 1000 - pwmiStatus.duty;
			}
			if(p_mcuPwmiTable[i].calib > 0)
			{
				pwmiStatus.duty += p_mcuPwmiTable[i].calib;
				if(pwmiStatus.duty > 1000)
				{
					pwmiStatus.duty = 1000;
				}
			}
			else if(p_mcuPwmiTable[i].calib < 0)
			{
				if(pwmiStatus.duty >= -p_mcuPwmiTable[i].calib)
				{
					pwmiStatus.duty += p_mcuPwmiTable[i].calib;
				}
				else
				{
					pwmiStatus.duty = 0;
				}
			}
			if(MATH_ABS(p_mcuPwmiTable[i].lastStatus.duty, pwmiStatus.duty) <= p_mcuPwmiTable[i].dutyTolerance\
					&& MATH_ABS(p_mcuPwmiTable[i].lastStatus.freq, pwmiStatus.freq) <= p_mcuPwmiTable[i].freqTolerance)
			{
				if(++p_mcuPwmiTable[i].filterCnt >= p_mcuPwmiTable[i].filterLen)
				{
					p_mcuPwmiTable[i].filterCnt = p_mcuPwmiTable[i].filterLen;
					p_mcuPwmiTable[i].status = pwmiStatus;
					updata_roll_count16(&p_mcuPwmiTable[i].rollCount);
				}
			}
			else
			{
				p_mcuPwmiTable[i].filterCnt = 0;
			}
			p_mcuPwmiTable[i].lastStatus = pwmiStatus;
		}
		else//未捕获到pwmi
		{
			if(++p_mcuPwmiTable[i].lostCnt >= p_mcuPwmiTable[i].lostLen)
			{
				p_mcuPwmiTable[i].lostCnt = 0;
				//if(p_mcuPwmiTable[i].p_etim->CH[p_mcuPwmiTable[i].channel].CTRL2.B.INPUT == p_mcuPwmiTable[i].flip)
				//{
				//	p_mcuPwmiTable[i].status.freq = 0;
				//	p_mcuPwmiTable[i].status.duty = 0;
				//}
				//else
				{
					p_mcuPwmiTable[i].status.freq = 0;
					p_mcuPwmiTable[i].status.duty = 1000;
				}
				updata_roll_count16(&p_mcuPwmiTable[i].rollCount);
				p_mcuPwmiTable[i].filterCnt = 0;
			}
		}
	}
}

/***********************************************************************************************
*函数名: void mcu_pwmi_isr(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_pwmi_isr(uint8 channel)
{

}





////////////////////////////////////////////配置flash/eep///////////////////////////////////////
static void (*pfun_mcuEraseCablck)(void) = NULL;
/***********************************************************************************************
*函数名: void mcu_flash_init(void)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_flash_init(void)
{
	pfun_mcuEraseCablck = NULL;
}

/***********************************************************************************************
*函数名: uint8 mcu_flash_get_data(uint32 addr, void *p_data, uint32 len)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 mcu_flash_get_data(uint32 addr, void *p_data, uint32 len)
{
	memcpy(p_data, (uint8 *)addr, len);
	return 0;
}

/***********************************************************************************************
*函数名: uint8 mcu_flash_set_data(uint32 addr, void *p_data, uint32 len)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 mcu_flash_set_data(uint32 addr, void *p_data, uint32 len)
{
	uint32 i = 0;
	uint8 *p = (uint8 *)p_data;
	FLASH_Unlock();
	if(len % 2)
	{
		for(i = 0; i < len - 1; i += 2)
		{
			if(*(uint16 *)addr != *(uint16 *)&p[i])
			{
				while(FLASH_COMPLETE != FLASH_ProgramHalfWord(addr, *(uint16 *)&p[i]));
			}
			addr += 2;
		}
		if(*(uint16 *)addr != *(uint16 *)&p[i])
		{
			while(FLASH_COMPLETE != FLASH_ProgramHalfWord(addr, *(uint8 *)&p[i] | 0xFF00));
		}
	}
	else
	{
		for(i = 0; i < len; i += 2)
		{
			if(*(uint16 *)addr != *(uint16 *)&p[i])
			{
				while(FLASH_COMPLETE != FLASH_ProgramHalfWord(addr, *(uint16 *)&p[i]));
			}
			addr += 2;
		}
	}
	FLASH_Lock();
	return 0;
}

/***********************************************************************************************
*函数名: uint8 mcu_flash_erase(uint32 addr)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 mcu_flash_erase(uint32 addr)
{
	FLASH_Unlock();
	while(FLASH_COMPLETE != FLASH_ErasePage(addr))
	{
		if(NULL != pfun_mcuEraseCablck)
		{
			pfun_mcuEraseCablck();
		}
	}
	FLASH_Lock();
	return 0;
}

/***********************************************************************************************
*函数名: void mcu_flash_erase_set_calback(void (*pfun_calBack)(void))
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_flash_erase_set_calback(void (*pfun_calBack)(void))
{
	pfun_mcuEraseCablck = pfun_calBack;
}





//////////////////////////////////////////////配置can///////////////////////////////////////////
static uint8 mcuCanTableNum = 0;
static MCU_CAN_TAB *p_mcuCanTable = NULL;
/***********************************************************************************************
*函数名: void mcu_can_init(MCU_CAN_TAB *p_table, uint8 tableNum)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_can_init(MCU_CAN_TAB *p_table, uint8 tableNum)
{
	uint8 i = 0;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	mcuCanTableNum = tableNum;
	p_mcuCanTable = p_table;
	for(i = 0; i < mcuCanTableNum; i++)
	{
		RCC_APB2PeriphClockCmd(p_mcuCanTable[i].txPortClk, ENABLE);
		RCC_APB2PeriphClockCmd(p_mcuCanTable[i].rxPortClk, ENABLE);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = p_mcuCanTable[i].txPin;
		GPIO_Init(p_mcuCanTable[i].p_txPort, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Pin = p_mcuCanTable[i].rxPin;
		GPIO_Init(p_mcuCanTable[i].p_rxPort, &GPIO_InitStructure);
		CAN_DeInit(p_mcuCanTable[i].p_can);
		RCC_APB1PeriphClockCmd(p_mcuCanTable[i].canClk, ENABLE);
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannel = p_mcuCanTable[i].irqChannel;
		NVIC_Init(&NVIC_InitStructure);
		NVIC_InitStructure.NVIC_IRQChannel = CAN1_SCE_IRQn;//
		NVIC_Init(&NVIC_InitStructure);//
		CAN_Init((CAN_TypeDef *)p_mcuCanTable[i].p_can, (CAN_InitTypeDef *)&p_mcuCanTable[i].initStruct);
		CAN_FilterInit((CAN_FilterInitTypeDef* )&p_mcuCanTable[i].filterStruct);
		CAN_ITConfig(p_mcuCanTable[i].p_can, CAN_IT_FMP0 | CAN_IT_FMP1 | CAN_IT_ERR | CAN_IT_BOF, ENABLE);
	}
}

/***********************************************************************************************
*函数名: void mcu_can_reinit(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_can_reinit(uint8 channel)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(p_mcuCanTable[channel].txPortClk, ENABLE);
	RCC_APB2PeriphClockCmd(p_mcuCanTable[channel].rxPortClk, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = p_mcuCanTable[channel].txPin;
	GPIO_Init(p_mcuCanTable[channel].p_txPort, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = p_mcuCanTable[channel].rxPin;
	GPIO_Init(p_mcuCanTable[channel].p_rxPort, &GPIO_InitStructure);
	CAN_DeInit(p_mcuCanTable[channel].p_can);
	RCC_APB1PeriphClockCmd(p_mcuCanTable[channel].canClk, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannel = p_mcuCanTable[channel].irqChannel;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_SCE_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	CAN_Init((CAN_TypeDef *)p_mcuCanTable[channel].p_can, (CAN_InitTypeDef *)&p_mcuCanTable[channel].initStruct);
	CAN_FilterInit((CAN_FilterInitTypeDef* )&p_mcuCanTable[channel].filterStruct);
	CAN_ITConfig(p_mcuCanTable[channel].p_can, CAN_IT_FMP0 | CAN_IT_FMP1 | CAN_IT_ERR | CAN_IT_BOF, ENABLE);
}
/***********************************************************************************************
*函数名: uint8 mcu_can_send(uint8 channel, CAN_MSG* p_send)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 mcu_can_send(uint8 channel, CAN_MSG* p_send)
{
	CanTxMsg TxMessage;
	if(p_send->ide == CAN_IDE_EXTENDED)
	{
		TxMessage.IDE = CAN_Id_Extended;
		TxMessage.ExtId = p_send->id;
	}
	else
	{
		TxMessage.IDE = CAN_Id_Standard;
		TxMessage.StdId = p_send->id;
	}
	TxMessage.DLC = p_send->dlc;
	if(p_send->rtr == CAN_FRAM_RTR_DATA)
	{
		TxMessage.RTR = CAN_RTR_DATA;
	}
	else
	{
		TxMessage.RTR = CAN_RTR_REMOTE;
	}
	memcpy(TxMessage.Data, p_send->a_data, 8);
	if(CAN_TxStatus_NoMailBox == CAN_Transmit(p_mcuCanTable[channel].p_can, &TxMessage))
	{
		return 1;
	}
	return 0;
}

/***********************************************************************************************
*函数名: void mcu_can_rx_isr(uint8 channel, CAN_MSG* p_recv)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_can_rx_isr(uint8 channel, CAN_MSG* p_recv)
{
	static CanRxMsg RxMessage;
	CAN_Receive(p_mcuCanTable[channel].p_can, CAN_FIFO0, &RxMessage);
	if(RxMessage.IDE == CAN_Id_Extended)
	{
		p_recv->ide = CAN_IDE_EXTENDED;
		p_recv->id = RxMessage.ExtId;
	}
	else
	{
		p_recv->ide = CAN_IDE_STANDARD;
		p_recv->id = RxMessage.StdId;
	}
	p_recv->dlc = RxMessage.DLC;
	if(RxMessage.RTR == CAN_FRAM_RTR_DATA)
	{
		p_recv->rtr = CAN_FRAM_RTR_DATA;
	}
	else
	{
		p_recv->rtr = CAN_FRAM_RTR_REMO;
	}
	memcpy(p_recv->a_data, RxMessage.Data, 8);
	if(NULL != p_mcuCanTable[channel].pfun_rxCallBack)
	{
		p_mcuCanTable[channel].pfun_rxCallBack(p_recv);
	}
}





//////////////////////////////////////////////配置mspi///////////////////////////////////////////
static uint8 mcuMspiTableNum = 0;
static MCU_MSPI_TAB * p_mcuMspiTable = NULL;
/***********************************************************************************************
*函数名: void mcu_mspi_init(MCU_MSPI_TAB *p_table, uint8 tableNum)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_mspi_init(MCU_MSPI_TAB *p_table, uint8 tableNum)
{
	uint8 i = 0;
	GPIO_InitTypeDef GPIO_InitStructure;
	mcuMspiTableNum = tableNum;
	p_mcuMspiTable = p_table;
	for(i = 0; i < mcuMspiTableNum; i++)
	{
		RCC_APB2PeriphClockCmd(p_mcuMspiTable[i].csPortClk | p_mcuMspiTable[i].sckPortClk |\
								p_mcuMspiTable[i].mosiPortClk | p_mcuMspiTable[i].misoPortClk, ENABLE);
		if(p_mcuMspiTable[i].portRemap)
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
			GPIO_PinRemapConfig(p_mcuMspiTable[i].portRemap, ENABLE);
		}
		if(SPI1 == p_mcuMspiTable[i].p_mspi)
		{
			RCC_APB2PeriphClockCmd(p_mcuMspiTable[i].mspiClk, ENABLE);
		}
		else
		{
			RCC_APB1PeriphClockCmd(p_mcuMspiTable[i].mspiClk, ENABLE);
		}
		SPI_I2S_DeInit(p_mcuMspiTable[i].p_mspi);
		GPIO_InitStructure.GPIO_Pin = p_mcuMspiTable[i].csPin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		if(SPI_NSS_Hard != p_mcuMspiTable[i].initStruct.SPI_NSS)
		{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(p_mcuMspiTable[i].p_csPort, &GPIO_InitStructure);
			if(p_mcuMspiTable[i].csDisable)
			{
				GPIO_SetBits(p_mcuMspiTable[i].p_csPort, p_mcuMspiTable[i].csPin);
			}
			else
			{
				GPIO_ResetBits(p_mcuMspiTable[i].p_csPort, p_mcuMspiTable[i].csPin);
			}
		}
		else
		{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(p_mcuMspiTable[i].p_csPort, &GPIO_InitStructure);
		}
		GPIO_InitStructure.GPIO_Pin = p_mcuMspiTable[i].sckPin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(p_mcuMspiTable[i].p_sckPort, &GPIO_InitStructure);
		if(NULL != p_mcuMspiTable[i].p_mosiPort)
		{
			GPIO_InitStructure.GPIO_Pin = p_mcuMspiTable[i].mosiPin;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(p_mcuMspiTable[i].p_mosiPort, &GPIO_InitStructure);
		}
		if(NULL != p_mcuMspiTable[i].p_misoPort)
		{
			GPIO_InitStructure.GPIO_Pin = p_mcuMspiTable[i].misoPin;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(p_mcuMspiTable[i].p_misoPort, &GPIO_InitStructure);
		}
		SPI_Init(p_mcuMspiTable[i].p_mspi, &p_mcuMspiTable[i].initStruct);
		SPI_Cmd(p_mcuMspiTable[i].p_mspi, ENABLE);
	}
}

/***********************************************************************************************
*函数名: uint8 mcu_mspi_txrx(uint8 channel, uint8 send)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 mcu_mspi_txrx(uint8 channel, uint8 send)
{
	while(SPI_I2S_GetFlagStatus(p_mcuMspiTable[channel].p_mspi, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(p_mcuMspiTable[channel].p_mspi, send);
	while (SPI_I2S_GetFlagStatus(p_mcuMspiTable[channel].p_mspi, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(p_mcuMspiTable[channel].p_mspi);
}

/***********************************************************************************************
*函数名: void mcu_mspi_txrx_continue(uint8 channel, void *p_tx, void *p_rx, uint16 dataLen)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_mspi_txrx_continue(uint8 channel, void *p_tx, void *p_rx, uint16 dataLen)
{
	uint16 i = 0;
	for(i = 0; i < dataLen; i++)
	{
		while(SPI_I2S_GetFlagStatus(p_mcuMspiTable[channel].p_mspi, SPI_I2S_FLAG_TXE) == RESET);
		SPI_I2S_SendData(p_mcuMspiTable[channel].p_mspi, ((uint8*)p_tx)[i]);
		while (SPI_I2S_GetFlagStatus(p_mcuMspiTable[channel].p_mspi, SPI_I2S_FLAG_RXNE) == RESET);
		((uint8*)p_rx)[i] = SPI_I2S_ReceiveData(p_mcuMspiTable[channel].p_mspi);
	}
}

/***********************************************************************************************
*函数名: void mcu_mspi_cs_enable(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_mspi_cs_enable(uint8 channel)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	if(SPI_NSS_Hard != p_mcuMspiTable[channel].initStruct.SPI_NSS)
	{
		if(p_mcuMspiTable[channel].csDisable)
		{
			GPIO_ResetBits(p_mcuMspiTable[channel].p_csPort, p_mcuMspiTable[channel].csPin);
		}
		else
		{
			GPIO_SetBits(p_mcuMspiTable[channel].p_csPort, p_mcuMspiTable[channel].csPin);
		}
	}
	else
	{
		if(p_mcuMspiTable[channel].csDisable)
		{
			GPIO_ResetBits(p_mcuMspiTable[channel].p_csPort, p_mcuMspiTable[channel].csPin);
		}
		else
		{
			GPIO_SetBits(p_mcuMspiTable[channel].p_csPort, p_mcuMspiTable[channel].csPin);
		}
		GPIO_InitStructure.GPIO_Pin = p_mcuMspiTable[channel].csPin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(p_mcuMspiTable[channel].p_csPort, &GPIO_InitStructure);
	}
}

/***********************************************************************************************
*函数名: void mcu_mspi_cs_disable(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_mspi_cs_disable(uint8 channel)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	if(SPI_NSS_Hard != p_mcuMspiTable[channel].initStruct.SPI_NSS)
	{
		if(p_mcuMspiTable[channel].csDisable)
		{
			GPIO_SetBits(p_mcuMspiTable[channel].p_csPort, p_mcuMspiTable[channel].csPin);
		}
		else
		{
			GPIO_ResetBits(p_mcuMspiTable[channel].p_csPort, p_mcuMspiTable[channel].csPin);
		}
	}
	else
	{
		GPIO_InitStructure.GPIO_Pin = p_mcuMspiTable[channel].csPin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(p_mcuMspiTable[channel].p_csPort, &GPIO_InitStructure);
	}
}





//////////////////////////////////////////////配置sspi///////////////////////////////////////////
static uint8 mcuSspiTableNum = 0;
static MCU_SSPI_TAB * p_mcuSspiTable = NULL;
/***********************************************************************************************
*函数名: void mcu_sspi_init(MCU_SSPI_TAB *p_table, uint8 tableNum)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_sspi_init(MCU_SSPI_TAB *p_table, uint8 tableNum)
{
	uint8 i = 0;
	mcuSspiTableNum = tableNum;
	p_mcuSspiTable = p_table;
	for(i = 0; i < mcuSspiTableNum; i++)
	{
	
	}
}

/***********************************************************************************************
*函数名: void mcu_sspi_start(uint8 channel, void *p_send, uint16 sendLen)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_sspi_start(uint8 channel, void *p_send, uint16 sendLen)
{

}

/***********************************************************************************************
*函数名: uint16 mcu_sspi_check_recv(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint16 mcu_sspi_check_recv(uint8 channel)
{
	return p_mcuSspiTable[channel].txRxIndex;
}

/***********************************************************************************************
*函数名: void mcu_sspi_get_recv(uint8 channel, void *p_recv, uint16 recvLen)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_sspi_get_recv(uint8 channel, void *p_recv, uint16 recvLen)
{
	if(recvLen > p_mcuSspiTable[channel].txRxIndex)
	{
		memcpy(p_recv, p_mcuSspiTable[channel].p_rxBuff, p_mcuSspiTable[channel].txRxIndex);
	}
	else
	{
		memcpy(p_recv, p_mcuSspiTable[channel].p_rxBuff, recvLen);
	}
}

/***********************************************************************************************
*函数名: void mcu_sspi_stop(uint8 channel)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_sspi_stop(uint8 channel)
{

}

/***********************************************************************************************
*函数名: void mcu_sspi_tx_isr(uint8 channel, void *p_send)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_sspi_tx_isr(uint8 channel, void *p_send)
{

}

/***********************************************************************************************
*函数名: void mcu_sspi_rx_isr(uint8 channel, void *p_recv)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_sspi_rx_isr(uint8 channel, void *p_recv)
{

}





//////////////////////////////////////////////配置uart///////////////////////////////////////////
static uint8 mcuUartTableNum = 0;
static MCU_UART_TAB * p_mcuUartTable = NULL;
/***********************************************************************************************
*函数名: void mcu_uart_init(MCU_UART_TAB *p_table, uint8 tableNum)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_uart_init(MCU_UART_TAB *p_table, uint8 tableNum)
{
	uint8 i = 0;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	mcuUartTableNum = tableNum;
	p_mcuUartTable = p_table;
	for(i = 0; i < mcuUartTableNum; i++)
	{
		RCC_APB2PeriphClockCmd(p_mcuUartTable[i].txPortClk, ENABLE);
		RCC_APB2PeriphClockCmd(p_mcuUartTable[i].rxPortClk, ENABLE);
		if(p_mcuUartTable[i].portRemap)//管脚是否映射
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
			GPIO_PinRemapConfig(p_mcuUartTable[i].portRemap, ENABLE);
		}
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Pin = p_mcuUartTable[i].rxPin;
		GPIO_Init(p_mcuUartTable[i].p_rxPort, &GPIO_InitStructure);//初始化接收管脚
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Pin = p_mcuUartTable[i].txPin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(p_mcuUartTable[i].p_txPort, &GPIO_InitStructure);//初始化发送管脚
		USART_DeInit(p_mcuUartTable[i].p_uart);//复位串口
		if(USART1 == p_mcuUartTable[i].p_uart)//发开串口时钟
		{
			RCC_APB2PeriphClockCmd(p_mcuUartTable[i].uartClk, ENABLE);
		}
		else
		{
			RCC_APB1PeriphClockCmd(p_mcuUartTable[i].uartClk, ENABLE);
		}
		NVIC_InitStructure.NVIC_IRQChannel = p_mcuUartTable[i].irqChannel;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);//配置串口中断
		USART_Init((USART_TypeDef* )p_mcuUartTable[i].p_uart, (USART_InitTypeDef* )&p_mcuUartTable[i].initStruct);
		USART_ITConfig(p_mcuUartTable[i].p_uart, USART_IT_RXNE, ENABLE);//使能接收中断
		USART_ClearFlag(p_mcuUartTable[i].p_uart, USART_FLAG_RXNE);
		USART_Cmd(p_mcuUartTable[i].p_uart, ENABLE);//使能串口
	}
}

/***********************************************************************************************
*函数名: uint8 mcu_uart_tx(uint8 channel, void *p_tx, uint16 len)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 mcu_uart_tx(uint8 channel, void *p_tx, uint16 len)
{
	uint16 i = 0;
	for(i = 0; i < len; i++)
	{
		USART_SendData(p_mcuUartTable[channel].p_uart, ((uint8*)p_tx)[i]);
		while (USART_GetFlagStatus(p_mcuUartTable[channel].p_uart, USART_FLAG_TXE) == RESET);
	}
	return 0;
}

/***********************************************************************************************
*函数名: void mcu_uart_rx_isr(uint8 channel, uint8 *p_recv)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void mcu_uart_rx_isr(uint8 channel, uint8 *p_recv)
{
	uint8 data = 0;
	if(USART_GetITStatus(p_mcuUartTable[channel].p_uart, USART_IT_RXNE) != RESET)
	{
		data = USART_ReceiveData(p_mcuUartTable[channel].p_uart);
		if(NULL != p_recv)
		{
			*p_recv = data;
		}
		if(NULL != p_mcuUartTable[channel].pfun_rxCallBack)
		{
			p_mcuUartTable[channel].pfun_rxCallBack(data);
		}
	}
}





