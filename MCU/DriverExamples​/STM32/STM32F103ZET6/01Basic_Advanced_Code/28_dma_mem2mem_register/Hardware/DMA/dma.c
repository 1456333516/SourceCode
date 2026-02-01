/*
 * @Author: wushengran
 * @Date: 2024-05-11 11:21:10
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "dma.h"

// 初始化
void DMA1_Init(void)
{
    // 1. 开启时钟
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 2. 配置 DMA
    // 2.1 数据传输方向：0 - 从外设发送，1 - 从存储器发送
    DMA1_Channel1->CCR |= DMA_CCR1_DIR;
    DMA1_Channel1->CCR |= DMA_CCR1_MEM2MEM;

    // 2.2 传输数据的宽度：00 - 8 bit
    DMA1_Channel1->CCR &= ~DMA_CCR1_PSIZE;
    DMA1_Channel1->CCR &= ~DMA_CCR1_MSIZE;

    // 2.3 设置地址自增
    DMA1_Channel1->CCR |= DMA_CCR1_PINC;
    DMA1_Channel1->CCR |= DMA_CCR1_MINC;

    // 2.4 开启传输完成中断
    DMA1_Channel1->CCR |= DMA_CCR1_TCIE;

    // 3. NVIC 配置
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(DMA1_Channel1_IRQn, 3);
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

// DMA1的通道1传输数据，参数：源地址、目的地址、数据长度
void DMA1_CH1_Transmit(uint32_t srcAddr, uint32_t destAddr, uint8_t dataLen)
{
    // 1. 设置外设地址寄存器为源地址
    DMA1_Channel1->CPAR = destAddr;

    // 2. 设置存储器地址寄存器为目的地址
    DMA1_Channel1->CMAR = srcAddr;

    // 3. 设置要传输的数据长度
    DMA1_Channel1->CNDTR = dataLen;

    // 4. 开启通道传输使能
    DMA1_Channel1->CCR |= DMA_CCR1_EN;
}

// 中断服务程序，数据传输完成
void DMA1_Channel1_IRQHandler(void)
{
    // 判断是否数据传输完成
    if (DMA1->ISR & DMA_ISR_TCIF1)
    {
        // 清除标记位
        DMA1->IFCR |= DMA_IFCR_CTCIF1;

        // 关闭通道，结束传输
        DMA1_Channel1->CCR &= ~DMA_CCR1_EN;

        // 标记位置位
        isFinished = 1;
    }  
}
