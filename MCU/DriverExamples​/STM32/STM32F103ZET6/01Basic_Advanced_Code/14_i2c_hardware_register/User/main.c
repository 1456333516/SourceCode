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
#include "m24c02.h"
#include <string.h>

// 定义接收数据的缓冲区
uint8_t buffer[100];
uint8_t size;

int main(void)
{
	// 1. 初始化
	USART_Init();
	M24C02_Init();

	// 打印字符串
	printf("Hello, world!\n");

	// 2. 从0地址开始，依次写入单个字符
	M24C02_WriteByte(0x00, 'a');
	M24C02_WriteByte(0x01, 'b');
	M24C02_WriteByte(0x02, 'c');

	// 3. 读取EEPROM中的数据
	printf("%c\n", M24C02_ReadByte(0x00));
	printf("%c\n", M24C02_ReadByte(0x01));
	printf("%c\n", M24C02_ReadByte(0x02));

	// 4. 连续写入多个字符（页写）
	M24C02_WriteBytes(0x00, "123456", 6);

	// printf("%c\n", M24C02_ReadByte(0x00));
	// printf("%c\n", M24C02_ReadByte(0x01));
	// printf("%c\n", M24C02_ReadByte(0x02));

	// 5. 连续读取多个字符
	uint8_t buffer[100] = {0};
	M24C02_ReadBytes(0x00, buffer, 7);
	printf("0x00 地址开始的数据为：%s\n", buffer);

	memset(buffer, 0, sizeof(buffer));

	M24C02_WriteByte(0x06, 'x');
	M24C02_ReadBytes(0x00, buffer, 7);
	printf("0x00 地址开始的数据为：%s\n", buffer);

	memset(buffer, 0, sizeof(buffer));

	// 6. 超出一页范围的连续写入
	M24C02_WriteBytes(0x00, "1234567890abcdefghi", 19);
	M24C02_ReadBytes(0x00, buffer, 19);
	printf("0x00 地址开始的数据为：%s\n", buffer);

	M24C02_WriteBytes(0x10, "1234567890abcdefghi", 19);
	M24C02_ReadBytes(0x00, buffer, 32);
	printf("0x00 地址开始的数据为：%s\n", buffer);

	while (1)
	{
	}
}
