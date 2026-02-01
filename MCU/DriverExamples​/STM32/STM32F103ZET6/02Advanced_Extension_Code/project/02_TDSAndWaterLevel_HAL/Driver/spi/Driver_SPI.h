#ifndef __DRIVER_SPI_H
#define __DRIVER_SPI_H

#include "spi.h"
#include "gpio.h"


void Driver_SPI_Init(void);
void Driver_SPI_Start(void);
void Driver_SPI_Stop(void);

uint8_t Driver_SPI_SwapByte(uint8_t byte);

void Driver_SPI_SendByte(uint8_t byte);
uint8_t Driver_SPI_ReceiveByte(void);

#endif 
/*
串行  同步  全双工
4线  cs mosi miso sck

spi: 靠片选 cs 拉低选中

数据交换 

4个模式:
        极性        相位
    0    0          0
    1    0          1
    2    1          0
    3    1          1
*/