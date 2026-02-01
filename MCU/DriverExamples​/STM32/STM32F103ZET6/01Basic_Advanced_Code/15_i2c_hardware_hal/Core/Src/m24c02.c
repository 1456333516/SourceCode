/*
 * @Author: wushengran
 * @Date: 2024-04-30 15:11:19
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#include "m24c02.h"

// 初始化
void M24C02_Init(void)
{
    MX_I2C2_Init();
}

// 写入一个字节
void M24C02_WriteByte(uint8_t innerAddr, uint8_t byte)
{
    HAL_I2C_Mem_Write(&hi2c2, W_ADDR, innerAddr, I2C_MEMADD_SIZE_8BIT, &byte, 1, HAL_MAX_DELAY);
    HAL_Delay(5);
}

// 读取一个字节
uint8_t M24C02_ReadByte(uint8_t innerAddr)
{
    uint8_t data;
    HAL_I2C_Mem_Read(&hi2c2, R_ADDR, innerAddr, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);
    return data;
}

// 连续写入多个字节（页写）
void M24C02_WriteBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t size)
{
    HAL_I2C_Mem_Write(&hi2c2, W_ADDR, innerAddr, I2C_MEMADD_SIZE_8BIT, bytes, size, HAL_MAX_DELAY);
    HAL_Delay(5);
}

// 连续读取多个字节
void M24C02_ReadBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t size)
{
    HAL_I2C_Mem_Read(&hi2c2, R_ADDR, innerAddr, I2C_MEMADD_SIZE_8BIT, bytes, size, HAL_MAX_DELAY);
}
