#include "Driver_I2C2.h"
#include "Delay.h"

#define I2C_DELAY Delay_us(10)


/**
 * @description: 初始化
 * @return {*}
 */
void Driver_I2C2_Init(void)
{
    /*
        PB10->SCL
        PB11->SDA
            开漏输出: 既可以用于输出也可以输入. 外界要有上拉电阻.
                    用于输入的时候,最好先输出一个1,把线的控制权交给外界.

            MODE=11 CNF=01

     */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    GPIOB->CRH |= (GPIO_CRH_MODE10 | GPIO_CRH_MODE11 | GPIO_CRH_CNF10_0 | GPIO_CRH_CNF11_0);
    GPIOB->CRH &= ~(GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_1);
}

/**
 * @description: 起始信号
 * @return {*}
 */
void Driver_I2C2_Start(void)
{
    /* 1. 拉高sda和scl */
    SDA_HIGH;
    SCL_HIGH;
    /* 2. 延时 */
    I2C_DELAY;
    /* 3. 拉低sda */
    SDA_LOW;
    /* 4. 延时 */
    I2C_DELAY;
}

/**
 * @description: 停止信号
 * @return {*}
 */
void Driver_I2C2_Stop(void)
{
    /* 1. scl 拉高 sda拉低 */
    SCL_HIGH;
    SDA_LOW;
    /* 2. 延时 */
    I2C_DELAY;
    /* 3. 拉高sda */
    SDA_HIGH;
    /* 4.  延时 */
    I2C_DELAY;
}

/**
 * @description: 接收方产生应答信号
 */
void Driver_I2C2_Ack(void)
{
    /* 1. 拉高sda和拉低scl */
    SDA_HIGH;
    SCL_LOW;
    /* 2. 延时 */
    I2C_DELAY;
    /* 3. sda拉低 */
    SDA_LOW;
    /* 4. 延时 */
    I2C_DELAY;
    /* 5. scl拉高 */
    SCL_HIGH;
    /* 6. 延时 */
    I2C_DELAY;
    /* 7. scl拉低 */
    SCL_LOW;
    /* 8. 延时 */
    I2C_DELAY;
    /* 9. sda 拉高 */
    SDA_HIGH;
    /* 10. 延时 */
    I2C_DELAY;
}

/**
 * @description: 接收方产生非应答信号
 */
void Driver_I2C2_NAck(void)
{
    /* 1. 拉高sda和拉低scl */
    SDA_HIGH;
    SCL_LOW;
    /* 2. 延时 */
    I2C_DELAY;

    /* 3. scl拉高 */
    SCL_HIGH;

    /* 4. 延时 */
    I2C_DELAY;

    /* 5. scl拉低*/
    SCL_LOW;

    /* 6. 延时 */
    I2C_DELAY;
}

/**
 * @description: 等待接收方法的应答
 * @return {*} 应答或非应答
 */
uint8_t Driver_I2C2_WaitAck(void)
{
    /* 1. 把sda拉高, sda的主动权交给对方(e2prom芯片) */
    SDA_HIGH;

    /* 2. scl拉低  */
    SCL_LOW;
    /* 3. 延时 */
    I2C_DELAY;
    /* 4. 拉高scl */
    SCL_HIGH;
    /* 5. 延时 */
    I2C_DELAY;
    /* 6. 读取sda的电平 */
    uint8_t ack = ACK;
    if (READ_SDA)
    {
        ack = NACK;
    }
    /* 7. 拉低scl */
    SCL_LOW;

    /* 8. 延时 */
    I2C_DELAY;
    return ack;
}

/**
 * @description: 发送一个字节的数据
 * @param {uint8_t} byte 要发送的字节
 */
void Driver_I2C_SendByte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        /* 1. sda和scl 拉低 */
        SDA_LOW;
        SCL_LOW;

        I2C_DELAY;

        /* 2. 向sda写数据 */
        if (byte & 0x80)
        {
            SDA_HIGH;
        }
        else
        {
            SDA_LOW;
        }
        I2C_DELAY;

        /* 3. 时钟拉高 */
        SCL_HIGH;

        I2C_DELAY;

        /* 4. 时钟拉低 */
        SCL_LOW;

        I2C_DELAY;

        /* 5. 左移1位, 为下一次发送做准备 */
        byte <<= 1;
    }
}

/**
 * @description: 读一个字节的数据
 * @param {uint8_t} byte 要发送的字节
 */
uint8_t Driver_I2C_ReadByte(void)
{
    uint8_t data = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        /* 1. 拉低scl */
        SCL_LOW;
        /* 2. 延时 */
        I2C_DELAY;
        /* 3. 拉高scl */
        SCL_HIGH;
        /* 4. 延时 */
        I2C_DELAY;
        /* 5. 读取sda */
        data <<= 1;
        if (READ_SDA)
        {
            data |= 0x01;
        }
        /* 6. 拉低scl */
        SCL_LOW;

        /* 7. 延时 */
        I2C_DELAY;
    }

    return data;
}
