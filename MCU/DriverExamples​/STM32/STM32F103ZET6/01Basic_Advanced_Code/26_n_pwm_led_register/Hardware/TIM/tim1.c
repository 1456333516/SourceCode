/*
 * @Author: wushengran
 * @Date: 2024-05-09 15:18:21
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "tim1.h"

// 初始化
void TIM1_Init(void)
{
    // 1. 开启时钟
    // 1.1 开启 TIM1 时钟
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    // 1.2 开启 GPIOB 时钟
    // RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // 2. 设置 GPIO 工作模式，PA8 复用推挽输出，MODE = 11，CNF = 10
    GPIOA->CRH |= GPIO_CRH_MODE8;
    GPIOA->CRH |= GPIO_CRH_CNF8_1;
    GPIOA->CRH &= ~GPIO_CRH_CNF8_0;

    // 3. 定时器配置
    // 3.1 时基部分
    // 3.1.1 设置预分频值 7199，表示 7200 分频，频率 10000Hz，100us一次计数
    TIM1->PSC = 7199;

    // 3.1.2 设置自动重装载值 4999，计数 5000 次溢出，500ms，2Hz
    TIM1->ARR = 4999;

    // 3.1.3 计数器计数方向
    TIM1->CR1 &= ~TIM_CR1_DIR;

    // 3.1.4 设置重复计数值
    TIM1->RCR = 4;

    // 3.2 输出比较模块
    // 3.2.1 通道1配置为输出模式
    TIM1->CCMR1 &= ~ TIM_CCMR1_CC1S;

    // 3.2.2 配置为 PWM 模式1 ，OC1M = 110
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_2;
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_1;
    TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_0;

    // 3.2.3 设置 CCR 占空比
    TIM1->CCR1 = 2500;

    // 3.2.4 通道1使能
    TIM1->CCER |= TIM_CCER_CC1E;

    // 4. 主输出使能
    TIM1->BDTR |= TIM_BDTR_MOE;

    // // 5. 为了防止一启动就进入中断关闭定时器，主动先产生一个更新事件
    TIM1->EGR |= TIM_EGR_UG;

    // 6. 开启更新中断
    TIM1->DIER |= TIM_DIER_UIE;

    // 7. NVIC配置
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM1_UP_IRQn, 3);
    NVIC_EnableIRQ(TIM1_UP_IRQn);

}

// 将开关包装成函数
void TIM1_Start(void)
{
    TIM1->CR1 |= TIM_CR1_CEN;
}
void TIM1_Stop(void)
{
    TIM1->CR1 &= ~TIM_CR1_CEN;
}

// 更新中断的中断服务程序
void TIM1_UP_IRQHandler(void)
{
    // 清除中断标记
    TIM1->SR &= ~TIM_SR_UIF;

    // 关闭定时器
    TIM1_Stop();
}
