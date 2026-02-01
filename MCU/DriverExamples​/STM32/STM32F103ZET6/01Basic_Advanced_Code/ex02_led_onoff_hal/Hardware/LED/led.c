#include "led.h"

// 开关某个LED灯
void LED_On(LED led)
{
    HAL_GPIO_WritePin(GPIOA, led, GPIO_PIN_RESET);
}
void LED_Off(LED led)
{
    HAL_GPIO_WritePin(GPIOA, led, GPIO_PIN_SET);
}

// 翻转某个LED灯状态
void LED_Toggle(LED led)
{
    // 先判断IDR中对应的引脚状态，然后根据状态开关灯
    HAL_GPIO_TogglePin(GPIOA, led);
}

// 全开/全关
void LED_OnAll(LED leds[], uint8_t size)
{
    // 循环遍历所有LED灯
    for (uint8_t i = 0; i < size; i++)
    {
        LED_On(leds[i]);
    }  
}

void LED_OffAll(LED leds[], uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        LED_Off(leds[i]);
    }
}
