#ifndef __DRIVER_SPI_H
#define __DRIVER_SPI_H
#include "stm32f10x.h"
#include "Delay.h"

void Driver_SPI_Init(void);
void Driver_SPI_Start(void);
void Driver_SPI_Stop(void);

void Driver_SPI_SendByte(uint8_t byte);

uint8_t Driver_SPI_ReciveByte(void);

#endif
