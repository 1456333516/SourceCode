/*
 * @Author: wushengran
 * @Date: 2024-05-09 10:31:13
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "tim4.h"

// 初始化
void TIM4_Init(void)
{
    // 1. 开启时钟
    // 1.1 开启 TIM4 时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    // 1.2 开启 GPIOB 时钟
    // RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // 2. 设置 GPIO 工作模式，PB6 浮空输入，MODE = 00，CNF = 01
    GPIOB->CRL &= ~GPIO_CRL_MODE6;
    GPIOB->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOB->CRL |= GPIO_CRL_CNF6_0;

    // 3. 定时器配置
    // 3.1 时基部分
    // 3.1.1 设置预分频值 71，表示 72 分频，频率 1MHz
    TIM4->PSC = 71;

    // 3.1.2 设置自动重装载值 65535，最大
    TIM4->ARR = 65535;

    // 3.1.3 计数器计数方向
    TIM4->CR1 &= ~TIM_CR1_DIR;

    // 3.2 输入捕获模块
    // 3.2.1 通道1是否开启异或门
    TIM4->CR2 &= ~TIM_CR2_TI1S;

    // 3.2.2 通道1的滤波器
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;

    // 3.2.3 通道1的边沿检测器
    TIM4->CCER &= ~TIM_CCER_CC1P;
    // 通道2的边沿检测器
    TIM4->CCER |= TIM_CCER_CC2P;

    // 3.2.4 配置最终输入信号IC1，映射到TI1上，CC1S = 01
    TIM4->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;

    // 配置最终输入信号IC2，映射到TI1上，CC2S = 10
    TIM4->CCMR1 |= TIM_CCMR1_CC2S_1;
    TIM4->CCMR1 &= ~TIM_CCMR1_CC2S_0;

    // 3.2.5 IC1配置预分频系数
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1PSC;
    TIM4->CCMR1 &= ~TIM_CCMR1_IC2PSC;

    // 3.2.6 使能输入捕获
    TIM4->CCER |= TIM_CCER_CC1E;
    TIM4->CCER |= TIM_CCER_CC2E;

    // 3.2.7 选择触发输入信号，TS = 101
    TIM4->SMCR |= TIM_SMCR_TS_2;
    TIM4->SMCR &= ~TIM_SMCR_TS_1;
    TIM4->SMCR |= TIM_SMCR_TS_0;

    // 3.2.8 设置从模式：复位模式 100
    TIM4->SMCR |= TIM_SMCR_SMS_2;
    TIM4->SMCR &= ~TIM_SMCR_SMS_1;
    TIM4->SMCR &= ~TIM_SMCR_SMS_0;
}

// 将开关包装成函数
void TIM4_Start(void)
{
    TIM4->CR1 |= TIM_CR1_CEN;
}
void TIM4_Stop(void)
{
    TIM4->CR1 &= ~TIM_CR1_CEN;
}

// 获取PWM周期和频率，ms为单位
double TIM4_GetPWMCycle(void)
{
    return TIM4->CCR1 / 1000.0;
}
double TIM4_GetPWMFreq(void)
{
    return 1000000.0 / TIM4->CCR1;
}

double TIM4_GetDutyCycle(void)
{
    return TIM4->CCR2 * 1.0 / TIM4->CCR1;
}
