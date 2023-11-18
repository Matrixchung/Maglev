#include "bluetooth.h"
uint8_t ble_state = 0;
static uint16_t ble_ret_len = 0;
static char ble_buffer[128] = {0};
void BLE_Init(void)
{
    UART_InstanceInit(&UART_BLE_Handler, USART2, DMA1, LL_DMA_STREAM_5, DMA1, LL_DMA_STREAM_6, 256);
    // UART_RegisterRxCallback(&UART_BLE_Handler, BLE_RX_Callback);
    if(!LL_GPIO_IsInputPinSet(BLE_STATE_GPIO_Port, BLE_STATE_Pin))
    {
        delay_ms(10);
        if(!LL_GPIO_IsInputPinSet(BLE_STATE_GPIO_Port, BLE_STATE_Pin)) ble_state = 1;
    }
}

void BLE_RX_Callback(void)
{
    // ble_ret_len = UART_GetRxLen(&UART_BLE_Handler);
    // if(ble_ret_len > 0)
    // {
    //     UART_GetRxFifo(&UART_BLE_Handler, ble_buffer, ble_ret_len);
    //     UART_TransmitFromBuffer(&UART_BLE_Handler, ble_buffer, ble_ret_len);
    //     // if(ble_buffer[ble_ret_len-1] == '\r')
    //     // {
    //     //     UART_TransmitFromBuffer(&UART_BLE_Handler, ble_buffer, ble_ret_len);
    //     // }
    // }
}

void BLE_EXTI_IRQHandler(void)
{
    ble_state = !LL_GPIO_IsInputPinSet(BLE_STATE_GPIO_Port, BLE_STATE_Pin);
    // if(ble_state) LL_GPIO_SetOutputPin(LED_1_GPIO_Port, LED_1_Pin);
    // else LL_GPIO_ResetOutputPin(LED_1_GPIO_Port, LED_1_Pin);
}