#include "Driver_I2C2.h"

void Driver_I2C2_Init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    /*
        pb10: scl   复用开漏  mode=11 cnf=11
        pb11: sda   复用开漏
     */
    GPIOB->CRH |= (GPIO_CRH_MODE10 | GPIO_CRH_MODE11 | GPIO_CRH_CNF10 | GPIO_CRH_CNF11);

    /* 1. 工作模式 */
    I2C2->CR1 &= ~I2C_CR1_SMBUS;

    /* 2. i2c的时钟频率 */
    I2C2->CR2 &= ~I2C_CR2_FREQ;
    I2C2->CR2 |= 36 << 0;

    /* 2. 速率 100k */
    I2C2->CCR &= ~I2C_CCR_FS;

    /* 3. 配置ccr
        100k=>10us
        Thigh = CCR ×TPCLK1
          5us = ccr * (1/36)us
          ccr = 36 * 5 = 180
    */
    I2C2->CCR |= 180 << 0;

    I2C2->TRISE = 37;

    I2C2->CR1 |= I2C_CR1_PE;
}

uint8_t Driver_I2C2_Start(void)
{
    I2C2->CR1 |= I2C_CR1_START;
    uint32_t timeout = 0xffffff;
    while((I2C2->SR1 & I2C_SR1_SB) == 0 && timeout--);

    return timeout ? OK : FAIL;
}

void Driver_I2C2_Stop(void)
{
    I2C2->CR1 |= I2C_CR1_STOP;
}

void Driver_I2C2_Ack(void)
{
    I2C2->CR1 |= I2C_CR1_ACK;
}

void Driver_I2C2_NoAck(void)
{
    I2C2->CR1 &= ~I2C_CR1_ACK;
}
#include "Common_Debug.h"
uint8_t Driver_I2C2_SendAddr(uint8_t addr)
{
    uint32_t timeout = 0xffff;
    //while((I2C2->SR1 & I2C_SR1_TXE) == 0 && timeout--);  // 发送地址阶段不会对这个位置1 所以不用判断的等待
    I2C2->DR = addr;

    timeout = 0xffff;
    while((I2C2->SR1 & I2C_SR1_ADDR) == 0 && timeout--);

    I2C2->SR2;

    return timeout ? OK : FAIL;
}

uint8_t Driver_I2C2_SendByte(uint8_t byte)
{
    uint32_t timeout = 0xffff;
    while((I2C2->SR1 & I2C_SR1_TXE) == 0 && timeout--);
    I2C2->DR = byte;

    timeout = 0xffff;
    while((I2C2->SR1 & I2C_SR1_BTF) == 0 && timeout--);

    I2C2->DR;  /* 清除 I2C_SR1_BTF*/
    return timeout ? OK : FAIL;
}

uint8_t Driver_I2C2_ReadByte(void)
{
    uint32_t timeout = 0xffff;
    while((I2C2->SR1 & I2C_SR1_RXNE) == 0 && timeout--);

    return timeout ? I2C2->DR : 0;
}


