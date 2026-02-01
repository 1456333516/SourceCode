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

// 定义接收数据完成标志位
uint8_t isOver;

int main(void)
{
	// 1. 初始化
	LED_Init();
	Key_Init();
	USART_Init();

	// 测试发送一个字符
	// SendChar('a');
	// SendChar('x');

	// 测试发送字符串
	SendString("Hello, world!\n", 14);

	while (1)
	{
		// 接收一个字符，并发回去
		// uint8_t ch = ReceiveChar();
		// SendChar(ch - 32);

		// 接收一个字符串，并发回去
		// ReceiveString(buffer, &size);

		// 判断标志为1，就发回去
		if (isOver)
		{
			SendString(buffer, size);
			// 恢复初始状态
			size = 0;
			isOver = 0;
		}
	}
}
