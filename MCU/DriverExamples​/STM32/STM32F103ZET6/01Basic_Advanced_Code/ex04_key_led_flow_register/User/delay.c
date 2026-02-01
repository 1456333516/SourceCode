/*
 * @Author: wushengran
 * @Date: 2024-04-27 10:01:46
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "delay.h"
#include "key.h"

void Delay_us(uint16_t us)
{
	// 装载计数值
	SysTick->LOAD = 72 * us;

	// 配置系统定时器
	SysTick->CTRL = 0x5;

	// 等待计数值变为0
	while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG)
		// && (SysTick->CTRL & SysTick_CTRL_ENABLE)
		&& isOn
		)
	{
	}

	// 关闭定时器
	// SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
}

void Delay_ms(uint16_t ms)
{
	while (ms--)
	{
		Delay_us(1000);
	}
}

void Delay_s(uint16_t s)
{
	while (s--)
	{
		Delay_ms(1000);
	}
}
