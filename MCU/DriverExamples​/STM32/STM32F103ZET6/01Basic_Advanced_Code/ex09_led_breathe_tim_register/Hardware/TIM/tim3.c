/*
 * @Author: wushengran
 * @Date: 2024-05-08 08:32:39
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "tim3.h"

// 初始化
void TIM3_Init(void)
{
    // 1. 开启时钟
    // 1.1 开启 TIM3 时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    // 1.2 开启 GPIOB 时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // 2. 设置 GPIO 工作模式
    // PB0 复用推挽输出，MODE = 11，CNF = 10
    GPIOB->CRL |= GPIO_CRL_MODE0;
    GPIOB->CRL |= GPIO_CRL_CNF0_1;
    GPIOB->CRL &= ~GPIO_CRL_CNF0_0;

    // 3. 定时器配置
    // 3.1 时基部分
    // 3.1.1 设置预分频值 7199，表示 7200 分频，频率 10000Hz
    TIM3->PSC = 7199;

    // 3.1.2 设置自动重装载值 99，计数 100 次溢出
    TIM3->ARR = 99;

    // 3.1.3 计数器计数方向
    TIM3->CR1 &= ~TIM_CR1_DIR;

    // 3.2 输出比较模块
    // 3.2.1 选择通道3的工作模式，输出
    TIM3->CCMR2 &= ~TIM_CCMR2_CC3S;

    // 3.2.2 配置通道3的输出比较模式为 PWM1 模式，110
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_2;
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_1;
    TIM3->CCMR2 &= ~TIM_CCMR2_OC3M_0;

    // 3.2.3 配置通道3的输出极性
    TIM3->CCER &= ~TIM_CCER_CC3P;

    // 3.2.4 使能通道3
    TIM3->CCER |= TIM_CCER_CC3E;
}

// 将开关包装成函数
void TIM3_Start(void)
{
    TIM3->CR1 |= TIM_CR1_CEN;
}
void TIM3_Stop(void)
{
    TIM3->CR1 &= ~TIM_CR1_CEN;
}

// 设置占空比的函数
void TIM3_SetDutyCycle(uint8_t dutyCycle, uint8_t ch)
{
    switch (ch)
    {
        // 更改 CCR 的值
    case 1:
        TIM3->CCR1 = dutyCycle;
        break;
    case 2:
        TIM3->CCR2 = dutyCycle;
        break;
    case 3:
        TIM3->CCR3 = dutyCycle;
        break;

    default:
        break;
    }
}
