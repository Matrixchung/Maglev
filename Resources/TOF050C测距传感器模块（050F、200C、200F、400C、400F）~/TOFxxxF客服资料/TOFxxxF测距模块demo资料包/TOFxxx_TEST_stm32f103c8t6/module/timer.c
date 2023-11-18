/*********************************************************************************
	*Copyright(C),20xx-20xx,

	*Filename:timer.c

	*Author:
			wwyyy
			q1046685883

	*Version:
			V1.0

	*Data:
			2019/11/16

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
#include "timer.h"
#include "string.h"

TIM tim;
static TIM_CONFIG timConfig;


/***********************************************************************************************
*������: void tim_init(TIM_CONFIG *p_config)
*��������:
*�ر�˵��:
*��������:
*��������ֵ:
*�޸ļ�¼:
***********************************************************************************************/
void tim_init(TIM_CONFIG *p_config)
{
	memset(&tim, 0, sizeof(tim));
	memcpy(&timConfig, p_config, sizeof(timConfig));
	tim.intvTime = timConfig.intvTime;
	if(NULL != timConfig.pfun_rtcInit)
	{
		if(timConfig.pfun_rtcInit(&timConfig.rtcInitUnixTime))
		{
			tim.rtcStatus = 2;//��ʼ��ʧ��
		}
		else
		{
			tim.rtcStatus = 1;//��ʼ���ɹ�
		}
	}
	tim.rtcUnixTime = timConfig.rtcInitUnixTime;
	if(NULL != timConfig.pfun_mcuTimInit)
	{
		timConfig.pfun_mcuTimInit(timConfig.intvTime);
	}
}

/***********************************************************************************************
*������: void tim_isr(void)
*��������:
*�ر�˵��:
*��������:
*��������ֵ:
*�޸ļ�¼:
***********************************************************************************************/
void tim_isr(void)
{
	static uint32 oneSecond = 0;
	tim.systemMs += tim.intvTime;
	oneSecond += tim.intvTime;
	if(oneSecond >= 1000)
	{
		oneSecond -= 1000;
		tim.rtcUnixTime++;
	}
}

/***********************************************************************************************
*������: uint32 tim_get_count(void)
*��������:
*�ر�˵��:
*��������:
*��������ֵ:
*�޸ļ�¼:
***********************************************************************************************/
uint32 tim_get_count(void)
{
	uint32 count;;
	do{
		count = tim.systemMs;
	}while(count != tim.systemMs);
	return count;
}
//void tim_enable_interrupt(void)
//{
//	mcu_tim_enable_interrupt();
//}
//void tim_disable_interrupt(void)
//{
//	mcu_tim_disable_interrupt();
//}

/***********************************************************************************************
*������: uint8 tim_check_timeout(uint32 start, uint32 now, uint32 invt)
*��������:
*�ر�˵��:
*��������:
*��������ֵ:
*�޸ļ�¼:
***********************************************************************************************/
uint8 tim_check_timeout(uint32 start, uint32 now, uint32 invt)
{
	if((uint32)(start + invt) >= start)//δ���
	{
		if((now >= (uint32)(start + invt)) || (now < start))
		{
			return 1;
		}
	}
	else//���
	{
		if((now < start) && (now >= (uint32)(start + invt)))
		{
			return 1;
		}
	}
	return 0;
}

/***********************************************************************************************
*������: uint8 tim_check_timenow(uint32 start, uint32 invt)
*��������:
*�ر�˵��:
*��������:
*��������ֵ:
*�޸ļ�¼:
***********************************************************************************************/
uint8 tim_check_timenow(uint32 start, uint32 invt)
{
	uint32 now = tim_get_count();
	if((uint32)(start + invt) >= start)//δ���
	{
		if((now >= (uint32)(start + invt)) || (now < start))
		{
			return 1;
		}
	}
	else//���
	{
		if((now < start) && (now >= (uint32)(start + invt)))
		{
			return 1;
		}
	}
	return 0;
}
/***********************************************************************************************
*������: void tim_delayus(uint16 us)
*��������:
*�ر�˵��:
*��������:
*��������ֵ:
*�޸ļ�¼:
***********************************************************************************************/
void tim_delayus(uint16 us)
{
	if(NULL != timConfig.pfun_mcuTimDelayus)
	{
		timConfig.pfun_mcuTimDelayus(us);
	}
}

/***********************************************************************************************
*������: void tim_delayms(uint32 ms)
*��������:
*�ر�˵��:
*��������:
*��������ֵ:
*�޸ļ�¼:
***********************************************************************************************/
void tim_delayms(uint32 ms)
{
	uint32 count = tim_get_count();
	while(0 == tim_check_timeout(count, tim_get_count(), ms))
	{
		if(NULL != timConfig.pfun_mcuKiDog)
		{
			timConfig.pfun_mcuKiDog();
		}
	}
}

/***********************************************************************************************
*������: uint32 tim_calcu_interval(uint32 oldTime, uint32 newTime)
*��������:
*�ر�˵��:
*��������:
*��������ֵ:
*�޸ļ�¼:
***********************************************************************************************/
uint32 tim_calcu_interval(uint32 oldTime, uint32 newTime)
{ 
	if(newTime >= oldTime)
	{
		return (newTime - oldTime);
	}
	else
	{
		return (uint32)(0xFFFFFFFF) - oldTime + newTime + 1;
	}
}

/***********************************************************************************************
*������: uint32 tim_get_rtc_unixtime(void)
*��������:
*�ر�˵��:
*��������:
*��������ֵ:
*�޸ļ�¼:
***********************************************************************************************/
uint32 tim_get_rtc_unixtime(void)
{
	uint32 unixTime = 0;
	static uint32 lastAdjustTime = 0;
	do{
		unixTime = tim.rtcUnixTime;
	}while(tim.rtcUnixTime != unixTime);
	if(NULL != timConfig.pfun_rtcGetUnixTime && lastAdjustTime + 1200 < unixTime)
	{
		lastAdjustTime = unixTime;
		if(timConfig.pfun_rtcGetUnixTime(&unixTime))
		{
			;//��ȡʧ��
		}
		else
		{//����rtc���ݵ�tim��
			do{
				tim.rtcUnixTime = unixTime;
			}while(tim.rtcUnixTime != unixTime);
		}
	}
	return unixTime;
}

/***********************************************************************************************
*������: uint8 tim_set_rtc_unixtime(uint32 unixTime)
*��������:
*�ر�˵��:
*��������:
*��������ֵ:
*�޸ļ�¼:
***********************************************************************************************/
uint8 tim_set_rtc_unixtime(uint32 unixTime)
{
	uint8 re = 0;
	do
	{
		tim.rtcUnixTime = unixTime;
	}while(tim.rtcUnixTime != unixTime);
	if(NULL != timConfig.pfun_rtcSetUnixTime)
	{
		if(timConfig.pfun_rtcSetUnixTime(unixTime))
		{
			tim.rtcStatus = 2;//ʧ��
			re = 1;
		}
		else
		{
			tim.rtcStatus = 1;//�ɹ�
			re = 0;
		}
	}
	return re;
}
