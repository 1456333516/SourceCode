/*
 * @Author: wushengran
 * @Date: 2024-04-23 16:49:27
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "tim1.h"

int main(void)
{
	// 1. 初始化
	USART_Init();
	TIM1_Init();

	// 打印字符串
	printf("Hello, world!\n");

	// 2. 开启定时器
	TIM1_Start();

	while (1)
	{
	}
}
