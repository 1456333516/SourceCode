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

#define OK 0
#define FAIL -1

// 初始化
void I2C_Init(void);

// 发出起始信号
uint8_t I2C_Start(void);

// 发出结束信号
void I2C_Stop(void);

// 主机发送应答
void I2C_Ack(void);
// 主机发送非应答
void I2C_NACK(void);

// 发送一个地址
uint8_t I2C_SendAddr(uint8_t addr);

// 发送一个字节（向EEPROM写入数据）
uint8_t I2C_SendByte(uint8_t byte);

// 接收一个字节（从EEPROM读取数据）
uint8_t I2C_ReadByte(void);

#endif
