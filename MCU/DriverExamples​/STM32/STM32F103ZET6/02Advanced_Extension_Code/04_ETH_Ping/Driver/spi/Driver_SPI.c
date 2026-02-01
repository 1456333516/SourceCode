#include "Driver_SPI.h"



#define CS_H (GPIOD->ODR |= GPIO_ODR_ODR3)
#define CS_L (GPIOD->ODR &= ~GPIO_ODR_ODR3)

#define DELAY

#ifdef DELAY
#define SPI_Delay (Delay_us(1))

#else
#define SPI_Delay  
#endif

void Driver_SPI_Init(void)
{

    /* 用GPIO模拟 */

    /* 1. 开启时钟 */
    RCC->APB1ENR |= (RCC_APB1ENR_SPI2EN );
    RCC->APB2ENR |= (RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPDEN);

    /* 2. 配置已经的模式 */
    /* PB13 CLK 和PB15 MOSI 复用推挽输出 MODE=11 CNF=10 */
    GPIOB->CRH |= (GPIO_CRH_MODE13 | GPIO_CRH_MODE15 | GPIO_CRH_CNF13_1 | GPIO_CRH_CNF15_1);
    GPIOB->CRH &= ~(GPIO_CRH_CNF13_0 | GPIO_CRH_CNF15_0);

    /* PB14 MISO浮空输入 MODE=00 CNF=01*/
    GPIOB->CRH &= ~(GPIO_CRH_MODE14 | GPIO_CRH_CNF14_1);
    GPIOB->CRH |= GPIO_CRH_CNF14_0;

    /* PC13 CS  通用推挽 MODE=11 CNF=00*/
    GPIOD->CRL |= GPIO_CRL_MODE3;
    GPIOD->CRL &= ~GPIO_CRL_CNF3;

    /* 3. 配置SPI外设 */
    /* 3.1 配置模式位SPI模式 */
    SPI2->I2SCFGR &= ~SPI_I2SCFGR_I2SMOD;

    /* 3.2 配置SPI为主模式 */
    SPI2->CR1 |= SPI_CR1_MSTR;

    /* 3.3 NSS禁用硬件管理, 使用软件控制片选 */
    SPI2->CR1 |= SPI_CR1_SSM;
    SPI2->CR1 |= SPI_CR1_SSI; 

    /* 3.4 配置SPI的工作模式0 */
    SPI2->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);

    /* 3.5 配置生成时钟的波特率  36M*/
    SPI2->CR1 &= ~SPI_CR1_BR;

    /* 3.6 设置数据帧格式 8位 */
    SPI2->CR1 &= ~SPI_CR1_DFF;

    /* 3.7 设置高位先行 */
    SPI2->CR1 &= ~SPI_CR1_LSBFIRST;

    /* 3.8 使能SPI外设 */
    SPI2->CR1 |= SPI_CR1_SPE;


    /* 4. 没有操作芯片的时候, CS要拉高 */
    CS_H;

    SPI_Delay;
}

void Driver_SPI_Start(void)
{
    CS_L;
    SPI_Delay;
}

void Driver_SPI_Stop(void)
{
    CS_H;
    SPI_Delay;
}

/* 交换一个字节的数据 */
uint8_t Driver_SPI_SwapByte(uint8_t sByte)
{

    /* 先把要发送的数据写入到数据寄存器 */
    /* 先判断发送缓冲区为空 */
    while((SPI2->SR & SPI_SR_TXE) == 0);

    SPI2->DR = sByte;

    /* 读数据: 先判断接收缓冲区为非空 */
    while((SPI2->SR & SPI_SR_RXNE) == 0);

    return SPI2->DR;
}

void Driver_SPI_SendByte(uint8_t byte)
{
    Driver_SPI_SwapByte(byte);
}

uint8_t Driver_SPI_ReciveByte(void)
{
    return Driver_SPI_SwapByte(0xff);
}
