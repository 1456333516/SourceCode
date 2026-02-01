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

uint8_t n = 3;
uint16_t leds[] = {LED_1, LED_2, LED_3};
uint8_t isOn = 0;

int main(void)
{
	// 1. 初始化
	LED_Init();
	Key_Init();

	// 2. 循环等待中断到来
	while (1)
	{
		if (isOn)
		{
			for (uint8_t i = 0; i < n; i++)
			{
				LED_On(leds[i]);
				Delay_ms(500);
				LED_Off(leds[i]);

				if ((uint8_t)~isOn)
				{
					break;
				}	
			}
		}
		else
		{
			LED_OffAll(leds, n);
		}
	}
}
