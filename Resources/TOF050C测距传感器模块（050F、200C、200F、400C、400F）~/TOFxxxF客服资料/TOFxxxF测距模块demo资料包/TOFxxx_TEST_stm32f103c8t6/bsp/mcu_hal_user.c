#include "mcu_hal_user.h"
#include <stdio.h>

//ÅäÖÃioin
MCU_IOIN_TAB a_mcuIoInTale[MCU_IOIN_NUM] = 
{
	{RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_0, IN_PD, NFLP, 10},
	{RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_1, IN_PU, FLIP, 10},
	{RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_2, IN_PD, NFLP, 10},
};
void mcu_ioin_user_init(void)
{
	mcu_ioin_init(a_mcuIoInTale, MCU_IOIN_NUM);
}




//ÅäÖÃioout
MCU_IOOUT_TAB a_mcuIoOutTale[MCU_IOOUT_NUM] = 
{
	{RCC_APB2Periph_GPIOD, GPIOD, GPIO_Pin_12, OUT_PP, NFLP, LOW},
	{RCC_APB2Periph_GPIOD, GPIOD, GPIO_Pin_13, OUT_PP, FLIP, LOW},
};
void mcu_ioout_user_init(void)
{
	mcu_ioout_init(a_mcuIoOutTale, MCU_IOOUT_NUM);
}





//ÅäÖÃdio
MCU_DIO_TAB a_mcuDioTable[MCU_DIO_NUM] = 
{
	{RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_5, DI_PU, NFLP, LOW},
	{RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_6, DO_PP, FLIP, LOW},
	{RCC_APB2Periph_GPIOE, GPIOE, GPIO_Pin_2, DO_PP, NFLP, LOW},
};
void mcu_dio_user_init(void)
{
	mcu_dio_init(a_mcuDioTable, MCU_DIO_NUM);
}




//ÅäÖÃadc
MCU_ADC_TAB a_mcuAdInTable[MCU_ADIN_NUM] = 
{
	{GPIOA, GPIO_Pin_0, ADC1, ADC_Channel_0, 10, 1, 1},
	{GPIOA, GPIO_Pin_1, ADC1, ADC_Channel_1, 10, 2, 1},
	{GPIOA, GPIO_Pin_2, ADC1, ADC_Channel_2, 10, 1, 2},
};
void mcu_adc_user_init(void)
{
	mcu_adc_init(a_mcuAdInTable, MCU_ADIN_NUM);
}





//ÅäÖÃcan
MCU_CAN_TAB a_mcuCanTable[MCU_CAN_NUM] = 
{
	{//36/6/12=500K
		CAN1, CAN1_RX1_IRQn, RCC_APB1Periph_CAN1, 0,
		{6,CAN_Mode_Normal,CAN_SJW_1tq,CAN_BS1_5tq,CAN_BS2_6tq,DISABLE,DISABLE,ENABLE,DISABLE,ENABLE,ENABLE},
		{0,0,0,0,CAN_FIFO1,0,CAN_FilterMode_IdMask,CAN_FilterScale_32bit,ENABLE},
		RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_12,
		RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_11,
		NULL
	}
};
void mcu_can_user_init(void)
{
	mcu_can_init(a_mcuCanTable, MCU_CAN_NUM);
}





//ÅäÖÃmspi
MCU_MSPI_TAB a_mcuMspiTable[MCU_MSPI_NUM] = 
{
	{
		SPI1, RCC_APB2Periph_SPI1, 0, 1,\
		{SPI_Direction_2Lines_FullDuplex, SPI_Mode_Master, SPI_DataSize_8b, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_NSS_Soft, SPI_BaudRatePrescaler_4, SPI_FirstBit_MSB, 7},\
		RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_4,\
		RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_5,\
		RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_6,\
		RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_7,\
	},
	{
		SPI2, RCC_APB1Periph_SPI2, 0, 0,\
		{SPI_Direction_2Lines_FullDuplex, SPI_Mode_Master, SPI_DataSize_8b, SPI_CPOL_Low, SPI_CPHA_2Edge, SPI_NSS_Soft, SPI_BaudRatePrescaler_4, SPI_FirstBit_LSB, 7},\
		RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_12,\
		RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_13,\
		RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_14,\
		RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_15,\
	},
};
void mcu_mspi_user_init(void)
{
	mcu_mspi_init(a_mcuMspiTable, MCU_MSPI_NUM);
}


int fputc(int ch, FILE *f)
{
	char str = ch;
	mcu_uart_tx(MCU_UART_TWO, &str, 1);
	return ch;
}

//ÅäÖÃuart
MCU_UART_TAB a_mcuUartTable[MCU_UART_NUM] = 
{
	{
		USART1, USART1_IRQn, RCC_APB2Periph_USART1, 0,\
		115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_Rx|USART_Mode_Tx, USART_HardwareFlowControl_None,\
		RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_9,
		RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_10,
		NULL
	},
	{
		USART2, USART2_IRQn, RCC_APB1Periph_USART2, 0,\
		115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_Rx|USART_Mode_Tx, USART_HardwareFlowControl_None,\
		RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_2,
		RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_3,
		NULL
	},
};
void mcu_uart_user_init(void)
{
	mcu_uart_init(a_mcuUartTable, MCU_UART_NUM);
}







void mcu_user_init(void)
{
	mcu_ioin_user_init();
	mcu_ioout_user_init();
	mcu_dio_user_init();
	mcu_adc_user_init();
	//mcu_pwmo_user_init();
	//mcu_pwmi_user_init();
	mcu_mspi_user_init();
	//mcu_sspi_user_init();
}
void mcu_user_main(void)
{
	mcu_ioin_main();
	mcu_ioout_main();
	mcu_adc_main();
	//mcu_pwmi_main();
}


