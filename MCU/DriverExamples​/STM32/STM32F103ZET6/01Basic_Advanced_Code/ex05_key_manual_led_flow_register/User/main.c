/*
 * @Author: wushengran
 * @Date: 2024-04-23 16:49:27
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "led.h"
#include "delay.h"
#include "key.h"

LED_Status led_status = LED_OFF;

int main(void)
{
	// 1. 初始化
	LED_Init();
	Key_Init();

	// 2. 循环等待中断到来
	while (1)
	{
		// 根据当前的状态，决定要进行的操作
		switch (led_status)
		{
		case L1:
			LED_Off(LED_3);
			LED_On(LED_1);
			break;
		case L2:
			LED_Off(LED_1);
			LED_On(LED_2);
			break;
		case L3:
			LED_Off(LED_2);
			LED_On(LED_3);
			break;

		default:
			break;
		}
	}
}
