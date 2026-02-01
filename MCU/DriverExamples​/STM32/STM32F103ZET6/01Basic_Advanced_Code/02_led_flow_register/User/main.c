/*
 * @Author: wushengran
 * @Date: 2024-04-23 16:49:27
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "stm32f10x.h"

void Delay_s(uint16_t s);
void Delay_ms(uint16_t ms);
void Delay_us(uint16_t us);

int main(void)
{
	// 1. 时钟配置
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	// 2. 工作模式
	GPIOA->CRL &= ~GPIO_CRL_CNF0;
	GPIOA->CRL |= GPIO_CRL_MODE0;
	GPIOA->CRL &= ~GPIO_CRL_CNF1;
	GPIOA->CRL |= GPIO_CRL_MODE1;
	GPIOA->CRH &= ~GPIO_CRH_CNF8;
	GPIOA->CRH |= GPIO_CRH_MODE8;
	
	// 3. 对应引脚输出高电平，默认关灯
	GPIOA->ODR |= GPIO_ODR_ODR0;
	
	GPIOA->ODR |= GPIO_ODR_ODR1;
	GPIOA->ODR |= GPIO_ODR_ODR8;
	
	// 4. 在死循环中依次点亮每个灯
	while(1)
	{
		// 点亮 LED1
		GPIOA->ODR &= ~GPIO_ODR_ODR0;
		// 延时
		Delay_ms(500);
		// 关闭 LED1
		GPIOA->ODR |= GPIO_ODR_ODR0;
		
		// 点亮 LED2
		GPIOA->ODR &= ~GPIO_ODR_ODR1;
		// 延时
		Delay_ms(500);
		// 关闭 LED2
		GPIOA->ODR |= GPIO_ODR_ODR1;

		// 点亮 LED3
		GPIOA->ODR &= ~GPIO_ODR_ODR8;
		// 延时
		Delay_ms(500);
		// 关闭 LED3
		GPIOA->ODR |= GPIO_ODR_ODR8;
	}
}

void Delay_us(uint16_t us)
{
	// 装载计数值
	SysTick->LOAD = 72 * us;

	// 配置系统定时器
	SysTick->CTRL = 0x5;

	// 等待计数值变为0
	while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG))
	{
	}

	// 关闭定时器
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
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
