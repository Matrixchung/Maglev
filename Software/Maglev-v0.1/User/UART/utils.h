#ifndef _UART_UTILS_H
#define _UART_UTILS_H

#include "main.h"
#include "math.h"

#define MAX_COMMAND_WORD_LENGTH 16
#define MAX_FLOAT_ARG_COUNT     16
void getCommand(char* pSource, uint16_t Lsource, char* pDest);
void splitData_f(char* pSource, uint16_t Lsource, float* pDest, uint8_t* Ldest, uint8_t split);
uint8_t trimData(char* pSource);
uint16_t getCRC16(uint8_t *data, uint16_t len);

#endif