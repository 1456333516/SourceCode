#include "Inf_W25Q32.h"

/*
W25Q32
    一共4MB

    64块 每块64KB
    每块有16个扇区 每个扇区4K
    每个扇区有16页 每页256

    字节->页->扇区->块

    写的时候,是以页位单位进行写

    一次最多写一页, 最多写256字节
*/
void Inf_W25Q32_Init(void)
{
    Driver_SPI_Init();
}

void Inf_W25Q32_ReadId(uint8_t *mid, uint16_t *did)
{
    Driver_SPI_Start();
    Driver_SPI_SendByte(0x9f);

    *mid = Driver_SPI_ReceiveByte();
    *did |= Driver_SPI_ReceiveByte() << 8;
    *did |= Driver_SPI_ReceiveByte();

    Driver_SPI_Stop();
}

void Inf_W25Q32_Erase(uint8_t block, uint8_t sector)
{
    /*  */
    Inf_W25Q32_WaiteNotBusy();
    Inf_W25Q32_WriteEnable();

    /* 计算出要擦除的扇区的首地址 */
    uint32_t addr = block * 0x010000 + sector * 0x001000;

    Driver_SPI_Start();

    Driver_SPI_SendByte(0x20);
    Driver_SPI_SendByte((addr >> 16) & 0xff);
    Driver_SPI_SendByte((addr >> 8) & 0xff);
    Driver_SPI_SendByte(addr & 0xff);

    Driver_SPI_Stop();
}

void Inf_W25Q32_WriteBytes(uint8_t block, uint8_t sector, uint8_t page, uint8_t *data, uint16_t len)
{
    Inf_W25Q32_WaiteNotBusy();
    Inf_W25Q32_WriteEnable();
    Inf_W25Q32_WaiteNotBusy();

    uint32_t addr = block * 0x010000 + sector * 0x001000 + page * 0x000100;
    Driver_SPI_Start();

    Driver_SPI_SendByte(0x02);

    Driver_SPI_SendByte((addr >> 16) & 0xff);
    Driver_SPI_SendByte((addr >> 8) & 0xff);
    Driver_SPI_SendByte(addr & 0xff);

    for(uint16_t i = 0; i < len; i++)
    {
        Driver_SPI_SendByte(data[i]);
    }

    Driver_SPI_Stop();
}

void Inf_W25Q32_ReadBytes(uint8_t block, uint8_t sector, uint8_t page, uint8_t *data, uint32_t len)
{
    Inf_W25Q32_WaiteNotBusy();

    uint32_t addr = block * 0x010000 + sector * 0x001000 + page * 0x000100;
    Driver_SPI_Start();

    Driver_SPI_SendByte(0x03);

    Driver_SPI_SendByte((addr >> 16) & 0xff);
    Driver_SPI_SendByte((addr >> 8) & 0xff);
    Driver_SPI_SendByte(addr & 0xff);

    for(uint16_t i = 0; i < len; i++)
    {
        data[i] = Driver_SPI_ReceiveByte();
    }
    Driver_SPI_Stop();
}

void Inf_W25Q32_WaiteNotBusy(void)
{
    Driver_SPI_Start();
    Driver_SPI_SendByte(0x05);

    while(Driver_SPI_ReceiveByte() & 0x01);

    Driver_SPI_Stop();
}

void Inf_W25Q32_WriteEnable(void)
{
    Inf_W25Q32_WaiteNotBusy();
    Driver_SPI_Start();
    Driver_SPI_SendByte(0x06);

    Driver_SPI_Stop();
}
