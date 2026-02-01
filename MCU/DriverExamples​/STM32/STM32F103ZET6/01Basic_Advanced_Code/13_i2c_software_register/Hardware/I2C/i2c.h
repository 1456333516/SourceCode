/*
 * @Author: wushengran
 * @Date: 2024-04-29 14:42:34
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __I2C_H
#define __I2C_H

#include "stm32f10x.h"
#include "delay.h"

// 宏定义ACK/NACK的电平值
#define ACK 0
#define NACK 1

// 宏定义一些基本操作
#define SCL_HIGH (GPIOB->ODR |= GPIO_ODR_ODR10)
#define SCL_LOW (GPIOB->ODR &= ~GPIO_ODR_ODR10)
#define SDA_HIGH (GPIOB->ODR |= GPIO_ODR_ODR11)
#define SDA_LOW (GPIOB->ODR &= ~GPIO_ODR_ODR11)

#define READ_SDA (GPIOB->IDR & GPIO_IDR_IDR11)

#define I2C_Delay Delay_us(10)

// 初始化
void I2C_Init(void);

// 发出起始信号
void I2C_Start(void);

// 发出结束信号
void I2C_Stop(void);

// 主机发送应答
void I2C_Ack(void);
// 主机发送非应答
void I2C_NACK(void);

// 主机等待从设备发送应答
uint8_t I2C_Wait4Ack(void);

// 发送一个字节（向EEPROM写入数据）
void I2C_SendByte(uint8_t byte);

// 接收一个字节（从EEPROM读取数据）
uint8_t I2C_ReadByte(void);

#endif
