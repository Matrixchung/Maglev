/*
 * oled.h
 *
 *  Created on: Nov 2, 2022
 *      Author: Matrix
 */

#ifndef OLED_OLED_H_
#define OLED_OLED_H_

#include "main.h"
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "stdio.h"
#include "i2c_handler.h"
#include "cmsis_gcc.h"
#include "oledfont.h"

#define OLED_CMD 0  // 写命令
#define OLED_DATA 1 // 写数据
#define OLED_REG_CMD 0x00
#define OLED_REG_DATA 0x40

#define OLED_NREFRESH 0
#define OLED_REFRESH 1

#define MODE_REVERSE 0
#define MODE_NORMAL 1

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_PAGE 8
#define OLED_ADDR 0x78

#define BIG_FONT 16
#define SMALL_FONT 12

#define MAX_CHAR_POSX 121
#define MAX_CHAR_POSY 58

extern uint8_t OLED_GRAM[OLED_PAGE][OLED_WIDTH]; // OLED Graphics RAM
extern uint8_t OLED_DMA_TC_FLAG;

void OLED_WriteByte(uint8_t data, uint8_t cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_RefreshGram(void);
void OLED_DMA_TC_Callback(void);
void OLED_DMA_Error_Callback(void);
// void OLED_RefreshGram_Line(uint8_t line);
// void OLED_RefreshGram_Pixel(uint8_t x, uint8_t y);
void OLED_Init(void);
void OLED_Clear(uint8_t refresh);
void OLED_DrawPixel(uint8_t x, uint8_t y, uint8_t state);
void OLED_DrawChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode);
void OLED_DrawNumber(uint8_t x, uint8_t y, uint32_t num, uint8_t size);
void OLED_DrawIcon(uint8_t x, uint8_t y, enum OLED_ICON icon, uint8_t mode);
void OLED_DrawString(uint8_t x, uint8_t y, uint8_t size, uint8_t mode, const char *p);
void OLED_printf(uint8_t x, uint8_t y, uint8_t size, uint8_t mode, char *fmt, ...);
void OLED_SetBrightness(uint8_t brightness);

#endif /* OLED_OLED_H_ */
