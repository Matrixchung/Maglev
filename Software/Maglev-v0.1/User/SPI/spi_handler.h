#ifndef _SPI_HANDLER_H
#define _SPI_HANDLER_H

#include "main.h"

#define SPI_MAX_TIMEOUT 0xFFFF

uint16_t SPI_WriteReg8(SPI_TypeDef *SPIx, uint8_t reg, uint8_t data);
uint16_t SPI_ReadReg8(SPI_TypeDef *SPIx, uint8_t reg, uint8_t *data);
uint16_t SPI_ReadReg8_2(SPI_TypeDef *SPIx, uint8_t reg, uint8_t *data);
uint16_t SPI_ReadReg16(SPI_TypeDef *SPIx, uint16_t reg, uint16_t *data);
uint16_t SPI_ReadReg8N(SPI_TypeDef *SPIx, uint8_t reg, uint8_t *data, uint8_t len);

#endif