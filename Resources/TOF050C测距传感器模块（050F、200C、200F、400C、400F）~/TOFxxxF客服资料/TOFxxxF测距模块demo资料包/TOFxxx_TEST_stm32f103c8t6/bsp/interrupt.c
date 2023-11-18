#include "interrupt.h"
#include "main.h"


void NMI_Handler(void)
{
}
void HardFault_Handler(void)
{
	while (1)
	{
	}
}
void MemManage_Handler(void)
{
	while (1)
	{
	}
}
void BusFault_Handler(void)
{
	while (1)
	{
	}
}
void UsageFault_Handler(void)
{
	while (1)
	{
	}
}
void SVC_Handler(void)
{
}
void DebugMon_Handler(void)
{
}
void PendSV_Handler(void)
{
}
void SysTick_Handler(void)
{
	tim_isr();
}


void ADC1_2_IRQHandler(void)
{
	mcu_adc_isr(ADC1);
}

void DMA1_Channel1_IRQHandler()
{
//	if(SET == DMA_GetITStatus(DMA1_IT_TC1))
//	{
//		DMA_ClearITPendingBit(DMA1_IT_TC1);
//	}
}


void USART1_IRQHandler(void)
{
	uint8 data;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		data = USART_ReceiveData(USART1);
		uart_tpm_rx_isr(MCU_UART_ONE, data);
	}
}
void USART2_IRQHandler(void)
{
//	uint8 data;
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//	{
//		data = USART_ReceiveData(USART2);
//		uart_tp_rx_data(MCU_UART_TWO, data);
//	}
}
void UART4_IRQHandler(void)
{
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{ 
		USART_ReceiveData(UART4);
	}
}


void TIM6_IRQHandler(void)
{
//	if(TIM_GetITStatus(TIM6, TIM_IT_Update ) != RESET)
//	{
//		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
//		tim_isr();
//	}
}


static CanRxMsg RxMessage;
static CAN_MSG canMsg;
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	if(RxMessage.IDE == CAN_Id_Extended)
	{
		canMsg.ide = CAN_IDE_EXTENDED;
		canMsg.id = RxMessage.ExtId;
	}
	else
	{
		canMsg.ide = CAN_IDE_STANDARD;
		canMsg.id = RxMessage.StdId;
	}
	canMsg.dlc = RxMessage.DLC;
	if(RxMessage.RTR == CAN_FRAM_RTR_DATA)
	{
		canMsg.rtr = CAN_FRAM_RTR_DATA;
	}
	else
	{
		canMsg.rtr = CAN_FRAM_RTR_REMO;
	}
	memcpy(canMsg.a_data, RxMessage.Data, 8);
	//can_recv_isr(MCU_CAN_ONE, &canMsg);
}

void CAN1_RX1_IRQHandler(void)
{
	CAN_Receive(CAN1, CAN_FIFO1, &RxMessage);
	if(RxMessage.IDE == CAN_Id_Extended)
	{
		canMsg.ide = CAN_IDE_EXTENDED;
		canMsg.id = RxMessage.ExtId;
	}
	else
	{
		canMsg.ide = CAN_IDE_STANDARD;
		canMsg.id = RxMessage.StdId;
	}
	canMsg.dlc = RxMessage.DLC;
	if(RxMessage.RTR == CAN_FRAM_RTR_DATA)
	{
		canMsg.rtr = CAN_FRAM_RTR_DATA;
	}
	else
	{
		canMsg.rtr = CAN_FRAM_RTR_REMO;
	}
	memcpy(canMsg.a_data, RxMessage.Data, 8);
//	can_recv_isr(MCU_CAN_ONE, &canMsg);
}

void CAN1_SCE_IRQHandler(void)
{
	if(RESET != CAN_GetITStatus(CAN1, CAN_IT_BOF))
	{
		//can_busoff_isr(MCU_CAN_ONE);
		CAN_ClearITPendingBit(CAN1, CAN_IT_BOF);
	}
}

void PPP_IRQHandler(void)
{
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

