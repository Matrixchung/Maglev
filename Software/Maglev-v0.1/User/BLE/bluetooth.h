#ifndef _BLUETOOTH_H
#define _BLUETOOTH_H

#include "main.h"
#include "uart_handler.h"
#include "delay.h"

extern uint8_t ble_state;

void BLE_Init(void);
void BLE_RX_Callback(void);
void BLE_EXTI_IRQHandler(void);
#endif