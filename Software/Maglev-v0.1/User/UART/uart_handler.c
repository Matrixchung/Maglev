#include "uart_handler.h"

UART_InstanceTypeDef UART1_Handler;
UART_InstanceTypeDef UART_TOF_Handler;
UART_InstanceTypeDef UART_BLE_Handler;

// stack size is used to receive fixed-size data packet
void UART_InstanceInit(UART_InstanceTypeDef *uart_instance, USART_TypeDef *UARTx, DMA_TypeDef *RX_DMAx, uint32_t RX_Stream, DMA_TypeDef *TX_DMAx, uint32_t TX_Stream, uint16_t stack_size)
{
    uart_instance->UARTx = UARTx;
    uart_instance->RX_DMAx = RX_DMAx;
    uart_instance->RX_DMA_Stream = RX_Stream;
    uart_instance->TX_DMAx = TX_DMAx;
    uart_instance->TX_DMA_Stream = TX_Stream;
    memset(uart_instance->rx_buffer, 0, RX_BUFFER_SIZE);
    memset(uart_instance->tx_buffer, 0, TX_BUFFER_SIZE);
    uart_instance->rx_fifo = *fifo_s_create(RX_BUFFER_SIZE);
    uart_instance->tx_fifo = *fifo_s_create(TX_BUFFER_SIZE);
    uart_instance->rx_finished_flag = 0;
    uart_instance->rx_stack_size = stack_size;
    uart_instance->ClearFlag_TC = NULL;
    uart_instance->rx_cb = NULL;
    switch(TX_Stream)
    {
        case LL_DMA_STREAM_0:
            uart_instance->ClearFlag_TC = LL_DMA_ClearFlag_TC0;
            break;
        case LL_DMA_STREAM_1:
            uart_instance->ClearFlag_TC = LL_DMA_ClearFlag_TC1;
            break;
        case LL_DMA_STREAM_2:
            uart_instance->ClearFlag_TC = LL_DMA_ClearFlag_TC2;
            break;
        case LL_DMA_STREAM_3:
            uart_instance->ClearFlag_TC = LL_DMA_ClearFlag_TC3;
            break;
        case LL_DMA_STREAM_4:
            uart_instance->ClearFlag_TC = LL_DMA_ClearFlag_TC4;
            break;
        case LL_DMA_STREAM_5:
            uart_instance->ClearFlag_TC = LL_DMA_ClearFlag_TC5;
            break;
        case LL_DMA_STREAM_6:
            uart_instance->ClearFlag_TC = LL_DMA_ClearFlag_TC6;
            break;
        case LL_DMA_STREAM_7:
            uart_instance->ClearFlag_TC = LL_DMA_ClearFlag_TC7;
            break;
    }
    LL_DMA_SetPeriphAddress(uart_instance->RX_DMAx, uart_instance->RX_DMA_Stream, LL_USART_DMA_GetRegAddr(uart_instance->UARTx));
    LL_DMA_SetPeriphAddress(uart_instance->TX_DMAx, uart_instance->TX_DMA_Stream, LL_USART_DMA_GetRegAddr(uart_instance->UARTx));
    LL_DMA_SetMemoryAddress(uart_instance->RX_DMAx, uart_instance->RX_DMA_Stream, (uint32_t)uart_instance->rx_buffer);
    LL_DMA_SetDataLength(uart_instance->RX_DMAx, uart_instance->RX_DMA_Stream, uart_instance->rx_stack_size);
    // LL_DMA_DisableIT_HT(uart_instance->RX_DMAx, uart_instance->RX_DMA_Stream);
    LL_USART_EnableDMAReq_RX(uart_instance->UARTx);
    LL_DMA_EnableStream(uart_instance->RX_DMAx, uart_instance->RX_DMA_Stream);
    LL_USART_EnableIT_IDLE(uart_instance->UARTx);
    // LL_DMA_DisableIT_TC(uart_instance->TX_DMAx, uart_instance->TX_DMA_Stream);
    LL_USART_Enable(uart_instance->UARTx);
    __attribute__((unused)) uint32_t u32wk0;
    u32wk0 = uart_instance->UARTx->SR; // Clear status register
    u32wk0 = uart_instance->UARTx->DR; // Clear data register
}

