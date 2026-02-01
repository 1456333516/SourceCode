// /*
//  * @Author: wushengran
//  * @Date: 2024-04-29 14:42:27
//  * @Description: 
//  * 
//  * Copyright (c) 2024 by atguigu, All Rights Reserved. 
//  */
#include "i2c.h"

void I2C_Init(void)
{
    // 1. 配置时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    // 2. GPIO配置
    // PB10、PB11：复用开漏输出，MODE：11，CNF：11
    GPIOB->CRH |= (GPIO_CRH_MODE10 | GPIO_CRH_MODE11 | GPIO_CRH_CNF10 | GPIO_CRH_CNF11);

    // 3. I2C配置
    // 3.1 硬件工作模式配置
    I2C2->CR1 &= ~I2C_CR1_SMBUS;
    I2C2->CCR &= ~I2C_CCR_FS;

    // 3.2 时钟频率
    I2C2->CR2 |= 36;

    // 3.3 设置SCL时钟分频系数，达到100kHz
    I2C2->CCR |= 180;

    // 3.4 设置上升沿占用时钟周期数
    I2C2->TRISE = 37;

    // 3.5 使能I2C模块
    I2C2->CR1 |= I2C_CR1_PE;
}

// 发出起始信号
uint8_t I2C_Start(void)
{
    // 产生Start信号
    I2C2->CR1 |= I2C_CR1_START;

    // 等待Start信号发出
    uint16_t timeout = 0xffff;
    while ((I2C2->SR1 & I2C_SR1_SB) == 0 && timeout)
    {
        timeout--;
    }
    return timeout ? OK : FAIL;
}

// 发出结束信号
void I2C_Stop(void)
{
    I2C2->CR1 |= I2C_CR1_STOP;
}

// 主机发送应答
void I2C_Ack(void)
{
    I2C2->CR1 |= I2C_CR1_ACK;
}

// 主机发送非应答
void I2C_NACK(void)
{
    I2C2->CR1 &= ~I2C_CR1_ACK;
}

// 发送一个设备地址
uint8_t I2C_SendAddr(uint8_t addr)
{
    // 因为是Start之后的第一步操作，所以不需要判断DR是否为空
    I2C2->DR = addr;

    // 等待接收到ACK
    uint16_t timeout = 0xffff;
    while ((I2C2->SR1 & I2C_SR1_ADDR) == 0 && timeout)
    {
        timeout--;
    }

    // 判断当前是否超时，如果未超时就清除 ADDR 位
    if (timeout > 0)
    {
        I2C2->SR2;
    }  

    return timeout ? OK : FAIL;
}


// 发送一个字节（向EEPROM写入数据）
uint8_t I2C_SendByte(uint8_t byte)
{
    // 需要判断DR是否为空，等待上一个字节移出DR
    uint16_t timeout = 0xffff;
    while ((I2C2->SR1 & I2C_SR1_TXE) == 0 && timeout)
    {
        timeout --;
    }

    I2C2->DR = byte;

    // 等待接收到ACK
    timeout = 0xffff;
    while ((I2C2->SR1 & I2C_SR1_BTF) == 0 && timeout)
    {
        timeout--;
    }

    return timeout ? OK : FAIL; 
}

// 接收一个字节（从EEPROM读取数据）
uint8_t I2C_ReadByte(void)
{
    // 需要判断DR是否非空，等待接收到字节
    uint16_t timeout = 0xffff;
    while ((I2C2->SR1 & I2C_SR1_RXNE) == 0 && timeout)
    {
        timeout --;
    }

    return timeout ? I2C2->DR : FAIL;
}
