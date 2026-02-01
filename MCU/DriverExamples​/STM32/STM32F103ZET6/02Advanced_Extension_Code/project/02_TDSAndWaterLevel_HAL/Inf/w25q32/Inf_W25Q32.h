#ifndef __INF_W25Q32_H
#define __INF_W25Q32_H

#include "Driver_SPI.h"

void Inf_W25Q32_Init(void);

void Inf_W25Q32_ReadId(uint8_t *mid, uint16_t *did);

void Inf_W25Q32_Erase(uint8_t block, uint8_t sector);

void Inf_W25Q32_WriteBytes(uint8_t  block,
                           uint8_t  sector,
                           uint8_t  page,
                           uint8_t *data,
                           uint16_t len);

void Inf_W25Q32_ReadBytes(uint8_t  block,
                           uint8_t  sector,
                           uint8_t  page,
                           uint8_t *data,
                           uint32_t len);

void Inf_W25Q32_WaiteNotBusy(void);

void Inf_W25Q32_WriteEnable(void);

#endif