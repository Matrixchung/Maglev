/*********************************************************************************
	*Copyright(C),20xx-20xx,

	*Author:
			wwyyy
			q1046685883

	*Version:
			V1.0

	*Data:
			2019/11/16

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
#ifndef __UART_TPM_H_
#define __UART_TPM_H_
#include "type.h"

#define UART_TPM_ERR_RXFLOW		0x0001//接收中断溢出
#define UART_TPM_ERR_UNPACKCX	0x0100//首位差续(接收到尾作为开始)
#define UART_TPM_ERR_UNPACKCDYC	0x0200//数据长度异常
#define UART_TPM_ERR_UNPACKCHK	0x0400//校验失败
#define UART_TPM_ERR_UNPACKZY	0x0800//转义错误

#define UART_TPM_ERR_UNPACKFLOW	0x0002//解析缓存溢出
#define UART_TPM_ERR_PACKFLOW	0x0004//打包缓存溢出


#pragma pack(1)
typedef struct
{
	uint8		uartId;//串口id
	uint8		*p_isrRx;//接收缓存
	uint16		isrRxLen;//接收缓存长度
	void		(*pfun_recvCallback)(uint8 *, uint16);
	//以下无需配置
	uint32		lastRxTimestamp;
	uint16		rxWrIndex;
	uint16		rxRdIndex;
	uint32		errStatus;
}UART_TPM_ASP_CONFIG;
#pragma pack()

#pragma pack(1)
typedef struct
{
	uint8		*p_buff;					//用于发送打包和中断双缓存
	uint16		buffLen;
	uint8		framTimeInvt;				//帧间隔ms
	void		(*pfun_mcuUartInit)(void);//可选
	uint8		(*pfun_mcuUartTx)(uint8, void *, uint16);//必须
	UART_TPM_ASP_CONFIG *p_aspConfig;
	uint8		aspConfigNum;
}UART_TPM_CONFIG;
#pragma pack()

void uart_tpm_init(UART_TPM_CONFIG *p_Config);
void uart_tpm_main(void);
uint8 uart_tpm_tx_data(uint8 channel, uint8 *p_data, uint16 len);
uint8 uart_tpm_rx_isr(uint8 channel, uint8 data);

#endif//__UART_TPM_H_
