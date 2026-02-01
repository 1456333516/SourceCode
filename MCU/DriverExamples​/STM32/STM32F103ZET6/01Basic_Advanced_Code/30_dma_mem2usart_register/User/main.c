/*
 * @Author: wushengran
 * @Date: 2024-04-23 16:49:27
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "delay.h"
#include "usart.h"
#include "dma.h"

// 定义全局变量，保存在 RAM 里
uint16_t src[4] = {'a', 'b', 'c', 'd'};

int main(void)
{
	// 1. 初始化
	USART_Init();
	DMA1_Init();

	// 打印字符串
	printf("Hello, world!\n");

	// 2. 启动 DMA 通道传输数据
	DMA1_CH4_Transmit((uint32_t)&src[0], (uint32_t)&(USART1->DR), 4);

	while (1)
	{
	}
}
