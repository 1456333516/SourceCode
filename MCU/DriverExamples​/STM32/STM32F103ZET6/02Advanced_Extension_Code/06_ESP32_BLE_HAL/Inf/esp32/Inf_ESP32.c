#include "Inf_ESP32.h"
#include "string.h"

uint8_t           rxBuff[128] = {0}; /* 产生一次中断接收的数据 */
volatile uint16_t rxSize      = 0;   /* 产生一次中断接收到的数据的长度 */

uint8_t  responseBuff[512] = {0}; /* 存储一次响应的数据 */
uint16_t responseSize      = 0;   /* 一次响应接收到的数据长度 */

void Inf_ESP32_SendCmd(uint8_t *cmd);
void Inf_ESP32_HandleResponse(void);

void Inf_ESP32_Init(void)
{
    /* 接收串口数据: 通过中断的方式 . 必须先调用

        先开中断,当数据接收完毕之后,再关闭中断

        1. 如果碰到了空闲帧,会产生中断
        2. 如果收到的数据>=128个字节, 也会结束这次接收
    */
    HAL_UARTEx_ReceiveToIdle_IT(&huart2, rxBuff, 128);

    /* 1. 测试esp32是否正常:  "AT\r\n" */
    Inf_ESP32_SendCmd("AT\r\n");

    /* 2. 重启模块 并进入正常模式 */
    Inf_ESP32_SendCmd("AT+RST=0\r\n");
    HAL_Delay(3000);

    Inf_ESP32_SendCmd("AT\r\n");

    /* 3. 查询版本信息 */
    Inf_ESP32_SendCmd("AT+GMR\r\n");

    /* 4. 设置提示信息 */
    Inf_ESP32_SendCmd("AT+SYSMSG=5\r\n");
}

void Inf_ESP32_BLE_Init(void)
{
    /* 0. 关闭wifi功能(清理wifi驱动) */
    Inf_ESP32_SendCmd("AT+CWINIT=0\r\n");

    /* 1. 初始化为服务器模式 */
    Inf_ESP32_SendCmd("AT+BLEINIT=2\r\n");

    /* 2. 创建服务 */
    Inf_ESP32_SendCmd("AT+BLEGATTSSRVCRE\r\n");

    /* 3. 开启提供的所有服务 */
    Inf_ESP32_SendCmd("AT+BLEGATTSSRVSTART\r\n");

    /* 4. 设置广播参数 */
    Inf_ESP32_SendCmd("AT+BLEADVPARAM=50,50,0,0,7,0,,\r\n");

    /* 5. 自动广播参数设置 */
    Inf_ESP32_SendCmd("AT+BLEADVDATAEX=\"ble-teacher\",\"A102\",\"0112030405\",1\r\n");

    /* 6. 开始广播 */
    Inf_ESP32_SendCmd("AT+BLEADVSTART\r\n");

    /* 透传模式: 必须等待有客户端连接之后才能开启.
        连接之前可以先设置一下 透传 的一些参数

     */
    /* 7. 设置透传的一些参数 */
    Inf_ESP32_SendCmd("AT+BLESPPCFG=1,1,7,1,5\r\n");
}
/*
当连接成功之后:
    +BLECONN:0,"7f:b3:6a:19:f4:35"  开启透传

    +BLECONNPARAM:0,0,0,6,0,500

    +WRITE:0,1,7,1,2,
    +BLECONNPARAM:0,0,0,40,0,500
选择服务和特征:
    +WRITE:0,1,7,1,2,
    +WRITE:0,1,7,1,2,
    +WRITE:0,1,7,1,2,
    +WRITE:0,1,7,1,2,
传送数据:
    +WRITE:0,1,5,,4,abcd

关闭连接的时候:
    +BLEDISCONN:0,"41:74:6e:e5:05:a8"

*/
void Inf_ESP32_BLE_ReadData(uint8_t data[], uint16_t *dataLen)
{
    *dataLen = 0;

    if(rxSize == 0) return;

    /* 开始对数据进行解析处理 */
    if(strstr((char *)rxBuff, "+BLECONN:")) /* 表示有客户端连接进来 */
    {
        /* 开启透传模式 */
        printf("有客户端连接,准备开启透传...\r\n");
        Inf_ESP32_SendCmd("AT+BLESPP\r\n");
        printf("透传开启成功...\r\n");
    }
    else if(strstr((char *)rxBuff, "+BLEDISCONN:"))
    {
        /* 如果连接端口, 应该关闭透传模式 */
        printf("客户端退出,关闭透传模式 \r\n");
        HAL_UART_Transmit(&huart2, "+++", 3, 2000);
        HAL_Delay(1500);
        /* 重新广播 */
        printf("重新开始广播\r\n");
        Inf_ESP32_SendCmd("AT+BLEADVSTART\r\n");
    }
    else if(strstr((char *)rxBuff, "+BLECONNPARAM:") || strstr((char *)rxBuff, "+WRITE"))
    {
        /* 什么都不做 */
    }
    else
    {
        /* 真正的透传数据 */
        memcpy(data, rxBuff, rxSize);
        *dataLen = rxSize;
    }

    memset(rxBuff, 0, rxSize);
    rxSize = 0;
}

void Inf_ESP32_BLE_SendData(uint8_t data[], uint16_t dataLen)
{
    if(dataLen == 0) return;
    HAL_UART_Transmit(&huart2, data, dataLen, 2000);
}

/*
当碰到空闲帧  或者收到的数据大于等于128
size: 表示实际收到的字节数
*/
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart->Instance == USART2)
    {
        /* 重新调用, 给下次接收做准备 */
        HAL_UARTEx_ReceiveToIdle_IT(&huart2, rxBuff, 128);
        rxSize = Size;
    }
}

/* 发送指令 */
void Inf_ESP32_SendCmd(uint8_t *cmd)
{
    HAL_UART_Transmit(&huart2, cmd, strlen((char *)cmd), 2000);

    /* 处理响应:  一定要阻塞. */
    Inf_ESP32_HandleResponse();
}

/* 处理响应:  一定要阻塞.

发送一个AT指令之后, 会有一个响应,这个响应需要多次中断才能完成

一个响应一般以OK或者ERROE

abc
def

[abcdef]
 */
void Inf_ESP32_HandleResponse(void)
{
    rxSize = 0;
    /* 先清除 一次响应缓冲区  */
    memset(responseBuff, 0, responseSize);
    responseSize = 0;

    uint8_t cnt = 3; /* 用来保证循环的次数 */
    do {
        uint32_t timeout = 0xffffff;
        while(rxSize == 0 && timeout--); /* 等待中断的时候不至于一直等待 */
        /* 把这次中断产生的数据, 存储到 一次响应缓冲区*/
        memcpy(&responseBuff[responseSize], rxBuff, rxSize);
        responseSize += rxSize;
        rxSize = 0;
        // printf("%#x  %d\r\n", timeout, cnt);
    } while(strstr((char *)responseBuff, "OK") == NULL && strstr((char *)responseBuff, "ERROR") == NULL && cnt--);

    printf("%.*s\r\n", responseSize, responseBuff);
    printf("====================================\r\n");
}
