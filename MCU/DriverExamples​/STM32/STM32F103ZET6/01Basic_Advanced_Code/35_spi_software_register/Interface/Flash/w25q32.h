/*
 * @Author: wushengran
 * @Date: 2024-05-13 15:14:22
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */

#ifndef __W25Q32_H
#define __W25Q32_H

#include "spi.h"

// 初始化
void W25Q32_Init(void);

// 读取生产商和设备ID
void W25Q32_ReadID(uint8_t * mid, uint16_t * did);

// 写入使能
void W25Q32_WriteEnalbe(void);

// 写入失能
void W25Q32_WriteDisable(void);

// 等待当前状态，不为busy
void W25Q32_WaitNotBusy(void);

// 擦除sector
void W25Q32_EraseSector(uint8_t block, uint8_t sector);

// 页写
void W25Q32_WritePage(uint8_t block, uint8_t sector, uint8_t page, uint8_t * data, uint8_t len);

// 读取
void W25Q32_Read(uint8_t block, uint8_t sector, uint8_t page, uint8_t * buffer, uint8_t len);

// 随机完整地址写入
void W25Q32_WritePageRandom(uint32_t addr, uint8_t *data, uint8_t len);

#endif
