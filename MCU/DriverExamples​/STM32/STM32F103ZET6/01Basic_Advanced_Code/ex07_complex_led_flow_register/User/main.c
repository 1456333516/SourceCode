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

// 定义全局变量
uint16_t leds[] = {LED_1, LED_2, LED_3};

// 串口通信的缓冲区、大小和接收完成标志
uint8_t buffer[100];
uint8_t size;
uint8_t isOver;

// 定义保存流水灯方案的二维数组
uint16_t plans[3][2];

// 定义流水灯状态
Plan_Status plan_status = NO_PLAN;

void LoadPlans(uint8_t *str);
void LedFlow(void);
void SavePlans(void);

int main(void)
{
	// 0. 初始化
	LED_Init();
	Key_Init();
	USART_Init();
	M24C02_Init();

	printf("花式流水灯项目启动！\n");

	// 1. 从EEPROM中读取流水灯方案
	printf("正在从 EEPROM 读取流水灯方案...\n");
	M24C02_ReadBytes(0x00, buffer, 11);

	// 2. 解析流水灯方案，放在数组中
	// 用第一个字符判断是否有已保存的方案
	LoadPlans(buffer);

	// 3. 采用中断方式，等待串口下发数据，判断状态控制流水灯开启
	while (1)
	{
		// 3.1 等待串口中断，保存到EEPROM，切换到第一套方案
		if (isOver)
		{
			SavePlans();
		}

		// 3.2 等待按键按下，切换下一套方案

		// 3.3 根据状态，确定当前方案，控制流水灯开启
		LedFlow();
	}
}

// 包装一个函数，从字符串中提取流水灯方案，加载到数组中
void LoadPlans(uint8_t *str)
{
	if (!str[0])
	{
		printf("未解析出流水灯方案！请继续等待串口输入...\n");
		return;
	}

	plan_status = P1;
	
	printf("开始解析流水灯指令...\n");

	// 用sscanf格式化方式提取数据
	sscanf((char *)str, "%d,%d#%d,%d#%d,%d",
		   &plans[0][0], &plans[0][1],
		   &plans[1][0], &plans[1][1],
		   &plans[2][0], &plans[2][1]);

	// 打印输出，转换成对应的 LED 值保存到数组
	for (uint8_t i = 0; i < 3; i++)
	{
		for (uint8_t j = 0; j < 2; j++)
		{
			printf("%d\t", plans[i][j]);
			switch (plans[i][j])
			{
			case 1:
				plans[i][j] = LED_1;
				break;
			case 2:
				plans[i][j] = LED_2;
				break;
			case 3:
				plans[i][j] = LED_3;
				break;
			default:
				break;
			}
		}
		printf("\n");
	}
}

void SavePlans(void)
{
	// 1) 保存到EEPROM
	M24C02_WriteBytes(0x00, buffer, size);

	// 2) 恢复 isOver 和 size
	size = 0;
	isOver = 0;

	// M24C02_ReadBytes(0x00, buffer, 11);
	// printf("buffer: %s\n", buffer);

	// 3) 加载流水灯方案
	LoadPlans(buffer);
}

// 根据状态，控制流水灯开启函数
void LedFlow(void)
{
	switch (plan_status)
	{
	case P1:
		LED_OffAll(leds, 3);
		for (uint8_t i = 0; i < 2; i++)
		{
			LED_On(plans[0][i]);
			Delay_ms(500);
			LED_Off(plans[0][i]);
		}
		break;
	case P2:
		LED_OffAll(leds, 3);
		for (uint8_t i = 0; i < 2; i++)
		{
			LED_On(plans[1][i]);
			Delay_ms(500);
			LED_Off(plans[1][i]);
		}
		break;
	case P3:
		LED_OffAll(leds, 3);
		for (uint8_t i = 0; i < 2; i++)
		{
			LED_On(plans[2][i]);
			Delay_ms(500);
			LED_Off(plans[2][i]);
		}
		break;

	default:
		LED_OffAll(leds, 3);
		break;
	}
}
