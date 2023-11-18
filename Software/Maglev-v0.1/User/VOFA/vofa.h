#ifndef _VOFA_H
#define _VOFA_H

#include "main.h"
// #include "user_task.h"
#include "uart_handler.h"

#define CHANNEL_COUNT 37
// void vofa_init(void);
uint8_t vofa_add_float(uint8_t channel, float data);
uint8_t vofa_send_data(void);
#endif