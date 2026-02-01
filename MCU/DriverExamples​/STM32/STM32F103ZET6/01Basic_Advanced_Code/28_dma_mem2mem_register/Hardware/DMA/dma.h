/*
 * @Author: wushengran
 * @Date: 2024-05-11 11:21:14
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */

#ifndef __DMA_H
#define __DMA_H

#include "stm32f10x.h"

extern uint8_t isFinished;

// 初始化
void DMA1_Init(void);

// DMA1的通道1传输数据，参数：源地址、目的地址、数据长度
void DMA1_CH1_Transmit(uint32_t srcAddr, uint32_t destAddr, uint8_t dataLen);

#endif
