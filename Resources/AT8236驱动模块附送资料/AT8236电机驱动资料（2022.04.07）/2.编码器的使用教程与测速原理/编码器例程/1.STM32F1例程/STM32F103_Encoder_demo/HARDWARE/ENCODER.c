#include "ENCODER.h"

/*TIM2��ʼ��Ϊ�������ӿ�*/
void Encoder_Init_TIM4(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure; //����һ�����ų�ʼ���Ľṹ��  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//����һ����ʱ����ʼ���Ľṹ��
  TIM_ICInitTypeDef TIM_ICInitStructure; //����һ����ʱ��������ģʽ��ʼ���Ľṹ��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʹ��TIM4ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��CPIOBʱ��
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//PB6��PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	//����GPIO_InitStructure�Ĳ�����ʼ��GPIOB0

	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc; // Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ѡ��ʱ�ӷ�Ƶ������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct�Ĳ�����ʼ����ʱ��TIM4
	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3��CH1��CH2ͬʱ�������ķ�Ƶ
	TIM_ICStructInit(&TIM_ICInitStructure); //��TIM_ICInitStruct �е�ÿһ��������ȱʡֵ����
	TIM_ICInitStructure.TIM_ICFilter = 10;  //�����˲�������
	TIM_ICInit(TIM4, &TIM_ICInitStructure); //��TIM_ICInitStructure������ʼ����ʱ��TIM4������ģʽ

	TIM_Cmd(TIM4, ENABLE); //ʹ�ܶ�ʱ��4
}

//��ȡ����������
int Read_Encoder_TIM4(void)
{
	int Encoder_TIM;
	Encoder_TIM=TIM4->CNT; //��ȡ����
	if(Encoder_TIM>0xefff)Encoder_TIM=Encoder_TIM-0xffff; //ת������ֵΪ�з����ֵ������0��ת��С��0��ת��
	                                                      //TIM4->CNT��ΧΪ0-0xffff����ֵΪ0��
	TIM4->CNT=0; //��ȡ����������
	return Encoder_TIM; //����ֵ
}







