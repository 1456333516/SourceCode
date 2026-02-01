#include "Inf_W24C02.h"

void Inf_W24C02_Init(void)
{
    Driver_I2C2_Init();
}

void Inf_W24C02_WriteByte(uint8_t innerAddr, uint8_t byte)
{
    /* 1. 开始信号 */
    Driver_I2C2_Start();

    /* 2. 发送写地址 */
    Driver_I2C_SendByte(ADDR);
    /* 3. 等待响应 */
    uint8_t ack = Driver_I2C2_WaitAck();
    if (ack == ACK)
    {
        /* 4. 发送内部地址 */
        Driver_I2C_SendByte(innerAddr);
        /* 5. 等待响应 */
        Driver_I2C2_WaitAck();
        /* 6. 发送具体数据 */
        Driver_I2C_SendByte(byte);
        /* 7. 等待响应 */
        Driver_I2C2_WaitAck();
        /* 8. 停止信号 */
        Driver_I2C2_Stop();
    }
    Delay_ms(5);
}

uint8_t Inf_W24C02_ReadByte(uint8_t innerAddr)
{
    /* 1. 起始信号 */
    Driver_I2C2_Start();
    /* 2. 发送一个写地址   假写 */
    Driver_I2C_SendByte(ADDR);
    /* 3. 等待响应 */
    Driver_I2C2_WaitAck();
    /* 4. 发送内部地址 */
    Driver_I2C_SendByte(innerAddr);
    /* 5. 等待响应 */
    Driver_I2C2_WaitAck();
    /* 6. 起始信号 */
    Driver_I2C2_Start();
    /* 7. 发送读地址  真读 */
    Driver_I2C_SendByte(ADDR + 1);
    /* 8. 等待响应 */
    Driver_I2C2_WaitAck();
    /* 9. 读取一个字节 */
    uint8_t byte = Driver_I2C_ReadByte();

    /* 10. 给对方一个非应答 */
    Driver_I2C2_NAck();

    /* 11. 停止信号 */
    Driver_I2C2_Stop();
    return byte;
}

/**
 * @description: 页写入.一次写入多个字节
 * @param {uint8_t} innerAddr
 * @param {uint8_t} *bytes
 * @param {uint8_t} len
 * @return {*}
 */
void Inf_W24C02_WriteBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t len)
{
    /* 1. 开始信号 */
    Driver_I2C2_Start();

    /* 2. 发送写地址 */
    Driver_I2C_SendByte(ADDR);
    /* 3. 等待响应 */
    uint8_t ack = Driver_I2C2_WaitAck();
    if (ack == ACK)
    {
        /* 4. 发送内部地址 */
        Driver_I2C_SendByte(innerAddr);
        /* 5. 等待响应 */
        Driver_I2C2_WaitAck();

        for (uint8_t i = 0; i < len; i++)
        {
            /* 6. 发送具体数据 */
            Driver_I2C_SendByte(bytes[i]);
            /* 7. 等待响应 */
            Driver_I2C2_WaitAck();
        }
        /* 8. 停止信号 */
        Driver_I2C2_Stop();
    }
    Delay_ms(5);
}

/**
 * @description: 一次性读取多个字节的数据
 * @param {uint8_t} innerAddr 起始位置
 * @param {uint8_t} *bytes 存储读到的数据
 * @param {uint8_t} len 读取的字节数
 * @return {*}
 */
void Inf_W24C02_ReadBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t len)
{

    /* 1. 起始信号 */
    Driver_I2C2_Start();
    /* 2. 发送一个写地址   假写 */
    Driver_I2C_SendByte(ADDR);
    /* 3. 等待响应 */
    Driver_I2C2_WaitAck();
    /* 4. 发送内部地址 */
    Driver_I2C_SendByte(innerAddr);
    /* 5. 等待响应 */
    Driver_I2C2_WaitAck();
    /* 6. 起始信号 */
    Driver_I2C2_Start();
    /* 7. 发送读地址  真读 */
    Driver_I2C_SendByte(ADDR + 1);
    /* 8. 等待响应 */
    Driver_I2C2_WaitAck();

    for (uint8_t i = 0; i < len; i++)
    {
        /* 9. 读取一个字节 */
        bytes[i] = Driver_I2C_ReadByte();
        if (i < len - 1)
        {
            Driver_I2C2_Ack();
        }
        else
        {
            Driver_I2C2_NAck();
        }
    }
    /* 11. 停止信号 */
    Driver_I2C2_Stop();
}

void Inf_W24C02_WriteBytesAutoPage(uint8_t innerAddr, uint8_t *bytes, uint8_t len)
{
    // 不需要自动换页
    if ((innerAddr % 16) + len <= 16)
    {
        Inf_W24C02_ReadBytes(innerAddr, bytes, len);
        return;
    }
    // 需要换页处理
    //uint8_t apge = (len - innerAddr % 16) / 16 + 1  ;// 一共需要page页

}