#ifndef USART_DEVICE_H
#define USART_DEVICE_H
#include "stm32f10x_usart.h"
#include "stdio.h"
#define UsartBaudRate                 115200
#define UsartPreemptionPriority            2
#define UsartSubPriority                   2

void Usart1_Config(void);
void Usart1_SendData(void);
int SendManager(void);
void Usart1_ReceivedManager(void);
#endif
