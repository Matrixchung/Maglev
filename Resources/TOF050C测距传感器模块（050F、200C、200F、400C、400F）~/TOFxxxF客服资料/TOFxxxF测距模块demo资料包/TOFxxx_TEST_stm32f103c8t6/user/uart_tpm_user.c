#include "uart_tpm_user.h"
#include "mcu_hal_user.h"

void modbus_recv_data(uint8 *p_data, uint16 len);
static uint8 a_rxIsrBuff[25];
UART_TPM_ASP_CONFIG a_uartTpmAspConfig[] = 
{
	{MCU_UART_ONE, a_rxIsrBuff, sizeof(a_rxIsrBuff), modbus_recv_data},
};
void uart_tpm_user_init(void)
{
	UART_TPM_CONFIG uartTpmConfig;
	static uint8 a_buff[30];//发送处理和接收双缓存
	uartTpmConfig.p_buff = a_buff;
	uartTpmConfig.buffLen = sizeof(a_buff);
	uartTpmConfig.framTimeInvt = 3;
	uartTpmConfig.pfun_mcuUartInit = mcu_uart_user_init;
	uartTpmConfig.pfun_mcuUartTx = mcu_uart_tx;
	uartTpmConfig.p_aspConfig = a_uartTpmAspConfig;
	uartTpmConfig.aspConfigNum = sizeof(a_uartTpmAspConfig)/sizeof(a_uartTpmAspConfig[0]);
	uart_tpm_init(&uartTpmConfig);
}

