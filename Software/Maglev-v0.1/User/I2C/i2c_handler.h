#ifndef _I2C_HANDLER_H
#define _I2C_HANDLER_H

#include "main.h"
#include "i2c.h"

#define I2C_WAIT_TIMEOUT_CYCLES 5000 // in cycle

uint8_t I2C_LL_Write(I2C_TypeDef *hi2c, uint8_t addr, uint8_t reg, uint8_t data);
uint8_t I2C_LL_WriteBuffer(I2C_TypeDef *hi2c, uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len);
uint8_t I2C_LL_StartDMA(I2C_TypeDef *hi2c, uint8_t addr, uint8_t reg, DMA_TypeDef *DMAx, uint32_t Stream, uint32_t len);
// uint8_t I2C_Soft_Write(uint8_t addr, uint8_t reg, uint8_t data);

#endif