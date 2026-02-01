#include "w25q32.h"

// 初始化
void W25Q32_Init(void)
{
    SPI_Init();
}

// 读取生产商和设备ID
void W25Q32_ReadID(uint8_t *mid, uint16_t *did)
{
    // 0. 开启 SPI 通道，片选生效
    SPI_Start();

    // 1. 发送指令
    SPI_SwapByte(0x9f);

    // 2. 下一个周期获取发来的第一个字节，制造商ID
    *mid = SPI_SwapByte(0xff);

    // 3. 下两个周期，获取设备ID
    *did = 0;
    *did |= SPI_SwapByte(0xff) << 8;
    *did |= SPI_SwapByte(0xff) & 0xff;

    SPI_Stop();
}

// 写入使能
void W25Q32_WriteEnalbe(void)
{
    SPI_Start();
    // 发送指令
    SPI_SwapByte(0x06);
    SPI_Stop();
}

// 写入失能
void W25Q32_WriteDisable(void)
{
    SPI_Start();
    // 发送指令
    SPI_SwapByte(0x04);
    SPI_Stop();
}

// 等待当前状态，不为busy
void W25Q32_WaitNotBusy(void)
{
    SPI_Start();

    // 1. 发送指令
    SPI_SwapByte(0x05);

    // 2. 循环等待，直到接收数据最后一位为 0
    while (SPI_SwapByte(0xff) & 0x01)
    {
    }

    SPI_Stop();
}

// 擦除sector
void W25Q32_EraseSector(uint8_t block, uint8_t sector)
{
    // 1. 先等待不为busy
    W25Q32_WaitNotBusy();

    // 2. 开启写使能
    W25Q32_WriteEnalbe();

    // 3. 拼接完整地址，sector首地址，后12位都为0
    uint32_t sectorHeadAddr = block * 0x010000 + sector * 0x001000;

    // 4. 启动
    SPI_Start();
    // 5. 发送指令
    SPI_SwapByte(0x20);

    // 6. 发送24位地址
    SPI_SwapByte(sectorHeadAddr >> 16 & 0xff); // 取三个字节中的最高字节
    SPI_SwapByte(sectorHeadAddr >> 8 & 0xff);  // 取第二个字节
    SPI_SwapByte(sectorHeadAddr >> 0 & 0xff);

    SPI_Stop();
    W25Q32_WriteDisable();
}

// 页写
void W25Q32_WritePage(uint8_t block, uint8_t sector, uint8_t page, uint8_t *data, uint8_t len)
{
    // 1. 先等待不为busy
    W25Q32_WaitNotBusy();

    // 2. 开启写使能
    W25Q32_WriteEnalbe();

    // 3. 拼接完整地址，page首地址，后8位都为0
    uint32_t pageHeadAddr = block * 0x010000 + sector * 0x001000 + page * 0x000100;

    // 4. 启动
    SPI_Start();

    // 5. 发送指令
    SPI_SwapByte(0x02);

    // 6. 发送24位地址
    SPI_SwapByte(pageHeadAddr >> 16 & 0xff); // 取三个字节中的最高字节
    SPI_SwapByte(pageHeadAddr >> 8 & 0xff);  // 取第二个字节
    SPI_SwapByte(pageHeadAddr >> 0 & 0xff);

    // 7. 继续发送数据
    for (uint8_t i = 0; i < len; i++)
    {
        SPI_SwapByte(data[i]);
    }

    SPI_Stop();
    W25Q32_WriteDisable();
}

// 读取
void W25Q32_Read(uint8_t block, uint8_t sector, uint8_t page, uint8_t *buffer, uint8_t len)
{
    // 1. 先等待不为busy
    W25Q32_WaitNotBusy();

    // 2. 拼接完整地址，page首地址，后8位都为0
    uint32_t pageHeadAddr = block * 0x010000 + sector * 0x001000 + page * 0x000100;

    // 3. 启动
    SPI_Start();

    // 4. 发送指令
    SPI_SwapByte(0x03);

    // 5. 发送24位地址
    SPI_SwapByte(pageHeadAddr >> 16 & 0xff); // 取三个字节中的最高字节
    SPI_SwapByte(pageHeadAddr >> 8 & 0xff);  // 取第二个字节
    SPI_SwapByte(pageHeadAddr >> 0 & 0xff);

    // 6. 读取数据
    for (uint8_t i = 0; i < len; i++)
    {
        buffer[i] = SPI_SwapByte(0xff);
    }

    SPI_Stop();
}


// 页写
void W25Q32_WritePageRandom(uint32_t addr, uint8_t *data, uint8_t len)
{
    // 1. 先等待不为busy
    W25Q32_WaitNotBusy();

    // 2. 开启写使能
    W25Q32_WriteEnalbe();

    // 4. 启动
    SPI_Start();

    // 5. 发送指令
    SPI_SwapByte(0x02);

    // 6. 发送24位地址
    SPI_SwapByte(addr >> 16 & 0xff); // 取三个字节中的最高字节
    SPI_SwapByte(addr >> 8 & 0xff);  // 取第二个字节
    SPI_SwapByte(addr >> 0 & 0xff);

    // 7. 继续发送数据
    for (uint8_t i = 0; i < len; i++)
    {
        SPI_SwapByte(data[i]);
    }

    SPI_Stop();
    W25Q32_WriteDisable();
}
