#include "Inf_W25Q32.h"

/*
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

void Inf_W25Q32_ReadId(uint8_t *mid, uint16_t *id)
{
    Driver_SPI_Start();

    Driver_SPI_SendByte(0x9F);

    *mid = Driver_SPI_ReciveByte();

    *id = 0;
    *id |= Driver_SPI_ReciveByte() << 8;
    *id |= Driver_SPI_ReciveByte();

    Driver_SPI_Stop();
}

void Inf_W25Q32_ReadUID(uint8_t *uid)
{
    Driver_SPI_Start();

    Driver_SPI_SendByte(0x4B);

    Driver_SPI_ReciveByte();
    Driver_SPI_ReciveByte();
    Driver_SPI_ReciveByte();
    Driver_SPI_ReciveByte();

    *uid |= Driver_SPI_ReciveByte();
    //*uid |= Driver_SPI_ReciveByte();

    Driver_SPI_Stop();
}

/* 写使能 */
void Inf_W25Q32_WriteEnable(void)
{
    Driver_SPI_Start();

    Driver_SPI_SendByte(0x06);

    Driver_SPI_Stop();
}

/* 等待不忙 */
void Inf_W25Q32_WaiteNotBusy(void)
{
    Driver_SPI_Start();

    Driver_SPI_SendByte(0x05);

    while(Driver_SPI_ReciveByte() & 0x01)
        ;

    Driver_SPI_Stop();
}

/**
 * @description: 擦除指定的块的指定扇区
 *  发送一个扇区首地址就可以擦除了
 * @param {uint8_t} block 要擦除的块
 * @param {uint8_t} sector 要擦除的扇区
 */
void Inf_W25Q32_EraseSector(uint8_t block, uint8_t sector)
{
    Inf_W25Q32_WaiteNotBusy();
    Inf_W25Q32_WriteEnable();

    // 根据参数计算出扇区的首地址  (每块0x010000个地址, 每个扇区0x001000个地址)
    uint32_t sectorStartAddr = block * 0x010000 + sector * 0x001000;
    Driver_SPI_Start();

    Driver_SPI_SendByte(0x20);   // 擦除扇区的指令

    /*把地址发送出去 */
    Driver_SPI_SendByte(sectorStartAddr >> 16 & 0xff);
    Driver_SPI_SendByte(sectorStartAddr >> 8 & 0xff);
    Driver_SPI_SendByte(sectorStartAddr & 0xff);

    Driver_SPI_Stop();
}

/**
 * @description: 向指定块的指定扇区的指定页写入指定长度的数据
 *  从指定页的首地址开始写
 * @param {uint8_t} block
 * @param {uint8_t} sector
 * @param {uint8_t} page
 * @param {uint8_t} *data
 * @param {uint16_t} len
 */
void Inf_W25Q32_WriteBytes(uint8_t block, uint8_t sector, uint8_t page, uint8_t *data, uint16_t len)
{
    Inf_W25Q32_WaiteNotBusy();
    Inf_W25Q32_WriteEnable();

    // 计算出来页的首地址
    uint32_t pageStartAddr = block * 0x010000 + sector * 0x001000 + page * 0x000100;
    Driver_SPI_Start();

    Driver_SPI_SendByte(0x02);

    Driver_SPI_SendByte(pageStartAddr >> 16 & 0xff);
    Driver_SPI_SendByte(pageStartAddr >> 8 & 0xff);
    Driver_SPI_SendByte(pageStartAddr & 0xff);

    for(uint16_t i = 0; i < len; i++)
    {
        Driver_SPI_SendByte(data[i]);
    }

    Driver_SPI_Stop();
}

/**
 * @description: 读取指定块的指定扇区的指定页指定长度的数据
 *  从指定页的首地址开始读
 * @param {uint8_t} block
 * @param {uint8_t} sector
 * @param {uint8_t} page
 * @param {uint8_t} *data
 * @param {uint16_t} len
 * @return {*}
 */
void Inf_W25Q32_ReadBytes(uint8_t block, uint8_t sector, uint8_t page, uint8_t *data, uint16_t len)
{
    Inf_W25Q32_WaiteNotBusy();

    // 计算出来页的首地址
    uint32_t pageStartAddr = block * 0x010000 + sector * 0x001000 + page * 0x000100;
    Driver_SPI_Start();

    Driver_SPI_SendByte(0x03);

    Driver_SPI_SendByte(pageStartAddr >> 16 & 0xff);
    Driver_SPI_SendByte(pageStartAddr >> 8 & 0xff);
    Driver_SPI_SendByte(pageStartAddr & 0xff);

    for(uint16_t i = 0; i < len; i++)
    {
        data[i] = Driver_SPI_ReciveByte();
    }

    Driver_SPI_Stop();
}

