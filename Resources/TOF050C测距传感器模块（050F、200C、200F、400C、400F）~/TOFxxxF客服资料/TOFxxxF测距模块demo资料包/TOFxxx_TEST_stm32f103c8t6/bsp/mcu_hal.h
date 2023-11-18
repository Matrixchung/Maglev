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





/////////////////////////////////////////////配置core///////////////////////////////////////////////
void mcu_init(void);
void mcu_disable_irq(void);
void mcu_enable_irq(void);
void mcu_kick_dog(void);
void mcu_soft_reset(void);





//////////////////////////////////////////////配置tim//////////////////////////////////////////////
void mcu_tim_init(uint16 intvTime);
void mcu_tim_init_call(uint16 intvTime, void(*pfun_callBack)(void));
void mcu_tim_delayus(uint16 delayCount);
void mcu_tim_isr(void);//用于驱动注册回调




//////////////////////////////////////////////配置ioin/////////////////////////////////////////////
typedef enum
{
	IN_FT = 0,	//浮空
	IN_PU,		//上拉
	IN_PD,		//下拉
}IOIN_MODE;
typedef struct{
	uint32			portClk;	//clk
	GPIO_TypeDef	*p_gpioPort;//port
	uint16			gpioPin;	//pin
	IOIN_MODE		mode;		//0:float,1:ipu,2:ipd
	uint8			flip;		//0:不翻转,1:翻转
	uint16			filterLen;	//滤波长度
	//以下无需配置
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
	OUT_PP = 0,	//推挽
	OUT_OD,		//开漏(带上拉)
}IOOUT_MODE;
typedef struct{
	uint32			portClk;	//clk
	GPIO_TypeDef	*p_gpioPort;//port
	uint16			gpioPin;	//pin
	IOOUT_MODE		mode;		//:0:pp,1:od
	uint8			flip;		//0:不翻转,1:翻转
	uint8			initStatus;	//默认电平状态
	//以下无需配置
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





////////////////////////////////////////////配置dio/////////////////////////////////////////////
typedef enum
{
	DI_FT = 0,	//浮空输入
	DI_PU,		//上拉输入
	DI_PD,		//下拉输入
	DO_PP,		//推挽输出
	DO_OD,		//开漏输出(带上拉)
}DIO_MODE;
typedef struct{
	uint32			portClk;	//clk
	GPIO_TypeDef	*p_gpioPort;//port
	uint16			gpioPin;	//pin
	DIO_MODE		mode;		//0:float,1:ipu,2:ipd,3:pp,4:od;
	uint8			flip;		//0:不翻转,1:翻转
	uint8			initStatus;	//默认电平状态
}MCU_DIO_TAB;
void mcu_dio_init(MCU_DIO_TAB *p_table, uint8 tableNum);
void mcu_dio_set_status(uint8 channel, uint8 status);
void mcu_dio_toggle(uint8 channel);
uint8 mcu_dio_get_status(uint8 channel);





////////////////////////////////////////////配置adc/////////////////////////////////////////////
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
	//无需配置
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




//////////////////////////////////////////////配置pwmo///////////////////////////////////////////
typedef struct
{
	TIM_TypeDef*		p_tim;
	uint32				timClk;
	uint32				portRemap;	//是否管脚映射
	uint32				portClk;
	GPIO_TypeDef* 		p_port;
	uint16				pin;
	uint8				flip;			// 是否反转
	PWM_STATUS			initStatus;
	//以下无需配置
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





//////////////////////////////////////////////配置pwmi///////////////////////////////////////////
typedef struct
{
	TIM_TypeDef*		p_tim;
	uint32				timClk;
	uint8				irqChannel;	//中断通道
	uint32				portRemap;	//是否管脚映射
	uint32				portClk;
	GPIO_TypeDef* 		p_port;
	uint16				pin;
	uint8				flip;			//
	int8				calib;			//单位us
	uint16				filterLen;		//滤波长度,与周期相关
	uint16				lostLen;		//用于判断超时,与周期相关
	uint8				dutyTolerance;	//占空比允许容差0.1%
	uint16				freqTolerance;	//频率允许容差0.1HZ
	//以下无需配置
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
void mcu_pwmi_isr(uint8 channel);//用于寄存器数据copy到缓存中进行计算





////////////////////////////////////////////配置flash/eep///////////////////////////////////////
#define MCU_FLASH_PB_SIZE	2
#define MCU_FLASH_PP_SIZE	0x800
void mcu_flash_init(void);
uint8 mcu_flash_get_data(uint32 addr, void *p_data, uint32 len);
uint8 mcu_flash_set_data(uint32 addr, void *p_data, uint32 len);
uint8 mcu_flash_erase(uint32 addr);
void mcu_flash_erase_set_calback(void (*pfun_calBack)(void));





//////////////////////////////////////////////配置can///////////////////////////////////////////
typedef struct
{
	CAN_TypeDef*				p_can;		//can
	uint8						irqChannel;	//中断
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
	void (*pfun_rxCallBack)(CAN_MSG*);		//接收回调
}MCU_CAN_TAB;
void mcu_can_init(MCU_CAN_TAB *p_table, uint8 tableNum);
void mcu_can_reinit(uint8 channel);
uint8 mcu_can_send(uint8 channel, CAN_MSG* p_send);
void mcu_can_rx_isr(uint8 channel, CAN_MSG* p_recv);//用于驱动回调





//////////////////////////////////////////////配置mspi///////////////////////////////////////////
typedef struct{
	SPI_TypeDef*			p_mspi;
	uint32					mspiClk;
	uint32					portRemap;
	uint8					csDisable;	//CS 无效状态
	//uint8					manulCs;	//1:手动cs,0:自动cs
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





//////////////////////////////////////////////配置sspi///////////////////////////////////////////
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
	//以下无需配置
	volatile uint16	txRxIndex;
}MCU_SSPI_TAB;
void mcu_sspi_init(MCU_SSPI_TAB *p_table, uint8 tableNum);
void mcu_sspi_start(uint8 channel, void *p_send, uint16 sendLen);
void mcu_sspi_start_direct(uint8 channel, void *p_send);
uint16 mcu_sspi_check_recv(uint8 channel);
void mcu_sspi_get_recv(uint8 channel, void *p_recv, uint16 recvLen);
void mcu_sspi_stop(uint8 channel);
void mcu_sspi_tx_isr(uint8 channel, void *p_send);//中断调用,写发送寄存器
void mcu_sspi_rx_isr(uint8 channel, void *p_recv);//中断调用,读寄存器到缓存中





//////////////////////////////////////////////配置uart///////////////////////////////////////////
typedef struct
{
	USART_TypeDef*		p_uart;		//串口主体
	uint8				irqChannel;	//中断通道
	uint32				uartClk;	//串口时钟源
	uint32				portRemap;	//是否管脚映射
	USART_InitTypeDef 	initStruct;	//参数配置
	uint32				txPortClk;	//发送管脚时钟
	GPIO_TypeDef* 		p_txPort;	//发送管脚主体
	uint16				txPin;		//发送管脚号
	uint32				rxPortClk;	//接收管脚时钟
	GPIO_TypeDef* 		p_rxPort;	//接收管脚主体
	uint16				rxPin;		//接收管脚号
	void (*pfun_rxCallBack)(uint8);	//接收回调
}MCU_UART_TAB;
void mcu_uart_init(MCU_UART_TAB *p_table, uint8 tableNum);
uint8 mcu_uart_tx(uint8 channel, void *p_tx, uint16 len);
void mcu_uart_rx_isr(uint8 channel, uint8 *p_recv);//用于驱动回调





#endif //__MCU_HAL_H_
