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

#define UART_TPM_ERR_RXFLOW		0x0001//�����ж����
#define UART_TPM_ERR_UNPACKCX	0x0100//��λ����(���յ�β��Ϊ��ʼ)
#define UART_TPM_ERR_UNPACKCDYC	0x0200//���ݳ����쳣
#define UART_TPM_ERR_UNPACKCHK	0x0400//У��ʧ��
#define UART_TPM_ERR_UNPACKZY	0x0800//ת�����

#define UART_TPM_ERR_UNPACKFLOW	0x0002//�����������
#define UART_TPM_ERR_PACKFLOW	0x0004//����������


#pragma pack(1)
typedef struct
{
	uint8		uartId;//����id
	uint8		*p_isrRx;//���ջ���
	uint16		isrRxLen;//���ջ��泤��
	void		(*pfun_recvCallback)(uint8 *, uint16);
	//������������
	uint32		lastRxTimestamp;
	uint16		rxWrIndex;
	uint16		rxRdIndex;
	uint32		errStatus;
}UART_TPM_ASP_CONFIG;
#pragma pack()

#pragma pack(1)
typedef struct
{
	uint8		*p_buff;					//���ڷ��ʹ�����ж�˫����
	uint16		buffLen;
	uint8		framTimeInvt;				//֡���ms
	void		(*pfun_mcuUartInit)(void);//��ѡ
	uint8		(*pfun_mcuUartTx)(uint8, void *, uint16);//����
	UART_TPM_ASP_CONFIG *p_aspConfig;
	uint8		aspConfigNum;
}UART_TPM_CONFIG;
#pragma pack()

void uart_tpm_init(UART_TPM_CONFIG *p_Config);
void uart_tpm_main(void);
uint8 uart_tpm_tx_data(uint8 channel, uint8 *p_data, uint16 len);
uint8 uart_tpm_rx_isr(uint8 channel, uint8 data);

#endif//__UART_TPM_H_
