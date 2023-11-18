#include "vofa.h"
/*
 * Use JustFloat data engine
 * 
 */
float channel_data[CHANNEL_COUNT] = {0.0f};
const char _tail[4] = {0x00, 0x00, 0x80, 0x7f};
// osMutexId vofa_mutex;
// void vofa_init(void)
// {
//     osMutexDef(_vofa_mutex);
//     vofa_mutex = osMutexCreate(osMutex(_vofa_mutex));
// }
/*
 * @retval: 0: success 1: fail
 */
uint8_t vofa_add_float(uint8_t channel, float data)
{
    if(channel >= CHANNEL_COUNT) return 1;
    // osMutexWait(vofa_mutex, osWaitForever);
    channel_data[channel] = data;
    // osMutexRelease(vofa_mutex);
    return 0;
}

uint8_t vofa_send_data(void)
{
    UART_PutTxFifo(&UART1_Handler, (char *)channel_data, sizeof(float)*CHANNEL_COUNT);
    UART_PutTxFifo(&UART1_Handler, (char *)_tail, 4);
    UART_TransmitFromFifo(&UART1_Handler);
    // return UART_Transmit(TX_BLOCK);
    return 0;
}