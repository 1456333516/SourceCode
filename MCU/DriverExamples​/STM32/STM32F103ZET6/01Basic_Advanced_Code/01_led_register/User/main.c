#include "stm32f10x.h"

int main(void)
{
	// 1. 配置时钟，开启 APB2 中 GPIOA 时钟
	// *(uint32_t *)(0x40021000 + 0x18) = 4;
	// RCC->APB2ENR = 4;
	
	// RCC->APB2ENR |= (1<<2);
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	// 2. 将 PA0 设置为输入模式
	// *(uint32_t *)(0x40010800 + 0x00) = 0x33;
	// *(uint32_t *)(0x40010800 + 0x04) = 0x3;
	
	// GPIOA->CRL = 0x3;
	// GPIOA->CRL &= ~(1<<3);
	// GPIOA->CRL &= ~(1<<2);
	// GPIOA->CRL |= (1<<1);
	// GPIOA->CRL |= (1<<0);
	GPIOA->CRL &= ~GPIO_CRL_CNF0;
	GPIOA->CRL |= GPIO_CRL_MODE0;
	GPIOA->CRL &= ~GPIO_CRL_CNF1;
	GPIOA->CRL |= GPIO_CRL_MODE1;
	GPIOA->CRH &= ~GPIO_CRH_CNF8;
	GPIOA->CRH |= GPIO_CRH_MODE8;
	
	// 3. 向 PA0 输出低电平，点亮 LED1
	// *(uint32_t *)(0x40010800 + 0x0C) = 0xfefc;
	
	// GPIOA->ODR = 0xfffe;
	// GPIOA->ODR &= ~(1<<0);
	GPIOA->ODR &= ~GPIO_ODR_ODR0;
	
	GPIOA->ODR &= ~GPIO_ODR_ODR1;
	GPIOA->ODR &= ~GPIO_ODR_ODR8;
	
	// 4. 用死循环保持点亮状态
	while(1){}
}
