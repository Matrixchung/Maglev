#include "ENCODER.h"
#include "usart.h"   				
#include "delay.h"

int main(void)
{
	delay_init();
	uart_init(9600);
	Encoder_Init_TIM4(0xffff,0);
  while(1)
	{		 
		delay_ms(200); //ÿ��200ms��ȡһ�α��������������ٶȡ�
		               //����ʹ�ö�ʱ�ж�ʵ�ָ���׼���ٶȼ��㣬�û����Զ���
    printf("Encoder=%d\r\n", Read_Encoder_TIM4());
	}
}
