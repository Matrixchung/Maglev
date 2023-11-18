#ifndef __UART_TPM_USER_H_
#define __UART_TPM_USER_H_
#include "uart_tpm.h"

typedef enum
{
	UART_TPM_ONE = 0,
	UART_TPM_NUM,
}UART_TPM_ENUM;

void uart_tpm_user_init(void);

#endif//__UART_TPM_USER_H_
