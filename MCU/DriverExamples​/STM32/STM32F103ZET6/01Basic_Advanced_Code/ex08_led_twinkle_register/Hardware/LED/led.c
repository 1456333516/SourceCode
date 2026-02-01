#include "led.h"

void LED_Init(void)
{
    // 1. 时钟配置
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	// 2. 工作模式
	GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1);
	GPIOA->CRL |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1);

	GPIOA->CRH &= ~GPIO_CRH_CNF8;
	GPIOA->CRH |= GPIO_CRH_MODE8;
	
	// 3. 对应引脚输出高电平，默认关灯
	LED_Off(LED_1);
    LED_Off(LED_2);
    LED_Off(LED_3);
}

// 开关某个LED灯
void LED_On(uint16_t led)
{
    GPIOA->ODR &= ~led;
}
void LED_Off(uint16_t led)
{
    GPIOA->ODR |= led;
}

// 翻转某个LED灯状态
void LED_Toggle(uint16_t led)
{
    // 先判断IDR中对应的引脚状态，然后根据状态开关灯
    if ( (GPIOA->IDR & led) == 0)
    {
        LED_Off(led);
    }
    else
    {
        LED_On(led);
    }
    
}

// 全开/全关
void LED_OnAll(uint16_t leds[], uint8_t size)
{
    // 循环遍历所有LED灯
    for (uint8_t i = 0; i < size; i++)
    {
        LED_On(leds[i]);
    }
    
}

void LED_OffAll(uint16_t leds[], uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        LED_Off(leds[i]);
    }
}
