/*
 * @Author: wushengran
 * @Date: 2024-05-13 15:13:39
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */

#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x.h"
#include "delay.h"

// 定义 CS 操作 - PC13
#define CS_HIGH (GPIOC->ODR |= GPIO_ODR_ODR13)
#define CS_LOW (GPIOC->ODR &= ~GPIO_ODR_ODR13)

// 定义函数
void SPI_Init(void);

void SPI_Start(void);
void SPI_Stop(void);

// 主从之间在一个周期内交换一个字节
uint8_t SPI_SwapByte(uint8_t tByte);

#endif
