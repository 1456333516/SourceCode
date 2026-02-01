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
#include "usart.h"

// 定义接收数据的缓冲区
uint8_t buffer[100];
uint8_t size;

int main(void)
{
	// 1. 初始化
	USART_Init();

	// 打印字符串
	printf("Hello, world!\n");

	uint8_t count = 0;
	while (1)
	{
		count ++;
		printf("hello, atguigu! %d\n", count);
		Delay_ms(1000);
	}
}
