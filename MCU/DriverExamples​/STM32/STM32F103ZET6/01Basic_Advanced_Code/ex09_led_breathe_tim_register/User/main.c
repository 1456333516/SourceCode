/*
 * @Author: wushengran
 * @Date: 2024-04-23 16:49:27
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "led.h"
#include "usart.h"
#include "tim5.h"
#include "tim3.h"
#include "delay.h"

int main(void)
{
	// 1. 初始化
	LED_Init();
	USART_Init();
	TIM5_Init();
	TIM3_Init();

	// 打印字符串
	printf("Hello, world!\n");

	// 2. 开启定时器
	TIM5_Start();
	TIM3_Start();

	// 定义变量
	uint8_t dutyCycle = 0;
	uint8_t dir = 0; // 占空比改变方向，0 表示增大，1 表示减小

	// 3. 在循环中不停地改变占空比

	while (1)
	{
		// 3.1 根据方向，改变占空比
		if (dir == 0)
		{
			dutyCycle++;
			if (dutyCycle >= 99)
			{
				dir = 1; // 如果到上限就改变方向为递减
			}
		}
		else
		{
			dutyCycle--;
			if (dutyCycle <= 1)
			{
				dir = 0; // 如果到下限就改变方向为递增
			}
		}

		// 3.2 设置占空比
		TIM5_SetDutyCycle(dutyCycle, 2);
		// TIM5_SetDutyCycle(dutyCycle, 1);
		TIM5_SetDutyCycle(100 - dutyCycle, 1);
		TIM3_SetDutyCycle(dutyCycle, 3);

		// 3.3 延迟 10 ms，一次完整的改变就是 1s
		Delay_ms(10);
	}
}
