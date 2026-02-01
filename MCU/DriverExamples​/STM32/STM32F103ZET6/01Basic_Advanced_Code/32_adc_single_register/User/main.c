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

int main(void)
{
	// 1. 初始化
	USART_Init();
	ADC1_Init();

	// 打印字符串
	printf("Hello, world!\n");

	// 2. 开启转换
	ADC1_StartConvert();

	// 3. 不停地获取转换结果发往串口
	while (1)
	{
		printf("V = %.2f\n", ADC1_ReadV());

		Delay_ms(1000);
	}
}
