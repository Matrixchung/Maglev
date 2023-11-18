#include "spi_handler.h"

uint16_t SPI_ReadReg8(SPI_TypeDef *SPIx, uint8_t reg, uint8_t *data)
{
    uint16_t timeout = 0;
    uint16_t state = 0;
    LL_SPI_ReceiveData8(SPIx);
    while(!LL_SPI_IsActiveFlag_TXE(SPIx))
    {
        timeout++;
        if(timeout > SPI_MAX_TIMEOUT)
        {
            state |= 0x01;
            break;
        }
    }
    timeout = 0;
    LL_SPI_TransmitData8(SPIx, reg);
    while(LL_SPI_IsActiveFlag_BSY(SPIx))
    {
        timeout++;
        if(timeout > SPI_MAX_TIMEOUT)
        {
            state |= (0x01 << 1);
            break;
        }
    }
    timeout = 0;
    while(!LL_SPI_IsActiveFlag_RXNE(SPIx))
    {
        timeout++;
        if(timeout > SPI_MAX_TIMEOUT)
        {
            state |= (0x01 << 2);
            break;
        }
    }
    *data = LL_SPI_ReceiveData8(SPIx);
    return state;
}

uint16_t SPI_WriteReg8(SPI_TypeDef *SPIx, uint8_t reg, uint8_t data)
{
    uint16_t timeout = 0;
    uint16_t state = 0;
    while(!LL_SPI_IsActiveFlag_TXE(SPIx))
    {
        timeout++;
        if(timeout > SPI_MAX_TIMEOUT)
        {
            state |= 0x01;
            break;
        }
    }
    timeout = 0;
    LL_SPI_TransmitData8(SPIx, reg);
    while(!LL_SPI_IsActiveFlag_TXE(SPIx))
    {
        timeout++;
        if(timeout > SPI_MAX_TIMEOUT)
        {
            state |= (0x01 << 1);
            break;
        }
    }
    timeout = 0;
    LL_SPI_TransmitData8(SPIx, data);
    while(LL_SPI_IsActiveFlag_BSY(SPIx))
    {
        timeout++;
        if(timeout > SPI_MAX_TIMEOUT)
        {
            state |= (0x01 << 2);
            break;
        }
    }
    return state;
}


uint16_t SPI_ReadReg8_2(SPI_TypeDef *SPIx, uint8_t reg, uint8_t *data)
{
    // uint16_t timeout = 0;
    // uint16_t state = 0;
    return (SPI_ReadReg8(SPIx, reg, data) << 8 | SPI_ReadReg8(SPIx, 0xFF, data));
}

uint16_t SPI_ReadReg16(SPI_TypeDef *SPIx, uint16_t reg, uint16_t *data)
{
    uint16_t timeout = 0;
    uint16_t state = 0;
    while(!LL_SPI_IsActiveFlag_TXE(SPIx))
    {
        timeout++;
        if(timeout > SPI_MAX_TIMEOUT)
        {
            state |= 0x01;
            break;
        }
    }
    timeout = 0;
    LL_SPI_TransmitData16(SPIx, reg);
    while(LL_SPI_IsActiveFlag_BSY(SPIx))
    {
        timeout++;
        if(timeout > SPI_MAX_TIMEOUT)
        {
            state |= (0x01 << 1);
            break;
        }
    }
    timeout = 0;
    while(!LL_SPI_IsActiveFlag_RXNE(SPIx))
    {
        timeout++;
        if(timeout > SPI_MAX_TIMEOUT)
        {
            state |= (0x01 << 2);
            break;
        }
    }
    *data = LL_SPI_ReceiveData16(SPIx);
    return state;
}

uint16_t SPI_ReadReg8N(SPI_TypeDef *SPIx, uint8_t reg, uint8_t *data, uint8_t len)
{
    SPI_ReadReg8(SPIx, reg, data);
    for(uint8_t i = 0; i < len; i++)
    {
        SPI_ReadReg8(SPIx, 0xFF, (data+i));
    }
    return 0;
    // uint16_t timeout = 0;
    // uint16_t state = 0;
    // while(!LL_SPI_IsActiveFlag_TXE(SPIx))
    // {
    //     timeout++;
    //     if(timeout > SPI_MAX_TIMEOUT)
    //     {
    //         state |= 0x01;
    //         break;
    //     }
    // }
    // for(uint8_t i = 0; i < len; i++)
    // {
    //     timeout = 0;
    //     LL_SPI_TransmitData8(SPIx, reg);
    //     while(LL_SPI_IsActiveFlag_BSY(SPIx))
    //     {
    //         timeout++;
    //         if(timeout > SPI_MAX_TIMEOUT)
    //         {
    //             state |= (0x01 << 1);
    //             break;
    //         }
    //     }
    //     timeout = 0;
    //     while(!LL_SPI_IsActiveFlag_RXNE(SPIx))
    //     {
    //         timeout++;
    //         if(timeout > SPI_MAX_TIMEOUT)
    //         {
    //             state |= (0x01 << (i + 2));
    //             break;
    //         }
    //     }
    //     *(data + i) = LL_SPI_ReceiveData8(SPIx);
    // }
    // return state;
}