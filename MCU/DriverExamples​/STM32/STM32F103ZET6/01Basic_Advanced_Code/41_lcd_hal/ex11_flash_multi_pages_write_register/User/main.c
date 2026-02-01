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
#include "w25q32.h"
#include <string.h>

int main(void)
{
	// 1. 初始化
	USART_Init();
	W25Q32_Init();

	printf("Hello world\n");

	// 2. 读取ID，测试Flash是否正常
	uint8_t mid = 0;
	uint16_t did = 0;
	W25Q32_ReadID(&mid, &did);
	printf("mid = %#x, did=%#x\n", mid, did);

	// 3. 跨页写入

	// 3.1 构建符合要求的数据字符串
	uint8_t str[261] = {0};
	uint8_t a2z[27] = {0};

	for (uint8_t i = 'a'; i <= 'z'; i++)
	{
		a2z[i-'a'] = i;
	}
	for (uint8_t i = 0; i < 10; i++)
	{
		strcat((char *)str, (char *)a2z);
	}

	printf("str = %s, len = %d\n", (char *)str, strlen((char *)str));
	
	// 3.2 根据地址，算出分页写入的两页各需要存放多少字符
	uint32_t addr = 0x32f680;
	uint8_t size1 = 0xff - (addr & 0xff) + 1;
	uint8_t size2 = strlen((char *)str) - size1;
	printf("size1 = %d, size2 = %d\n", size1, size2);

	// 3.3 先擦除
	W25Q32_EraseSector((addr >> 16) & 0xff, (addr >> 12) & 0x0f);

	// 3.4 两次写入
	W25Q32_WritePageRandom(addr, str, size1);
	W25Q32_WritePage(0x32, 0xf, 0x7, str + size1, size2);

	// 4. 跨页读取
	uint8_t buffer[300];
	W25Q32_ReadRandom(addr, buffer, strlen((char *)str));

	printf("buffer = %s\n", buffer);

	while (1)
	{
	}
}
