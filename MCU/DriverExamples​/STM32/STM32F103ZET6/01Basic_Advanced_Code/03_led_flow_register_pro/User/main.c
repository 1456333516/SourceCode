/*
 * @Author: wushengran
 * @Date: 2024-04-23 16:49:27
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */

#include "led.h"
#include "delay.h"

uint8_t n = 3;
uint16_t leds[] = {LED_1, LED_2, LED_3};

int main(void)
{
	// 1. 初始化
	LED_Init();
	
	// 2. 在死循环中依次点亮每个灯
	while(1)
	{
		for (uint8_t i = 0; i < n; i++)
		{
			LED_On(leds[i]);
			Delay_ms(500);
			LED_Off(leds[i]);
		}
	}
}

