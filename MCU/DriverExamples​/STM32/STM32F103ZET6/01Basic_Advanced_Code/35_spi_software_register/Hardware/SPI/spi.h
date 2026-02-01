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

// SCK - PA5
#define SCK_HIGH (GPIOA->ODR |= GPIO_ODR_ODR5)
#define SCK_LOW (GPIOA->ODR &= ~GPIO_ODR_ODR5)

// MOSI - PA7
#define MOSI_HIGH (GPIOA->ODR |= GPIO_ODR_ODR7)
#define MOSI_LOW (GPIOA->ODR &= ~GPIO_ODR_ODR7)

// MISO - PA6，读取
#define MISO_READ (GPIOA->IDR & GPIO_IDR_IDR6)

// 延迟
#define SPI_DELAY Delay_us(5)

// 定义函数
void SPI_Init(void);

void SPI_Start(void);
void SPI_Stop(void);

// 主从之间在一个周期内交换一个字节
uint8_t SPI_SwapByte(uint8_t tByte);

#endif
