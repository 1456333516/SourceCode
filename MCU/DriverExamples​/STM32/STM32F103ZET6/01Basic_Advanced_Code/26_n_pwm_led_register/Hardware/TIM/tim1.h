/*
 * @Author: wushengran
 * @Date: 2024-05-08 10:33:39
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
/*
 * @Author: wushengran
 * @Date: 2024-05-06 16:26:08
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */

#ifndef __TIM1_H
#define __TIM1_H

#include "stm32f10x.h"

// 初始化
void TIM1_Init(void);

// 将开关包装成函数
void TIM1_Start(void);
void TIM1_Stop(void);

#endif
