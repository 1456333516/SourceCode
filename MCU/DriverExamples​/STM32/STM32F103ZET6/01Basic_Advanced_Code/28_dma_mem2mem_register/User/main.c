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

// 定义全局常量，保存在 ROM 里
const uint8_t src[4] = {10, 20, 30, 40};

// 定义全局变量，保存在 RAM 里
uint8_t dest[4] = {0};

// 定义全局变量，数据传输完成标记位
uint8_t isFinished = 0;

int main(void)
{
	// 1. 初始化
	USART_Init();
	DMA1_Init();

	// 打印字符串
	printf("Hello, world!\n");

	// 2. 打印数据地址
	printf("src addr = %p, dest addr = %p\n", (uint32_t *)&src[0], (uint32_t *)&dest[0]);

	// 3. 启动 DMA 通道传输数据
	DMA1_CH1_Transmit((uint32_t)&src[0], (uint32_t)&dest[0], 4);

	// // 4. 等待传输完成
	// while (!isFinished)
	// {
	// }

	// // 5. 打印变量进行验证
	// for (uint8_t i = 0; i < 4; i++)
	// {
	// 	printf("%d\t", dest[i]);
	// }

	while (1)
	{
		if (isFinished)
		{
			for (uint8_t i = 0; i < 4; i++)
			{
				printf("%d\t", dest[i]);
			}
			isFinished = 0;
		}
	}
}
