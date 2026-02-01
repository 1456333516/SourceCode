/*
 * @Author: wushengran
 * @Date: 2024-05-09 10:31:13
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "tim5.h"

// 初始化
void TIM5_Init(void)
{
    // 1. 开启时钟
    // 1.1 开启 TIM5 时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    // 1.2 开启 GPIOA 时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 2. 设置 GPIO 工作模式，PA1 复用推挽输出，MODE = 11，CNF = 10
    GPIOA->CRL |= GPIO_CRL_MODE1;
    GPIOA->CRL |= GPIO_CRL_CNF1_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF1_0;

    // 3. 定时器配置
    // 3.1 时基部分
    // 3.1.1 设置预分频值 7199，表示 7200 分频，频率 10000Hz
    TIM5->PSC = 7199;

    // 3.1.2 设置自动重装载值 99，计数 100 次溢出
    TIM5->ARR = 99;

    // 3.1.3 计数器计数方向
    TIM5->CR1 &= ~TIM_CR1_DIR;

    // 3.2 输出比较模块
    // 3.2.1 选择通道2的工作模式，输出
    TIM5->CCMR1 &= ~TIM_CCMR1_CC2S;

    // 3.2.2 配置通道2的输出比较模式为 PWM1 模式，110
    TIM5->CCMR1 |= TIM_CCMR1_OC2M_2;
    TIM5->CCMR1 |= TIM_CCMR1_OC2M_1;
    TIM5->CCMR1 &= ~TIM_CCMR1_OC2M_0;

    // 3.2.3 配置通道2的输出极性
    TIM5->CCER &= ~TIM_CCER_CC2P;

    // 3.2.4 使能通道2
    TIM5->CCER |= TIM_CCER_CC2E;

}

// 将开关包装成函数
void TIM5_Start(void)
{
    TIM5->CR1 |= TIM_CR1_CEN;
}
void TIM5_Stop(void)
{
    TIM5->CR1 &= ~TIM_CR1_CEN;
}

// 设置占空比的函数
void TIM5_SetDutyCycle(uint8_t dutyCycle)
{
    // 更改 CCR 的值
    TIM5->CCR2 = dutyCycle;
}
