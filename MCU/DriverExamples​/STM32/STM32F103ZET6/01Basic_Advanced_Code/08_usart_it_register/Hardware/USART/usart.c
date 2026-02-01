/*
 * @Author: wushengran
 * @Date: 2024-04-28 11:28:19
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "usart.h"

void USART_Init(void)
{
    // 1. 配置时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // 2. GPIO引脚配置
    // 2.1 PA9 设为复用推挽输出 MODE: 11, CNF: 10
    GPIOA->CRH |= GPIO_CRH_MODE9;
    GPIOA->CRH |= GPIO_CRH_CNF9_1;
    GPIOA->CRH &= ~GPIO_CRH_CNF9_0;

    // 2.2 PA10 设为浮空输入 MODE：00， CNF：01
    GPIOA->CRH &= ~GPIO_CRH_MODE10;
    GPIOA->CRH &= ~GPIO_CRH_CNF10_1;
    GPIOA->CRH |= GPIO_CRH_CNF10_0;

    // 3. 串口配置
    // 3.1 设置波特率
    USART1->BRR = 0x271;

    // 3.2 串口使能和收发使能
    USART1->CR1 |= (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);

    // 3.3 中断使能
    USART1->CR1 |= (USART_CR1_IDLEIE | USART_CR1_RXNEIE);

    // 4. 配置NVIC
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(USART1_IRQn, 2);
    NVIC_EnableIRQ(USART1_IRQn);
}

// 发送一个字符
void SendChar(uint8_t ch)
{
    while ((USART1->SR & USART_SR_TXE) == 0)
    {
    }

    // 准备就绪，继续发送下一个字符
    USART1->DR = ch;
}


// 发送一个字符串
void SendString(uint8_t *str, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        SendChar(str[i]);
    }
}


// 中断处理程序，接收字符串
void USART1_IRQHandler(void)
{
    // 判断状态寄存器中的标志位，确定是那种中断类型
    // 1. 如果是RXNE，说明已经接收到了一个字符，就读取DR保存起来
    if (USART1->SR & USART_SR_RXNE)
    {
        buffer[size] = USART1->DR;
        size++;
    }
    // 2. 如果是IDLE，说明字符串已经接收完毕，用一个标志位通知主函数
    else if (USART1->SR & USART_SR_IDLE)
    {
        isOver = 1;
        // 清除IDLE标志位
        // USART1->SR;
        USART1->DR;
    }
}
