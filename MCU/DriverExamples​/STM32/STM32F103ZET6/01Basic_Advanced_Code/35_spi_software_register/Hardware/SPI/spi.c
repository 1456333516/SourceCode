#include "spi.h"

// 初始化
void SPI_Init(void)
{
    // 1. 开启时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    // 2. 设置GPIO工作模式
    // 2.1 CS - PC13, 通用推挽输出，MODE = 11，CNF = 00
    GPIOC->CRH |= GPIO_CRH_MODE13;
    GPIOC->CRH &= ~GPIO_CRH_CNF13;
    // 2.2 SCK - PA5, 通用推挽输出，MODE = 11，CNF = 00
    GPIOA->CRL |= GPIO_CRL_MODE5;
    GPIOA->CRL &= ~GPIO_CRL_CNF5;
    // 2.3 MOSI - PA7, 通用推挽输出，MODE = 11，CNF = 00
    GPIOA->CRL |= GPIO_CRL_MODE7;
    GPIOA->CRL &= ~GPIO_CRL_CNF7;

    // 2.4 MISO - PA6，浮空输入，MODE = 00，CNF = 01
    GPIOA->CRL &= ~GPIO_CRL_MODE6;
    GPIOA->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOA->CRL |= GPIO_CRL_CNF6_0;

    // 3. 默认片选不生效
    CS_HIGH;

    // 4. 模式 0，空闲状态时钟是 0
    SCK_LOW;

    // 5. 延时
    SPI_DELAY;
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
    // 定义一个变量，保存接收到的字节
    uint8_t rByte = 0x00;

    // 用一个循环，依次读取/发送 8 位数据
    for (uint8_t i = 0; i < 8; i++)
    {
        // 1. 发送数据：判断 tByte 最高位
        if (tByte & 0x80)
        {
            MOSI_HIGH; // 如果为1，就拉高MOSI
        }
        else
        {
            MOSI_LOW;
        }
        tByte <<= 1; // 左移一位

        // 2. 拉高时钟，采样数据
        SCK_HIGH;
        SPI_DELAY;

        // 3. 在 MISO 上读取传来的数据
        rByte <<= 1; // 先左移1位，空出最后一位接收数据
        if (MISO_READ)
        {
            rByte |= 0x01; // 如果读取高电平，就保存一位1
        }

        // 4. 拉低时钟，为下一次采样做准备
        SCK_LOW;
        SPI_DELAY;
    }

    return rByte;
}
