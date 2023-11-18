#include "stm32f10x.h"
#include "usart2.h"
#include "senser.h"
#include "string.h"


uint16_t Time;
extern unsigned char ucRxFinish;
extern unsigned char ucRxData[100];

void Delay(void)
{
	unsigned int i=0;
	while(++i<1000);
}

void GetData(unsigned char *ucCMD)
{
	unsigned int uiData;
	USART2_Send(ucCMD,8);
	Time=0;
	while((Time<100)&&(ucRxFinish==0))
	{ Delay();}
	if(ucRxFinish==1)
	{
		uiData=ucRxData[3]<<8|ucRxData[4];
		printf("Sensor %d Data:%d\r\n",ucRxData[0],uiData);		
	}
	ucRxFinish=0;
}
