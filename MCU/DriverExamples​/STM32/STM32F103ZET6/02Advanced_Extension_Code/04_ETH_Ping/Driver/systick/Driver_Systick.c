#include "Driver_Systick.h"

/* 初始化滴答定时器
    1. 每1ms产生一次中断
 */
void Driver_Systick_Init(void)
{
    /* 1. 配置时钟源: 1=AHB 0=AHB/8 */
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;

    /* 2. 使能中断 */
    SysTick->CTRL |= SysTick_CTRL_TICKINT;

    /* 3. 设置重装载值寄存器 */
    SysTick->LOAD = 72000 - 1;

    /* 4 . 使能定时器 */
    SysTick->CTRL |= SysTick_CTRL_ENABLE;
}


uint32_t count = 0;
/* 中断服务函数 */
void SysTick_Handler(void)
{
    count++;
    if(count == 1000)
    {
        count = 0;
        /*翻转led */
        //printf("1s\r\n");
        Drviver_LED_Toggle(LED_3);
    }
}
