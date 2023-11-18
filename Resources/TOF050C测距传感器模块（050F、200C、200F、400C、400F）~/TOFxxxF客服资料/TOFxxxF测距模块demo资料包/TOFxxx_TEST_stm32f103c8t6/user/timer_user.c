#include "timer_user.h"
#include "mcu_hal_user.h"
//#include "pca21125.h"




//static void rtc_try_init(void)
//{
//	uint32 time = tim_get_rtc_unixtime();
//	if(pca21125_init_unixtime(&time))
//	{
//		return;
//	}
//	tim.rtcStatus = 1;
//	tim_set_rtc_unixtime(time);
//}
//static void mcu_tim_user_init(uint16 invt)
//{
//	mcu_tim_init_call(invt, tim_isr);
//}
void tim_user_init(void)
{
	TIM_CONFIG timConfig;
	timConfig.intvTime = 1;
	timConfig.pfun_mcuTimInit = mcu_tim_init;
	timConfig.pfun_mcuTimDelayus = mcu_tim_delayus;
	timConfig.pfun_mcuKiDog = NULL;
	timConfig.rtcInitUnixTime = 0;//DefaultUnixTime;
	timConfig.pfun_rtcInit = NULL;//pca21125_init_unixtime;
	timConfig.pfun_rtcSetUnixTime = NULL;//pca21125_set_unixtime;
	timConfig.pfun_rtcGetUnixTime = NULL;//pca21125_get_unixtime;
	tim_init(&timConfig);
//	if(2 == tim.rtcStatus)//由于硬件rtc供电可能较慢,再次初始化
//	{
//		os_creat_callback(rtc_try_init, 1000);
//	}
}
