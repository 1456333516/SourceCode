#ifndef __INF_LCD_H
#define __INF_LCD_H

#include "Delay.h"
#include "Driver_FSMC.h"

#define LCD_BASE_ADDR 0X6C000000
#define LCD_CMD_ADDR (uint16_t *)LCD_BASE_ADDR
#define LCD_DATA_ADDR (uint16_t *)(LCD_BASE_ADDR + (1 << 11))

#define DISPLAY_W 320
#define DISPLAY_H 480

/* 常见颜色 */
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40   // 棕色
#define BRRED 0XFC07   // 棕红色
#define GRAY 0X8430    // 灰色

void     Inf_LCD_Init(void);
uint32_t Inf_LCD_ReadId(void);

void Inf_LCD_ShowAsciChar(uint16_t x, uint16_t y, uint16_t height, uint8_t c, uint16_t fColor,
                          uint16_t bColor);
void Inf_LCD_ShowAsciString(uint16_t x, uint16_t y, uint8_t height, uint8_t *str, uint16_t fColor,
                            uint16_t bColor);
void Inf_LCD_ShowChineseChar(uint16_t x, uint16_t y, uint8_t charIndex, uint16_t fColor,
                             uint16_t bColor);
void Inf_LCD_SetColor(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void Inf_LCD_ShowImg(uint16_t x, uint16_t y, uint8_t *img, uint32_t len, uint16_t imgW,
                     uint16_t imgH);
void Inf_LCD_ShowPoint(uint16_t x, uint16_t y, uint8_t w, uint16_t color);
void Inf_LCD_ShowLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t w,
                      uint16_t color);
void Inf_LCD_ShowCircle(uint16_t centerX, uint16_t centerY, uint16_t r, uint8_t w, uint16_t color);
#endif
