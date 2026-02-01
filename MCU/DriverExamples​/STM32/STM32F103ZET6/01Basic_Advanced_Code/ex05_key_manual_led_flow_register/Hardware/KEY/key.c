/*
 * @Author: wushengran
 * @Date: 2024-04-27 11:12:55
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "key.h"
#include "led.h"
#include "delay.h"

void Key_Init(void)
{
    // 1. 配置时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    // 2. GPIO配置
    // 配置PF10为下拉输入模式（CNF=10，MODE=00），默认低电平
    GPIOF->CRH &= ~GPIO_CRH_MODE10;
    GPIOF->CRH |= GPIO_CRH_CNF10_1;
    GPIOF->CRH &= ~GPIO_CRH_CNF10_0;

    GPIOF->ODR &= ~GPIO_ODR_ODR10;

    // 3. AFIO复用选择配置
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI10_PF;

    // 4. EXTI配置
    EXTI->RTSR |= EXTI_RTSR_TR10;
    EXTI->IMR |= EXTI_IMR_MR10;

    // 5. NVIC配置
    // 配置优先级组方案以及中断的优先级
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(EXTI15_10_IRQn, 2);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

// 中断处理程序
void EXTI15_10_IRQHandler(void)
{
    // 进入中断处理，清除挂起寄存器中的标志
    EXTI->PR |= EXTI_PR_PR10;

    // 用延时消抖
    Delay_ms(50);

    // 判断确认当前一直保持高电平，就根据之前状态进行状态转移
    if ( (GPIOF->IDR & GPIO_IDR_IDR10) != 0 )
    {
        led_status = (LED_Status)((led_status + 1) % 3);
    }
}
