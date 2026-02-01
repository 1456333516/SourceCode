/*
 * @Author: wushengran
 * @Date: 2024-05-17 10:43:46
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __LCD_H
#define __LCD_H

#include "fsmc.h"
// #include "delay.h"
#include <math.h>

// 宏定义
// 定义“写命令”/“写数据”地址
#define SRAM_BANK4_Base 0x6C000000
#define LCD_ADDR_CMD (uint16_t *)SRAM_BANK4_Base
#define LCD_ADDR_DATA (uint16_t *)(SRAM_BANK4_Base + (1 << 11))

// 定义显示屏的宽和高
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
#define BROWN 0XBC40 // 棕色
#define BRRED 0XFC07 // 棕红色
#define GRAY 0X8430  // 灰色

// 1. 底层基本控制操作
// 初始化
void LCD_Init(void);

// 重置复位
void LCD_Reset(void);

// 开关背光
void LCD_BGOn(void);
void LCD_BGOff(void);

// 发送命令（写命令）
void LCD_WriteCmd(uint16_t cmd);

// 发送数据（写入数据）
void LCD_WriteData(uint16_t data);

// 读取数据
uint16_t LCD_ReadData(void);

// 配置LCD控制寄存器
void LCD_RegConfig(void);


// 2. 包装应用层接口
// 2.1 返回ID信息
uint32_t LCD_ReadID(void);

// 2.2 设置操作区域的范围，给定 起始点横纵坐标、区域的宽高
void LCD_SetArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

// 2.3 清屏（整个屏幕用指定颜色来覆盖）
void LCD_ClearAll(uint16_t color);

// 2.4 显示一个英文字符，给定显示区域左上角坐标、字体高、字体颜色、背景颜色
void LCD_WriteAsciiChar(uint16_t x, uint16_t y, uint16_t h, uint8_t c, uint16_t fColor, uint16_t bColor);

// 2.5 显示英文字符串，给定显示区域左上角坐标、字体高、字体颜色、背景颜色
void LCD_WriteAsciiString(uint16_t x, uint16_t y, uint16_t h, uint8_t *str, uint16_t fColor, uint16_t bColor);

// 2.6 显示一个汉字，给定显示区域左上角坐标、字体高、字体颜色、背景颜色、在字库中的索引
void LCD_WriteChineseChar(uint16_t x, uint16_t y, uint16_t h, uint8_t index, uint16_t fColor, uint16_t bColor);

// 2.7 显示一张图片，尚硅谷logo
void LCD_DisplayAtguiguLogo(uint16_t x, uint16_t y);

// 2.8 画一个点/画一个正方形（实心），给定左上角坐标
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t w, uint16_t color);

// 2.9 画一条线
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t w, uint16_t color);

// 2.10 画一个长方形（空心）
void LCD_DrawRectangle(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, uint16_t w, uint16_t color);

// 2.11 画一个圆（空心），给定圆心坐标和半径
void LCD_DrawCircle(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color);

// 2.12 画一个实心圆
void LCD_DrawFilledCircle(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t fColor, uint16_t bColor);

#endif
