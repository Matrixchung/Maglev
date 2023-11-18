#include "delay.h"


static u32 Time=0;	

void Decline(void)
{
	if(Time>0)
		Time--;

}
		    								   

void delay_ms(u16 nms)
{	 
	Time=nms;
	while(Time!=0);    
} 








































