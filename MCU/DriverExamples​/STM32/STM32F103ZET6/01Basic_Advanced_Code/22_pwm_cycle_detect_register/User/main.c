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
#include "tim4.h"
#include "delay.h"

int main(void)
{
	// 1. 初始化
	LED_Init();
	USART_Init();
	TIM5_Init();
	TIM4_Init();

	// 打印字符串
	printf("Hello, world!\n");

	// 2. 开启定时器
	TIM5_Start();
	TIM4_Start();

	// 3. 给PWM波形设置占空比
	TIM5_SetDutyCycle(36);

	while (1)
	{
		printf("T = %.2f ms, f = %.2f Hz\n", TIM4_GetPWMCycle(), TIM4_GetPWMFreq());

		Delay_ms(1000);
	}
}
