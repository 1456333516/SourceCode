/*
 * @Author: wushengran
 * @Date: 2024-05-11 16:30:39
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "dma.h"
#include "delay.h"

// 初始化
void DMA1_Init(void)
{
    // 1. 开启时钟
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 2. 配置 DMA，通道4
    // 2.1 数据传输方向：0 - 从外设发送，1 - 从存储器发送
    DMA1_Channel4->CCR |= DMA_CCR4_DIR;

    // 2.2 传输数据的宽度：01 - 16 bit
    DMA1_Channel4->CCR &= ~DMA_CCR4_PSIZE;
    DMA1_Channel4->CCR |= DMA_CCR4_PSIZE_0;
    DMA1_Channel4->CCR &= ~DMA_CCR4_MSIZE;
    DMA1_Channel4->CCR |= DMA_CCR4_MSIZE_0;

    // 2.3 设置地址自增，串口地址不能自增
    DMA1_Channel4->CCR &= ~DMA_CCR4_PINC;
    DMA1_Channel4->CCR |= DMA_CCR4_MINC;

    // 循环模式
    DMA1_Channel4->CCR |= DMA_CCR4_CIRC;

    // 2.4 开启传输完成中断
    DMA1_Channel4->CCR |= DMA_CCR4_TCIE;

    // 3. 开启串口的DMA传输
    USART1->CR3 |= USART_CR3_DMAT;


    // 4. NVIC 配置
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(DMA1_Channel4_IRQn, 3);
    NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}

// DMA1的通道4传输数据，参数：源地址、目的地址、数据长度
void DMA1_CH4_Transmit(uint32_t srcAddr, uint32_t destAddr, uint8_t dataLen)
{
    // 1. 设置存储器地址寄存器为源地址
    DMA1_Channel4->CMAR = srcAddr;

    // 2. 设置外设地址寄存器为目的地址
    DMA1_Channel4->CPAR = destAddr;

    // 3. 设置要传输的数据长度
    DMA1_Channel4->CNDTR = dataLen;

    // 4. 开启通道传输使能
    DMA1_Channel4->CCR |= DMA_CCR4_EN;
}

// 中断服务程序，数据传输完成
void DMA1_Channel4_IRQHandler(void)
{
    // 判断是否数据传输完成
    if (DMA1->ISR & DMA_ISR_TCIF4)
    {
        // 清除标记位
        DMA1->IFCR |= DMA_IFCR_CTCIF4;

        // 关闭通道，结束传输
        // DMA1_Channel4->CCR &= ~DMA_CCR4_EN;
    }  
}
