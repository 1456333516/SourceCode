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

    // 2. GPIO配置
    // PB10、PB11：通用开漏输出，MODE：11，CNF：01
    GPIOB->CRH |= (GPIO_CRH_MODE10 | GPIO_CRH_MODE11);
    GPIOB->CRH &= ~(GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_1);
    GPIOB->CRH |= (GPIO_CRH_CNF10_0 | GPIO_CRH_CNF11_0);
}

// 发出起始信号
void I2C_Start(void)
{
    // 1. SCL、SDA拉高
    SCL_HIGH;
    SDA_HIGH;
    I2C_Delay;

    // 2. SDA向下跳变
    SDA_LOW;
    I2C_Delay;
}

// 发出结束信号
void I2C_Stop(void)
{
    // 1. SCL拉高、SDA拉低
    SCL_HIGH;
    SDA_LOW;
    I2C_Delay;

    // 2. SDA向上跳变
    SDA_HIGH;
    I2C_Delay;
}

// 主机发送应答
void I2C_Ack(void)
{
    // 1. SDA拉高，SCL拉低，准备翻转数据
    SDA_HIGH;
    SCL_LOW;
    I2C_Delay;

    // 2. 保持SCL，SDA拉低，输出ACK
    SDA_LOW;
    I2C_Delay;

    // 3. 保持SDA，SCL拉高，时钟采样数据
    SCL_HIGH;
    I2C_Delay;

    // 4. 保持SDA，SCL拉低，时钟采样结束，检测到一位 0
    SCL_LOW;
    I2C_Delay;

    // 5. SDA拉高，释放数据总线
    SDA_HIGH;
    I2C_Delay;
}

// 主机发送非应答
void I2C_NACK(void)
{
    // 1. SDA拉高，SCL拉低，准备翻转数据
    SDA_HIGH;
    SCL_LOW;
    I2C_Delay;

    // 2 保持SDA，SCL拉高，时钟采样数据
    SCL_HIGH;
    I2C_Delay;

    // 3. 保持SDA，SCL拉低，时钟采样结束，检测到一位 0
    SCL_LOW;
    I2C_Delay;
}

// 主机等待从设备发送应答
uint8_t I2C_Wait4Ack(void)
{
    // 1. SDA拉高，SCL拉低，释放数据总线，等待应答信号翻转电平
    SDA_HIGH;
    SCL_LOW;
    I2C_Delay;

    // 2. SCL拉高，开始采样
    SCL_HIGH;
    I2C_Delay;

    // 3. 读取SDA的电平
    uint8_t ack = READ_SDA;

    // 4. SCL拉低，采样结束
    SCL_LOW;
    I2C_Delay;

    return ack;
}

// 发送一个字节（向EEPROM写入数据）
void I2C_SendByte(uint8_t byte)
{
    // 通过循环遍历每一位进行发送
    for (uint8_t i = 0; i < 8; i++)
    {
        // 1. SCL、SDA拉低，等待数据翻转
        SDA_LOW;
        SCL_LOW;
        I2C_Delay;

        // 2. 取字节的最高位，MSB作为当前电平信号发出
        if (byte & 0x80)   
        {
            SDA_HIGH;
        }
        else
        {
            SDA_LOW;
        }
        I2C_Delay;

        // 3. SCL拉高，开始采样
        SCL_HIGH;
        I2C_Delay;

        // 4. SCL拉低，结束采样
        SCL_LOW;
        I2C_Delay;

        // byte 需要左移一位，继续判断
        byte <<= 1;     
    }
    
}

// 接收一个字节（从EEPROM读取数据）
uint8_t I2C_ReadByte(void)
{
    // 定义变量保存数据
    uint8_t data = 0;

    // 用循环逐位读取SDA，保存数据
    for (uint8_t i = 0; i < 8; i++)
    {
        // 1. 拉低SCL，等待SDA数据翻转
        SCL_LOW;
        I2C_Delay;

        // 2. SCL拉高，开始采样
        SCL_HIGH;
        I2C_Delay;

        // 3. 读取SDA上的数据
        // 先将data左移一位，然后将最低位写入SDA电平
        data <<= 1;
        data |= READ_SDA ? 1 : 0;

        // 4. SCL拉低，采样结束
        SCL_LOW;
        I2C_Delay;
    }
    return data;
}
