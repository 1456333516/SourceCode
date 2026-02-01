/*
 * @Author: wushengran
 * @Date: 2024-04-23 16:49:27
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "delay.h"
#include "usart.h"
#include "adc.h"

// 定义全局变量，保存转换后的数据
uint16_t data[2] = {0};

int main(void)
{
	// 1. 初始化
	USART_Init();
	ADC1_Init();
	ADC1_DMA_Init();

	// 打印字符串
	printf("Hello, world!\n");

	// 2. 开启转换
	ADC1_DMA_StartConvert((uint32_t)data, 2);

	// 3. 不停地获取转换结果发往串口
	while (1)
	{
		printf("V1 = %.2f, V2 = %.2f\n", data[0] * 3.3 / 4095, data[1] * 3.3 / 4095);

		Delay_ms(1000);
	}
}
