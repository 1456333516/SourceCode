#include "Driver_SPI.h"

#define CS_H (HAL_GPIO_WritePin(W25Q32_CS_GPIO_Port, W25Q32_CS_Pin, GPIO_PIN_SET))
#define CS_L (HAL_GPIO_WritePin(W25Q32_CS_GPIO_Port, W25Q32_CS_Pin, GPIO_PIN_RESET))

#define SPI_DELAY (HAL_Delay(1))

void Driver_SPI_Init()
{
    MX_SPI1_Init();
}

void Driver_SPI_Start()
{
    CS_L;
    SPI_DELAY;
}

void Driver_SPI_Stop()
{
    CS_H;
    SPI_DELAY;
}

static uint8_t Driver_SPI_SwapByte(uint8_t byte)
{
    uint8_t rByte = 0;
    HAL_SPI_TransmitReceive(&hspi1, &byte, &rByte, 1, 2000);

    return rByte;
}

void Driver_SPI_SendByte(uint8_t byte)
{
    Driver_SPI_SwapByte(byte);
}

uint8_t Driver_SPI_ReceiveByte(void)
{
    return Driver_SPI_SwapByte(0xff);
}
