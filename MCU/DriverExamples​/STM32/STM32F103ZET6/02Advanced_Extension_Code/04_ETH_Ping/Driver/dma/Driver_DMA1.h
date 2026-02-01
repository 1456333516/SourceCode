#ifndef __DRIVER_DMA1_H
#define __DRIVER_DMA1_H

#include "stm32f10x.h"
extern uint8_t dmaTC;
void Driver_DMA1_Init(void);

void Driver_DMA1_StartTransmit(uint32_t srcAddr, uint32_t desAddr, uint16_t dataLen);

#endif

