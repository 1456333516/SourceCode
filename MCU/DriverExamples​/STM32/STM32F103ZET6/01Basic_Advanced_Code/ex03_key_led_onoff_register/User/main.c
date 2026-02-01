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

int main(void)
{
	// 1. 初始化
	LED_Init();
	Key_Init();
	
	// 2. 循环等待中断到来
	while(1)
	{

	}
}

