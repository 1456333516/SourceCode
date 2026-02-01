/*
 * @Author: wushengran
 * @Date: 2024-05-07 08:34:36
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "tim6.h"
#include "led.h"

void TIM6_Init(void)
{
    // 1. 开启时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    // 2. 设置预分频值 7199，表示 7200 分频，计数频率 10000Hz
    TIM6->PSC = 7199;

    // 3. 设置自动重装载值 9999，表示计数 10000 次产生一个更新事件
    TIM6->ARR = 9999;

    // 4. 开启更新中断
    TIM6->DIER |= TIM_DIER_UIE;

    // 5. 配置 NVIC
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM6_IRQn, 3);
    NVIC_EnableIRQ(TIM6_IRQn);

    // 6. 开启计数器
    TIM6->CR1 |= TIM_CR1_CEN;
}

// 中断服务程序
void TIM6_IRQHandler(void)
{
    printf("TIM6 中断！\n");
    // 清除中断标记位
    TIM6->SR &= ~TIM_SR_UIF;

    // 翻转LED2
    LED_Toggle(LED_2);
}