void UART_printf(UART_InstanceTypeDef *uart_instance, uint8_t transmit, char *fmt, ...)
{
    char buffer[TX_BUFFER_SIZE];
    va_list args;
    uint16_t buffer_len = 0;
    va_start(args, fmt);
    vsnprintf(buffer, TX_BUFFER_SIZE, fmt, args);
    buffer_len = strlen(buffer);
    if(buffer_len > TX_BUFFER_SIZE) buffer_len = TX_BUFFER_SIZE;
    UART_PutTxFifo(uart_instance, buffer, buffer_len);
    // UART_Transmit, 这里有一种设想的实现方法, 就是在多个 printf 之后一次性执行发送，充分利用 FIFO 的优势。
    if(transmit) UART_TransmitFromFifo(uart_instance);
}

void UART_TransmitFromFifo(UART_InstanceTypeDef *uart_instance)
{
    if(uart_instance->ClearFlag_TC != NULL) uart_instance->ClearFlag_TC(uart_instance->TX_DMAx);
    uint16_t len = fifo_s_used(&uart_instance->tx_fifo);
    fifo_s_gets(&uart_instance->tx_fifo, uart_instance->tx_buffer, len);
    LL_DMA_SetMemoryAddress(uart_instance->TX_DMAx, uart_instance->TX_DMA_Stream, (uint32_t)uart_instance->tx_buffer);
    LL_DMA_SetDataLength(uart_instance->TX_DMAx, uart_instance->TX_DMA_Stream, len);
    LL_DMA_EnableStream(uart_instance->TX_DMAx, uart_instance->TX_DMA_Stream);
    LL_USART_EnableDMAReq_TX(uart_instance->UARTx);
}

void UART_TransmitFromBuffer(UART_InstanceTypeDef *uart_instance, char *p_src, uint16_t len)
{
    if(uart_instance->ClearFlag_TC != NULL) uart_instance->ClearFlag_TC(uart_instance->TX_DMAx);
    LL_DMA_SetMemoryAddress(uart_instance->TX_DMAx, uart_instance->TX_DMA_Stream, (uint32_t)p_src);
    LL_DMA_SetDataLength(uart_instance->TX_DMAx, uart_instance->TX_DMA_Stream, len);
    LL_DMA_EnableStream(uart_instance->TX_DMAx, uart_instance->TX_DMA_Stream);
    LL_USART_EnableDMAReq_TX(uart_instance->UARTx);
}

void UART_RX_IRQHandler(UART_InstanceTypeDef *uart_instance)
{
    if(LL_USART_IsActiveFlag_IDLE(uart_instance->UARTx))
    {
        LL_USART_ClearFlag_IDLE(uart_instance->UARTx);
        LL_USART_ClearFlag_ORE(uart_instance->UARTx);
        // "Once the stream is enabled, the LL_DMA_GetDataLength return value indicate the remaining bytes to be transmitted." -- STM32F4 Ref Manual
        uint16_t len = uart_instance->rx_stack_size - LL_DMA_GetDataLength(uart_instance->RX_DMAx, uart_instance->RX_DMA_Stream);
        if(len > RX_BUFFER_SIZE) len = RX_BUFFER_SIZE;
        // Disable stream to make sure a reliable fifo transfer and we can SetDataLength later.
        LL_DMA_DisableStream(uart_instance->RX_DMAx, uart_instance->RX_DMA_Stream);
        fifo_s_puts(&uart_instance->rx_fifo, uart_instance->rx_buffer, len);
        // "LL_DMA_SetDataLength has no effect if stream is enabled." So we disable stream first and re-enable it.
        LL_DMA_SetDataLength(uart_instance->RX_DMAx, uart_instance->RX_DMA_Stream, uart_instance->rx_stack_size);
        LL_DMA_EnableStream(uart_instance->RX_DMAx, uart_instance->RX_DMA_Stream);
        uart_instance->rx_finished_flag = 1;
        if(uart_instance->rx_cb != NULL) uart_instance->rx_cb();
    }
}
