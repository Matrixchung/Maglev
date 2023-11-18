/*********************************************************************************
	*Copyright(C),20xx-20xx,

	*Filename:uart_tpm.c

	*Author:
			wwyyy
			q1046685883

	*Version:
			V1.0

	*Data:
			2020/05/13

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
#include "uart_tpm.h"
#include "string.h"
#include "public.h"
#include "timer.h"

static UART_TPM_CONFIG uartTpmConfig = {0};


/***********************************************************************************************
*函数名: static uint16 uart_read_isr(uint8 channel, uint8 *p_buff, uint16 maxLen)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
static uint16 uart_read_isr(uint8 channel, uint8 *p_buff, uint16 maxLen)
{
	uint16 i = 0, wrIndex = 0;
	for(wrIndex = uartTpmConfig.p_aspConfig[channel].rxWrIndex;
		(wrIndex!=uartTpmConfig.p_aspConfig[channel].rxRdIndex)&&(maxLen>0); \
		uartTpmConfig.p_aspConfig[channel].rxRdIndex =\
		(uartTpmConfig.p_aspConfig[channel].rxRdIndex+1)%uartTpmConfig.p_aspConfig[channel].isrRxLen, 
		i++,maxLen--)
	{
		p_buff[i] = uartTpmConfig.p_aspConfig[channel].p_isrRx[uartTpmConfig.p_aspConfig[channel].rxRdIndex];
	}
	return i;
}

/***********************************************************************************************
*函数名: void uart_tp_init(UART_TP_CONFIG *p_Config)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void uart_tpm_init(UART_TPM_CONFIG *p_Config)
{
	uint8 i = 0;
	memcpy(&uartTpmConfig, p_Config, sizeof(uartTpmConfig));
	for(i = 0; i < uartTpmConfig.aspConfigNum; i++)
	{
		if(NULL != uartTpmConfig.p_aspConfig)
		{
			uartTpmConfig.p_aspConfig[i].rxWrIndex = 0;
			uartTpmConfig.p_aspConfig[i].rxRdIndex = 0;
			uartTpmConfig.p_aspConfig[i].errStatus = 0;
		}
	}
	if(NULL != uartTpmConfig.pfun_mcuUartInit)
	{
		uartTpmConfig.pfun_mcuUartInit();
	}
}

/***********************************************************************************************
*函数名: void uart_tpm_main(void)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
void uart_tpm_main(void)
{
	uint16 rxLen = 0;
	uint16 crc16 = 0;
	uint8 i = 0;
	for(i = 0; i < uartTpmConfig.aspConfigNum; i++)
	{
		if(tim_check_timenow(uartTpmConfig.p_aspConfig[i].lastRxTimestamp, uartTpmConfig.framTimeInvt))
		{
			rxLen = uart_read_isr(i, uartTpmConfig.p_buff, uartTpmConfig.buffLen);
			if(rxLen > 2)
			{
				crc16 = uartTpmConfig.p_buff[rxLen-1];
				crc16 <<= 8;
				crc16 |= uartTpmConfig.p_buff[rxLen-2];
				if(crc16 == crc16_calculate_modbus(uartTpmConfig.p_buff, rxLen-2))
				{
					if(NULL != uartTpmConfig.p_aspConfig[i].pfun_recvCallback)
					{
						uartTpmConfig.p_aspConfig[i].pfun_recvCallback(uartTpmConfig.p_buff, rxLen-2);
					}
				}
			}
			uartTpmConfig.p_aspConfig[i].lastRxTimestamp = tim_get_count();
		}
	}
}

/***********************************************************************************************
*函数名: uint8 uart_tpm_rx_isr(uint8 channel, uint8 data)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 uart_tpm_rx_isr(uint8 channel, uint8 data)
{
	if(channel >= uartTpmConfig.aspConfigNum)
	{
		return 1;
	}
	if(uartTpmConfig.p_aspConfig[channel].rxRdIndex == ((uartTpmConfig.p_aspConfig[channel].rxWrIndex + 1) %\
		uartTpmConfig.p_aspConfig[channel].isrRxLen))//队列已满
	{
		uartTpmConfig.p_aspConfig[channel].errStatus |= UART_TPM_ERR_RXFLOW;
		return 1;
	}
	else
	{
		uartTpmConfig.p_aspConfig[channel].p_isrRx[uartTpmConfig.p_aspConfig[channel].rxWrIndex] = data;
		uartTpmConfig.p_aspConfig[channel].rxWrIndex = (uartTpmConfig.p_aspConfig[channel].rxWrIndex+1)%
		uartTpmConfig.p_aspConfig[channel].isrRxLen;
	}
	uartTpmConfig.p_aspConfig[channel].lastRxTimestamp = tim_get_count();
	return 0;
}

/***********************************************************************************************
*函数名: uint8 uart_tpm_tx_data(uint8 channel, uint8 *p_data, uint16 len)
*功能描述:
*特别说明:
*函数参数:
*函数返回值:
*修改记录:
***********************************************************************************************/
uint8 uart_tpm_tx_data(uint8 channel, uint8 *p_data, uint16 len)
{
	uint16 crc16 = 0;
	if(channel >= uartTpmConfig.aspConfigNum ||
		NULL == p_data || 0 == len ||
		len + 2 > uartTpmConfig.buffLen)
	{
		return 1;
	}
	memcpy(uartTpmConfig.p_buff, p_data, len);
	crc16 = crc16_calculate_modbus(uartTpmConfig.p_buff, len);
	uartTpmConfig.p_buff[len] = crc16;		//crc16l
	uartTpmConfig.p_buff[len+1] = crc16>>8;		//crc16h
	if(NULL != uartTpmConfig.pfun_mcuUartTx)
	{
		return uartTpmConfig.pfun_mcuUartTx(uartTpmConfig.p_aspConfig[channel].uartId, uartTpmConfig.p_buff, len+2);
	}
	return 1;
}
