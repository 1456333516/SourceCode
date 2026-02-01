/*
 * @Author: wushengran
 * @Date: 2024-05-14 11:17:35
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "spi.h"

// 初始化
void SPI_Init(void)
{
    // 1. 开启时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // 2. 设置GPIO工作模式
    // 2.1 CS - PC13, 通用推挽输出，MODE = 11，CNF = 00
    GPIOC->CRH |= GPIO_CRH_MODE13;
    GPIOC->CRH &= ~GPIO_CRH_CNF13;
    // 2.2 SCK - PA5, 复用推挽输出，MODE = 11，CNF = 10
    GPIOA->CRL |= GPIO_CRL_MODE5;
    GPIOA->CRL |= GPIO_CRL_CNF5_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF5_0;
    // 2.3 MOSI - PA7, 复用推挽输出，MODE = 11，CNF = 10
    GPIOA->CRL |= GPIO_CRL_MODE7;
    GPIOA->CRL |= GPIO_CRL_CNF7_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF7_0;

    // 2.4 MISO - PA6，浮空输入，MODE = 00，CNF = 01
    GPIOA->CRL &= ~GPIO_CRL_MODE6;
    GPIOA->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOA->CRL |= GPIO_CRL_CNF6_0;

    // 3. SPI1 模块相关配置
    // 3.1 主模式
    SPI1->CR1 |= SPI_CR1_MSTR;

    // 3.2 NSS 必须拉高，软件配置
    SPI1->CR1 |= SPI_CR1_SSM;       // 选择软件配置NSS
    SPI1->CR1 |= SPI_CR1_SSI;       // NSS = 1 
    // SPI1->CR2 &= ~SPI_CR2_SSOE;

    // 3.3 波特率，BR = 001，4分频
    SPI1->CR1 &= ~SPI_CR1_BR;
    SPI1->CR1 |= SPI_CR1_BR_0;

    // 3.4 数据帧格式，默认8位
    SPI1->CR1 &= ~SPI_CR1_DFF;

    // 3.5 配置高位优先
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;

    // 3.6 配置SPI 模式 0
    SPI1->CR1 &= ~SPI_CR1_CPOL;
    SPI1->CR1 &= ~SPI_CR1_CPHA;

    // 3.7 使能 SPI
    SPI1->CR1 |= SPI_CR1_SPE;
}

void SPI_Start(void)
{
    CS_LOW;
}
void SPI_Stop(void)
{
    CS_HIGH;
}

// 主从之间在一个周期内交换一个字节
uint8_t SPI_SwapByte(uint8_t tByte)
{
    // 1. 写入数据，进行发送的流程
    // 1.1 等待发送缓冲区为空
    while ((SPI1->SR & SPI_SR_TXE)== 0)
    {}
    // 1.2 将新的数据写入DR
    SPI1->DR = tByte;

    // 2. 读取数据，进行接收的流程
    // 2.1 等待接收缓冲区非空
    while ((SPI1->SR & SPI_SR_RXNE) == 0)
    {}
    // 2.2 直接将收到的字节返回
    return (uint8_t)(SPI1->DR & 0xff);
}
