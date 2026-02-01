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
	
	// 2. 往返流水灯
	while(1)
	{
		for (uint8_t i = 0; i < n; i++)
		{
			LED_On(leds[i]);
			Delay_ms(500);
			LED_Off(leds[i]);
		}
		// // 中间插入一次LED2的开关
		// LED_On(LED_2);
		// Delay_ms(500);
		// LED_Off(LED_2);

		// // 通用写法：反向遍历LED灯
		// for (uint8_t i = n - 2; i > 0; i--)
		// {
		// 	LED_On(leds[i]);
		// 	Delay_ms(500);
		// 	LED_Off(leds[i]);
		// }
		
		// 通用写法：反向遍历LED灯
		for (int8_t i = n - 1; i >= 0; i--)
		{
			LED_On(leds[i]);
			Delay_ms(500);
			LED_Off(leds[i]);
		}
	}
}

