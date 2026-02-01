/*
 * @Author: wushengran
 * @Date: 2024-05-13 09:08:31
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
/*
 * @Author: wushengran
 * @Date: 2024-05-12 14:35:05
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */

#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"

// 初始化
void ADC1_Init(void);

// DMA 相关的初始化
void ADC1_DMA_Init(void);

// 开启转换的函数
void ADC1_DMA_StartConvert(uint32_t destAddr, uint8_t dataLen);

// 返回转换后的数字信号计算成的模拟电压值

// double ADC1_ReadV(void);

#endif
