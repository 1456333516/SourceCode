/*
 * @Author: wushengran
 * @Date: 2024-04-23 16:49:27
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */

#include "led.h"
#include "delay.h"
#include "systick.h"

int main(void)
{
	// 1. ≥ı ºªØ
	LED_Init();
	SysTick_Init();
	
	// 2. À¿—≠ª∑
	while(1)
	{
	}
}

