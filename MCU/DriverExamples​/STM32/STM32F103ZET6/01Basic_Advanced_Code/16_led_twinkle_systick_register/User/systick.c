#include "systick.h"

void SysTick_Init(void)
{
    // 1. 时钟
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;

    // 2. Load，1ms
    SysTick->LOAD = 72000 - 1;

    // 3. 开中断
    SysTick->CTRL |= SysTick_CTRL_TICKINT;

    // 4. 使能
    SysTick->CTRL |= SysTick_CTRL_ENABLE;
}

// 记录中断次数
uint16_t count = 0;

void SysTick_Handler(void)
{
    count++;

    // 到1000次，翻转 LED
    if (count == 1000)
    {
        LED_Toggle(LED_1);
        count = 0;
    }
}
