/*********************************************************************************
	*Copyright(C),20xx-20xx,

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
#ifndef __TIMER_H_
#define __TIMER_H_
#include"type.h"

#pragma pack(1)
typedef struct{
	uint16	intvTime;
	void (*pfun_mcuTimInit)(uint16);		//可选
	void (*pfun_mcuTimDelayus)(uint16);		//必须
	void (*pfun_mcuKiDog)(void);			//可选
	uint32 rtcInitUnixTime;					//默认rtc时间
	uint8 (*pfun_rtcInit)(uint32 *);		//可选
	uint8 (*pfun_rtcSetUnixTime)(uint32);	//可选
	uint8 (*pfun_rtcGetUnixTime)(uint32 *);	//可选
}TIM_CONFIG;
#pragma pack()

#pragma pack(1)
typedef struct{
	uint16	intvTime;
	volatile uint32	systemMs;
	volatile uint32 rtcUnixTime;
	uint8	rtcStatus;
}TIM;
#pragma pack()
extern TIM tim;

void tim_init(TIM_CONFIG *p_config);
void tim_isr(void);
uint32 tim_get_count(void);
uint8 tim_check_timeout(uint32 start, uint32 now, uint32 invt);
uint8 tim_check_timenow(uint32 start, uint32 invt);
void tim_delayus(uint16 us);
void tim_delayms(uint32 ms);
uint32 tim_calcu_interval(uint32 oldTime, uint32 newTime);
uint32 tim_get_rtc_unixtime(void);
uint8 tim_set_rtc_unixtime(uint32 unixTime);

#endif//__TIMER_H_
