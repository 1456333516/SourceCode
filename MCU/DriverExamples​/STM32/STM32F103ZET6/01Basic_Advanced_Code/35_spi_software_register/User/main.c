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

// 定义接收数据的缓冲区
uint8_t buffer[100];
uint8_t size;

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

	// 3. 段擦除
	W25Q32_EraseSector(0, 0);

	// 4. 页写（第一个块的第一个段的第一页）
	W25Q32_WritePage(0,0,0,"12345678",8);

	W25Q32_WritePage(0,0,1,"abcdef",8);

	// 5. 页读
	uint8_t buffer[100] = {0};
	W25Q32_Read(0,0,0, buffer, 8);

	printf("read data = %s\n", buffer);

	W25Q32_Read(0,0,1, buffer, 8);

	printf("read data = %s\n", buffer);

	// 6. 测试：随机完整地址写入
	W25Q32_WritePageRandom(0x00022f,"12345678",8);
	W25Q32_Read(0,0,2, buffer, 255);

	printf("read data = %s\n", buffer);

	while (1)
	{
	}
}
