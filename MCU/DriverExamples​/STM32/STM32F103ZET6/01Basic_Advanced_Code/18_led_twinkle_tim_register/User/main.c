/*
 * @Author: wushengran
 * @Date: 2024-04-23 16:49:27
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "led.h"
#include "usart.h"
#include "tim6.h"


int main(void)
{
	// 1. ³õÊ¼»¯
	LED_Init();
	USART_Init();
	TIM6_Init();

	// ´òÓ¡×Ö·û´®
	printf("Hello, world!\n");

	while (1)
	{
	}
}
