#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"	
#include "sys.h" 


void USART2_Init(u32 bound);
void USART2_Send(unsigned char *data,uint8_t len);
uint16_t ModbusCRC(uint8_t *ptr,uint16_t len);


#endif


