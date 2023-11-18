#include "tof.h"
static uint8_t tof_ongoing_conn = 0; // 0 - no ongoing, 1 - writing register(wait response), 2 - reading register(wait response),
static char tof_cmd_buffer[8] = {0};
static char tof_ret_buffer[16] = {0};
static char tof_readout_buffer[8] = {0};
static uint8_t tof_readout_len = 0;
static uint16_t tof_ret_len = 0;
uint16_t tof_distance = 0;
uint8_t tof_sensor_state = 0; // 0 - not init, 1 - normal, 2 - connection failed, 3 - connection lost
void TOF_Sensor_Init(void)
{
    UART_InstanceInit(&UART_TOF_Handler, USART3, DMA1, LL_DMA_STREAM_1, DMA1, LL_DMA_STREAM_3, 64);
    UART_RegisterRxCallback(&UART_TOF_Handler, TOF_RX_Callback);
    // uint16_t buf[4] = {0};
    // uint16_t len = 0;
    // TOF_ReadRegister(TOF050F_REG_SCALE, buf, &len);
    // if(tof_sensor_state == 2) UART_printf(&UART1_Handler, 1, "TOF050F Connection failed\n");
    // UART_printf(&UART1_Handler, 1, "TOF050F Returns: %x %x\n", buf[0], buf[1]);
}
// packet: [address, r:0x03/w, regH, regL, dataH, dataL, crcL, crcH]
void TOF_ReadRegister(uint16_t reg, uint16_t* dst, uint16_t* len)
{
    if(!tof_ongoing_conn)
    {
        uint16_t crc = 0;
        uint32_t timeout = 0;
        UART_FlushTxFifo(&UART_TOF_Handler);
        tof_cmd_buffer[0] = TOF050F_ADDRESS;
        tof_cmd_buffer[1] = 0x06;
        tof_cmd_buffer[2] = (char)(reg >> 8);
        tof_cmd_buffer[3] = (char)(reg);
        tof_cmd_buffer[4] = 0x00;
        tof_cmd_buffer[5] = 0x01;
        crc = getCRC16((uint8_t*)tof_cmd_buffer, 6);
        tof_cmd_buffer[6] = (char)(crc);
        tof_cmd_buffer[7] = (char)(crc >> 8);
        UART_PutTxFifo(&UART_TOF_Handler, tof_cmd_buffer, 8);
        UART_TransmitFromFifo(&UART_TOF_Handler);
        tof_ongoing_conn = 2;
        while(tof_ongoing_conn)
        {
            timeout++;
            delay_us(1);
            if(timeout >= TOF050F_COMM_TIMEOUT)
            {
                tof_ongoing_conn = 0;
                tof_sensor_state = 2;
                *len = 0;
                return;
            }
        }
        *len = tof_readout_len;
        memcpy(tof_readout_buffer, dst, *len);
    }
}
// packet: [address, r/w:0x06, regH, regL, dataH, dataL, crcL, crcH]
void TOF_WriteRegister(uint16_t reg, uint16_t data)
{
    if(!tof_ongoing_conn)
    {
        uint16_t crc = 0;
        uint32_t timeout = 0;
        UART_FlushTxFifo(&UART_TOF_Handler);
        tof_cmd_buffer[0] = TOF050F_ADDRESS;
        tof_cmd_buffer[1] = 0x06;
        tof_cmd_buffer[2] = (char)(reg >> 8);
        tof_cmd_buffer[3] = (char)(reg);
        tof_cmd_buffer[4] = (char)(data >> 8);
        tof_cmd_buffer[5] = (char)(data);
        crc = getCRC16((uint8_t*)tof_cmd_buffer, 6);
        tof_cmd_buffer[6] = (char)(crc);
        tof_cmd_buffer[7] = (char)(crc >> 8);
        UART_PutTxFifo(&UART_TOF_Handler, tof_cmd_buffer, 8);
        UART_TransmitFromFifo(&UART_TOF_Handler);
        tof_ongoing_conn = 1;
        while(tof_ongoing_conn)
        {
            timeout++;
            delay_us(1);
            if(timeout >= TOF050F_COMM_TIMEOUT)
            {
                tof_ongoing_conn = 0;
                tof_sensor_state = 2;
                return;
            }
        }
    }
}
void TOF_RX_Callback(void)
{
    tof_ret_len = UART_GetRxLen(&UART_TOF_Handler);
    if(UART_PeekRxFifo(&UART_TOF_Handler, 0) != TOF050F_ADDRESS) // invalid packet, ignore
    {
        UART_FlushRxFifo(&UART_TOF_Handler);
        return;
    }
    if(tof_ongoing_conn == 0) // no ongoing conn, received fixed-time distance report
    {
        if(tof_ret_len == 7) // packet requirements
        {
            UART_GetRxFifo(&UART_TOF_Handler, tof_ret_buffer, tof_ret_len);
            uint16_t crc = getCRC16((uint8_t*)tof_ret_buffer, 5);
            if(tof_ret_buffer[5] == (uint8_t)(crc) && tof_ret_buffer[6] == (uint8_t)(crc >> 8))
            {
                tof_distance = (tof_ret_buffer[3] << 8 | tof_ret_buffer[4]);
                tof_sensor_state = 1;
            }
            else
            {
                UART_FlushRxFifo(&UART_TOF_Handler);
                tof_sensor_state = 2; // CRC mismatch
            }
        }
        else
        {
            // tof_sensor_state = 2; // packet length mismatch
            UART_FlushRxFifo(&UART_TOF_Handler);
        }
    }
    else if(tof_ongoing_conn == 1) // writing register
    {
        if(tof_ret_len == 8) // packet requirements
        {
            UART_GetRxFifo(&UART_TOF_Handler, tof_ret_buffer, tof_ret_len);
            for(uint8_t i = 0; i < 8; i++)
            {
                if(tof_cmd_buffer[i] != tof_ret_buffer[i])
                {
                    tof_sensor_state = 2; // return mismatch with send buffer
                    UART_FlushRxFifo(&UART_TOF_Handler);
                    return;
                }
            }
            tof_ongoing_conn = 0;
        }
        else
        {
            // tof_sensor_state = 2; // packet length mismatch
            UART_FlushRxFifo(&UART_TOF_Handler);
        }
    }
    else if(tof_ongoing_conn == 2) // reading register
    {
        UART_GetRxFifo(&UART_TOF_Handler, tof_ret_buffer, tof_ret_len);
        uint16_t crc = getCRC16((uint8_t*)tof_ret_buffer, tof_ret_len - 2);
        // UART_printf(&UART1_Handler, 1, "2:TOF050F rec len:%d\n", tof_ret_len);
        if(tof_ret_buffer[tof_ret_len - 2] == (uint8_t)(crc) && tof_ret_buffer[tof_ret_len - 1] == (uint8_t)(crc >> 8))
        {
            tof_readout_len = tof_ret_buffer[2];
            for(uint8_t i = 0; i < tof_readout_len; i++)
            {
                tof_readout_buffer[i] = tof_ret_buffer[i + 3];
            }
            tof_ongoing_conn = 0;
        }
        else
        {
            tof_readout_len = 0;
            UART_FlushRxFifo(&UART_TOF_Handler);
            // tof_sensor_state = 2; // CRC mismatch
        }
    }
    UART_FlushRxFifo(&UART_TOF_Handler);
}