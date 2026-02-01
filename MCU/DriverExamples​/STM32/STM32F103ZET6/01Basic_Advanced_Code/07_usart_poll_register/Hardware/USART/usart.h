/*
 * @Author: wushengran
 * @Date: 2024-04-27 16:11:25
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */

#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"

// 初始化
void USART_Init(void);

// 发送一个字符
void SendChar(uint8_t ch);

// 接收一个字符
uint8_t ReceiveChar(void);

// 发送字符串
void SendString(uint8_t * str, uint8_t size);

// 接收字符串
void ReceiveString(uint8_t buffer[], uint8_t * pSize);

#endif
