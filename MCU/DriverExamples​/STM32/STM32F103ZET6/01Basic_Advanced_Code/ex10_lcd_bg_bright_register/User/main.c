/*
 * @Author: wushengran
 * @Date: 2024-04-23 16:49:27
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "key.h"
#include "usart.h"
#include "tim3.h"
#include "delay.h"

// 定义全局变量
uint8_t isAdd;
uint8_t isSub;

int main(void)
{
	// 1. 初始化
	USART_Init();
	TIM3_Init();
	Key_Init();

	// 打印字符串
	printf("Hello, world!\n");

	// 2. 开启定时器
	TIM3_Start();

	// 定义变量
	uint8_t dutyCycle = 0;
	int8_t step = 10;

	// 3. 在循环中检测到按键按下，就改变占空比

	while (1)
	{
		if (isAdd)
		{
			dutyCycle = dutyCycle + step > 99 ? 99 : dutyCycle + step;
			TIM3_SetDutyCycle(dutyCycle, 3);

			isAdd = 0;
		}
		if (isSub)
		{
			dutyCycle = dutyCycle - step < 0 ? 0 : dutyCycle - step;
			TIM3_SetDutyCycle(dutyCycle, 3);

			isSub = 0;
		}
	}
}
