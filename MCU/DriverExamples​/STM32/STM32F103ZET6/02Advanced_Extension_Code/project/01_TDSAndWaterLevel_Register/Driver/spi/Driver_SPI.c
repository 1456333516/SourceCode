#include "Driver_SPI.h"

#define SCK_H (GPIOA->ODR |= GPIO_ODR_ODR5)
#define SCK_L (GPIOA->ODR &= ~GPIO_ODR_ODR5)

#define MOSI_H (GPIOA->ODR |= GPIO_ODR_ODR7)
#define MOSI_L (GPIOA->ODR &= ~GPIO_ODR_ODR7)

#define MISO_R (GPIOA->IDR & GPIO_IDR_IDR6)

#define CS_H (GPIOC->ODR |= GPIO_ODR_ODR13)
#define CS_L (GPIOC->ODR &= ~GPIO_ODR_ODR13)

#define SPI_DELAY (Delay_ms(1))

void Driver_SPI_Init()
{
    RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN);

    /*
        sck: A5 通用推挽 mode=11 cnf=00
        mosi: A7 通用推挽

        cs: C13 通用推挽

        miso: A6 浮空输入  mode=00 cnf=01
     */
    GPIOA->CRL |= (GPIO_CRL_MODE5 | GPIO_CRL_MODE7 | GPIO_CRL_CNF6_0);
    GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_CNF7 | GPIO_CRL_MODE6 | GPIO_CRL_CNF6_1);

    GPIOC->CRH |= GPIO_CRH_MODE13;
    GPIOC->CRH &= ~GPIO_CRH_CNF13;

    /* 模式0 */
    SCK_L;
    /* cs: 拉高 */
    CS_H;
    SPI_DELAY;
}

void Driver_SPI_Start()
{
    CS_L;
    SPI_DELAY;
}

void Driver_SPI_Stop()
{
    CS_H;
    SPI_DELAY;
}

static uint8_t Driver_SPI_SwapByte(uint8_t byte)
{
    uint8_t rByte = 0;
    for(uint8_t i = 0; i < 8; i++)
    {
        /* 先准备数据: mosi */
        if(byte & 0x80)
        {
            MOSI_H;
        }
        else
        {
            MOSI_L;
        }

        byte <<= 1;
        SPI_DELAY;
        /* 发出数据 拉高时钟 */
        SCK_H;
        SPI_DELAY;
        /* 读取miso上的数据 */
        rByte <<= 1;
        if(MISO_R)
        {
            rByte |= 1;
        }
        SPI_DELAY;
        
        SCK_L;
        SPI_DELAY;
    }

    return rByte;
}

void Driver_SPI_SendByte(uint8_t byte)
{
    Driver_SPI_SwapByte(byte);
}

uint8_t Driver_SPI_ReceiveByte(void)
{
    return Driver_SPI_SwapByte(0xff);
}
