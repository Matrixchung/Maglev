/*********************************************************************************
	*Copyright(C),20xx-20xx,

	*Filename:mcu_hal.h

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
#ifndef __MCU_HAL_H_
#define __MCU_HAL_H_
#include "type.h"
#include "stm32f10x.h"





/////////////////////////////////////////////����core///////////////////////////////////////////////
void mcu_init(void);
void mcu_disable_irq(void);
void mcu_enable_irq(void);
void mcu_kick_dog(void);
void mcu_soft_reset(void);





//////////////////////////////////////////////����tim//////////////////////////////////////////////
void mcu_tim_init(uint16 intvTime);
void mcu_tim_init_call(uint16 intvTime, void(*pfun_callBack)(void));
void mcu_tim_delayus(uint16 delayCount);
void mcu_tim_isr(void);//��������ע��ص�




//////////////////////////////////////////////����ioin/////////////////////////////////////////////
typedef enum
{
	IN_FT = 0,	//����
	IN_PU,		//����
	IN_PD,		//����
}IOIN_MODE;
typedef struct{
	uint32			portClk;	//clk
	GPIO_TypeDef	*p_gpioPort;//port
	uint16			gpioPin;	//pin
	IOIN_MODE		mode;		//0:float,1:ipu,2:ipd
	uint8			flip;		//0:����ת,1:��ת
	uint16			filterLen;	//�˲�����
	//������������
	uint16			filterCnt;
	uint16			rollCount;
	uint8			status;
	uint8			lastStatus;
}MCU_IOIN_TAB;
void mcu_ioin_init(MCU_IOIN_TAB *p_table, uint8 tableNum);
uint16 mcu_ioin_get_ready(uint8 channel);
void mcu_ioin_clr_ready(uint8 channel);
uint8 mcu_ioin_get_status(uint8 channel);
void mcu_ioin_main(void);





///////////////////////////////////////////ioout////////////////////////////////////////////////
typedef enum
{
	OUT_PP = 0,	//����
	OUT_OD,		//��©(������)
}IOOUT_MODE;
typedef struct{
	uint32			portClk;	//clk
	GPIO_TypeDef	*p_gpioPort;//port
	uint16			gpioPin;	//pin
	IOOUT_MODE		mode;		//:0:pp,1:od
	uint8			flip;		//0:����ת,1:��ת
	uint8			initStatus;	//Ĭ�ϵ�ƽ״̬
	//������������
	uint16			rollCount;
	uint8			status;
}MCU_IOOUT_TAB;
void mcu_ioout_init(MCU_IOOUT_TAB *p_table, uint8 tableNum);
uint16 mcu_ioout_get_ready(uint8 channel);
void mcu_ioout_clr_ready(uint8 channel);
void mcu_ioout_set_status(uint8 channel, uint8 status);
uint8 mcu_ioout_get_status(uint8 channel);
void mcu_ioout_toggle(uint8 channel);
void mcu_ioout_set_status_hard(uint8 channel, uint8 status);
uint8 mcu_ioout_get_status_hard(uint8 channel);
void mcu_ioout_toggle_hard(uint8 channel);
void mcu_ioout_main(void);





////////////////////////////////////////////����dio/////////////////////////////////////////////
typedef enum
{
	DI_FT = 0,	//��������
	DI_PU,		//��������
	DI_PD,		//��������
	DO_PP,		//�������
	DO_OD,		//��©���(������)
}DIO_MODE;
typedef struct{
	uint32			portClk;	//clk
	GPIO_TypeDef	*p_gpioPort;//port
	uint16			gpioPin;	//pin
	DIO_MODE		mode;		//0:float,1:ipu,2:ipd,3:pp,4:od;
	uint8			flip;		//0:����ת,1:��ת
	uint8			initStatus;	//Ĭ�ϵ�ƽ״̬
}MCU_DIO_TAB;
void mcu_dio_init(MCU_DIO_TAB *p_table, uint8 tableNum);
void mcu_dio_set_status(uint8 channel, uint8 status);
void mcu_dio_toggle(uint8 channel);
uint8 mcu_dio_get_status(uint8 channel);





////////////////////////////////////////////����adc/////////////////////////////////////////////
typedef struct{
	//uint32			portClk;
	GPIO_TypeDef	*p_gpioPort;
	uint16			gpioPin;
	ADC_TypeDef		*p_adc;
	uint32			adcClk;
	uint8			adcChannel;
	uint16			filterLen;
	uint16			ratioFenzi;
	uint16 			ratioFenmu;
	//��������
	uint16			adcCode;
	uint32			adcCodeSum;
	uint16			filterCnt;
	uint16			rollCount;
	uint32			voltage;
}MCU_ADC_TAB;
void mcu_adc_init(MCU_ADC_TAB *p_table, uint8 tableNum);
uint16 mcu_adc_get_ready(uint8 channel);
void mcu_adc_clr_ready(uint8 channel);
uint32 mcu_adc_get_voltage(uint8 channel);
void mcu_adc_main(void);
void mcu_adc_isr(ADC_TypeDef *p_adc);
void mcu_adc_stop_cycle(void);
void mcu_adc_start_cycle(void);
uint32 mcu_adc_trgget_voltage(uint8 channel);
void mcu_adc_set_mux_callback(void(*pfun_callback)(uint16 *p_userChannel, uint16 codeValue, MCU_ADC_TAB *p_curTable));




//////////////////////////////////////////////����pwmo///////////////////////////////////////////
typedef struct
{
	TIM_TypeDef*		p_tim;
	uint32				timClk;
	uint32				portRemap;	//�Ƿ�ܽ�ӳ��
	uint32				portClk;
	GPIO_TypeDef* 		p_port;
	uint16				pin;
	uint8				flip;			// �Ƿ�ת
	PWM_STATUS			initStatus;
	//������������
	PWM_STATUS			status;
	uint16				periodTime;
	uint16				highTime;
}MCU_PWMO_TAB;
void mcu_pwmo_init(MCU_PWMO_TAB *p_table, uint8 tableNum);
void mcu_pwmo_set_freq_duty(uint8 channel, PWM_STATUS *p_status);
void mcu_pwmo_set_freq(uint8 channel, uint32 freq01);
void mcu_pwmo_set_duty(uint8 channel, uint16 duty01);
void mcu_pwmo_get(uint8 channel, PWM_STATUS *p_status);
PWM_STATUS *mcu_pwmo_get_src(uint8 channel);





//////////////////////////////////////////////����pwmi///////////////////////////////////////////
typedef struct
{
	TIM_TypeDef*		p_tim;
	uint32				timClk;
	uint8				irqChannel;	//�ж�ͨ��
	uint32				portRemap;	//�Ƿ�ܽ�ӳ��
	uint32				portClk;
	GPIO_TypeDef* 		p_port;
	uint16				pin;
	uint8				flip;			//
	int8				calib;			//��λus
	uint16				filterLen;		//�˲�����,���������
	uint16				lostLen;		//�����жϳ�ʱ,���������
	uint8				dutyTolerance;	//ռ�ձ������ݲ�0.1%
	uint16				freqTolerance;	//Ƶ�������ݲ�0.1HZ
	//������������
	PWM_STATUS			status;
	uint16				rollCount;
	volatile uint32		periodTime;
	volatile uint32		highTime;
	volatile uint16		ovtCount[2];
	volatile uint8		ready;
	uint16				filterCnt;
	uint16				lostCnt;
	PWM_STATUS			lastStatus;
}MCU_PWMI_TAB;
void mcu_pwmi_init(MCU_PWMI_TAB *p_table, uint8 tableNum);
uint16 mcu_pwmi_get_ready(uint8 channel);
void mcu_pwmi_clr_ready(uint8 channel);
void mcu_pwmi_get(uint8 channel, PWM_STATUS *p_status);
PWM_STATUS *mcu_pwmi_get_src(uint8 channel);
void mcu_pwmi_main(void);
void mcu_pwmi_isr(uint8 channel);//���ڼĴ�������copy�������н��м���





////////////////////////////////////////////����flash/eep///////////////////////////////////////
#define MCU_FLASH_PB_SIZE	2
#define MCU_FLASH_PP_SIZE	0x800
void mcu_flash_init(void);
uint8 mcu_flash_get_data(uint32 addr, void *p_data, uint32 len);
uint8 mcu_flash_set_data(uint32 addr, void *p_data, uint32 len);
uint8 mcu_flash_erase(uint32 addr);
void mcu_flash_erase_set_calback(void (*pfun_calBack)(void));





//////////////////////////////////////////////����can///////////////////////////////////////////
typedef struct
{
	CAN_TypeDef*				p_can;		//can
	uint8						irqChannel;	//�ж�
	uint32						canClk;		//clk
	uint32						portRemap;	//remap
	CAN_InitTypeDef 			initStruct;
	CAN_FilterInitTypeDef		filterStruct;
	uint32						txPortClk;
	GPIO_TypeDef* 				p_txPort;
	uint16						txPin;
	uint32						rxPortClk;
	GPIO_TypeDef* 				p_rxPort;
	uint16						rxPin;
	void (*pfun_rxCallBack)(CAN_MSG*);		//���ջص�
}MCU_CAN_TAB;
void mcu_can_init(MCU_CAN_TAB *p_table, uint8 tableNum);
void mcu_can_reinit(uint8 channel);
uint8 mcu_can_send(uint8 channel, CAN_MSG* p_send);
void mcu_can_rx_isr(uint8 channel, CAN_MSG* p_recv);//���������ص�





//////////////////////////////////////////////����mspi///////////////////////////////////////////
typedef struct{
	SPI_TypeDef*			p_mspi;
	uint32					mspiClk;
	uint32					portRemap;
	uint8					csDisable;	//CS ��Ч״̬
	//uint8					manulCs;	//1:�ֶ�cs,0:�Զ�cs
	SPI_InitTypeDef 		initStruct;
	uint32					csPortClk;
	GPIO_TypeDef* 			p_csPort;
	uint16					csPin;
	uint32					sckPortClk;
	GPIO_TypeDef* 			p_sckPort;
	uint16					sckPin;
	uint32					misoPortClk;
	GPIO_TypeDef* 			p_misoPort;
	uint16					misoPin;
	uint32					mosiPortClk;
	GPIO_TypeDef* 			p_mosiPort;
	uint16					mosiPin;
}MCU_MSPI_TAB;
void mcu_mspi_init(MCU_MSPI_TAB *p_table, uint8 tableNum);
uint8 mcu_mspi_txrx(uint8 channel, uint8 send);
void mcu_mspi_txrx_continue(uint8 channel, void *p_tx, void *p_rx, uint16 dataLen);
void mcu_mspi_cs_enable(uint8 channel);
void mcu_mspi_cs_disable(uint8 channel);





//////////////////////////////////////////////����sspi///////////////////////////////////////////
typedef struct{
	SPI_TypeDef*	p_sspi;
	uint32			mspiClk;
	uint32			portRemap;
	SPI_InitTypeDef initStruct;
	uint32			csPortClk;
	GPIO_TypeDef* 	p_csPort;
	uint16			csPin;
	uint32			sckPortClk;
	GPIO_TypeDef* 	p_sckPort;
	uint16			sckPin;
	uint32			mosiPortClk;
	GPIO_TypeDef* 	p_mosiPort;
	uint16			mosiPin;
	uint32			misoPortClk;
	GPIO_TypeDef* 	p_misoPort;
	uint16			misoPin;
	void*			p_rxBuff;
	uint16			rxBuffLen;
	void*			p_txBuff;
	uint16			txBuffLen;
	//������������
	volatile uint16	txRxIndex;
}MCU_SSPI_TAB;
void mcu_sspi_init(MCU_SSPI_TAB *p_table, uint8 tableNum);
void mcu_sspi_start(uint8 channel, void *p_send, uint16 sendLen);
void mcu_sspi_start_direct(uint8 channel, void *p_send);
uint16 mcu_sspi_check_recv(uint8 channel);
void mcu_sspi_get_recv(uint8 channel, void *p_recv, uint16 recvLen);
void mcu_sspi_stop(uint8 channel);
void mcu_sspi_tx_isr(uint8 channel, void *p_send);//�жϵ���,д���ͼĴ���
void mcu_sspi_rx_isr(uint8 channel, void *p_recv);//�жϵ���,���Ĵ�����������





//////////////////////////////////////////////����uart///////////////////////////////////////////
typedef struct
{
	USART_TypeDef*		p_uart;		//��������
	uint8				irqChannel;	//�ж�ͨ��
	uint32				uartClk;	//����ʱ��Դ
	uint32				portRemap;	//�Ƿ�ܽ�ӳ��
	USART_InitTypeDef 	initStruct;	//��������
	uint32				txPortClk;	//���͹ܽ�ʱ��
	GPIO_TypeDef* 		p_txPort;	//���͹ܽ�����
	uint16				txPin;		//���͹ܽź�
	uint32				rxPortClk;	//���չܽ�ʱ��
	GPIO_TypeDef* 		p_rxPort;	//���չܽ�����
	uint16				rxPin;		//���չܽź�
	void (*pfun_rxCallBack)(uint8);	//���ջص�
}MCU_UART_TAB;
void mcu_uart_init(MCU_UART_TAB *p_table, uint8 tableNum);
uint8 mcu_uart_tx(uint8 channel, void *p_tx, uint16 len);
void mcu_uart_rx_isr(uint8 channel, uint8 *p_recv);//���������ص�





#endif //__MCU_HAL_H_
