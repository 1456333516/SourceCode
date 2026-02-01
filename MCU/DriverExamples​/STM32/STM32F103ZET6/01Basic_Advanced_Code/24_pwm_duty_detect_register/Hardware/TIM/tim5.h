/*
 * @Author: wushengran
 * @Date: 2024-05-06 16:26:08
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */

#ifndef __TIM5_H
#define __TIM5_H

#include "stm32f10x.h"

// 初始化
void TIM5_Init(void);

// 将开关包装成函数
void TIM5_Start(void);
void TIM5_Stop(void);

// 设置占空比的函数
void TIM5_SetDutyCycle(uint8_t dutyCycle);

#endif
