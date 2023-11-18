#ifndef _UART_HANDLER_H
#define _UART_HANDLER_H
#include "usart.h"
#include "fifo.h"
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "stdio.h"
#include "utils.h"
#define RX_BUFFER_SIZE 256
#define TX_BUFFER_SIZE 256
typedef void (*Clear_TC_Fun)(DMA_TypeDef *DMAx);
typedef void (*UART_RX_Callback)();
// use dma normal mode to prevent overrun
typedef struct UART_InstanceTypeDef
{
    USART_TypeDef *UARTx;
    DMA_TypeDef *RX_DMAx;
    uint32_t RX_DMA_Stream;
    DMA_TypeDef *TX_DMAx;
    uint32_t TX_DMA_Stream;
    uint16_t rx_stack_size; // size to reach a complete packet
    char rx_buffer[RX_BUFFER_SIZE];
    fifo_s_t rx_fifo;
    uint8_t rx_finished_flag;
    char tx_buffer[TX_BUFFER_SIZE];
    fifo_s_t tx_fifo;
    Clear_TC_Fun ClearFlag_TC;
    UART_RX_Callback rx_cb;
}UART_InstanceTypeDef;

__STATIC_INLINE uint16_t UART_GetRxLen(UART_InstanceTypeDef *uart_instance)
{
    return fifo_s_used(&uart_instance->rx_fifo);
}

__STATIC_INLINE char UART_PeekRxFifo(UART_InstanceTypeDef *uart_instance, char ptr)
{
    return fifo_s_preread(&uart_instance->rx_fifo, ptr);
}

__STATIC_INLINE void UART_GetRxFifo(UART_InstanceTypeDef *uart_instance, char *p_dest, uint16_t len)
{
    fifo_s_gets(&uart_instance->rx_fifo, p_dest, len);
}

__STATIC_INLINE void UART_FlushRxFifo(UART_InstanceTypeDef *uart_instance)
{
    __attribute__((unused)) uint32_t u32wk0;
    u32wk0 = uart_instance->UARTx->SR; // Clear status register
    u32wk0 = uart_instance->UARTx->DR; // Clear data register
    fifo_s_flush(&uart_instance->rx_fifo);
    uart_instance->rx_finished_flag = 0;
}

__STATIC_INLINE void UART_PutTxFifo(UART_InstanceTypeDef *uart_instance, char *p_src, uint16_t len)
{
    fifo_s_puts(&uart_instance->tx_fifo, p_src, len);
}

__STATIC_INLINE void UART_FlushTxFifo(UART_InstanceTypeDef *uart_instance)
{
    fifo_s_flush(&uart_instance->tx_fifo);
}

__STATIC_INLINE void UART_RegisterRxCallback(UART_InstanceTypeDef *uart_instance, UART_RX_Callback func)
{
    uart_instance->rx_cb = func;
}

extern UART_InstanceTypeDef UART1_Handler;
extern UART_InstanceTypeDef UART_TOF_Handler;
extern UART_InstanceTypeDef UART_BLE_Handler;

void UART_InstanceInit(UART_InstanceTypeDef *uart_instance, USART_TypeDef *UARTx, DMA_TypeDef *RX_DMAx, uint32_t RX_Stream, DMA_TypeDef *TX_DMAx, uint32_t TX_Stream, uint16_t stack_size);
void UART_printf(UART_InstanceTypeDef *uart_instance, uint8_t transmit, char *fmt, ...);
void UART_TransmitFromFifo(UART_InstanceTypeDef *uart_instance);
void UART_TransmitFromBuffer(UART_InstanceTypeDef *uart_instance, char *p_src, uint16_t len);
void UART_RX_IRQHandler(UART_InstanceTypeDef *uart_instance);

#endif