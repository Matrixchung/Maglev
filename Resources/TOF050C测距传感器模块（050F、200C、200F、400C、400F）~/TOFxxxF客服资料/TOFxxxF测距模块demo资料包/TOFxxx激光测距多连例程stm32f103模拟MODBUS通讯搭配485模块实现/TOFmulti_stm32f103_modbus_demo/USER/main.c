#include "main.h"

//TOFxxx获取传感器数据命令
//

unsigned char CMD_1[8]={0x01,0x03,0x00,0x10,0x00,0x01,0x85,0xcf};
unsigned char CMD_2[8]={0x02,0x03,0x00,0x10,0x00,0x01,0x85,0xfc};//02 03 00 10 00 01 85 FC
unsigned char CMD_3[8]={0x03,0x03,0x00,0x10,0x00,0x01,0x84,0x2d};//03 03 00 10 00 01 84 2D

int main(void)
{
	SysTick_Config(72000);//1ms
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断分组配置
	USART1_Init(115200);
	USART2_Init(115200);	
	delay_ms(10);

	while(1)
	{ 
		 delay_ms(500);
		
		   GetData(CMD_1);
		
		delay_ms(500);
		   GetData(CMD_2);
		
	//	delay_ms(1000);
	//	   GetData(CMD_3);
	}
}