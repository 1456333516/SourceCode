/*
 * @Author: wushengran
 * @Date: 2024-04-23 16:49:27
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "fsmc.h"

// 方法一：定义全局变量，指定变量的地址
uint8_t v1 __attribute__((at(0x68000000)));
uint16_t v2 __attribute__((at(0x68000004)));
uint16_t v3 = 30;

int main(void)
{
	// 1. 初始化
	USART_Init();
	FSMC_Init();

	printf("Hello world\n");

	// 定义各种变量，打印地址和值
	v1 = 10;
	v2 = 20;
	uint8_t v4 __attribute__((at(0x68000008)));
	v4 = 40;
	uint16_t v5 = 50;

	printf("v1 = %d, @%p\n", v1, &v1);
	printf("v2 = %d, @%p\n", v2, &v2);
	printf("v3 = %d, @%p\n", v3, &v3);
	printf("v4 = %d, @%p\n", v4, &v4);
	printf("v5 = %d, @%p\n", v5, &v5);

	// 方法二：定义指针
	uint8_t * p = (uint8_t *)0x68000001;
	*p = 100;
	printf("*p = %d, @%p, @@%p\n", *p, p, &p);

	while (1)
	{
	}
}
