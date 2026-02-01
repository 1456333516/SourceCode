#include "Inf_ESP32.h"
#include "string.h"

uint8_t  rxBuff[128] = {0}; /* 产生一次中断接收的数据 */
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
}

void Inf_ESP32_Wifi_Init(void)
{
    /* 工作模式:  AP还是 STA */
    Inf_ESP32_SendCmd("AT+CWMODE=1,1\r\n");
    /* 设置wifi 名和密码 */
    Inf_ESP32_SendCmd("AT+CWJAP=\"lzc2022\",\"aaaaaaaa\"\r\n");

    /* 获取 wifi 的 ip */
    Inf_ESP32_SendCmd("AT+CIPSTA?\r\n");
}

void Inf_ESP32_Wifi_StartTcpServer(void)
{
    /* 1.  启用多连接, 然后才允许启动tcp服务器 */
    Inf_ESP32_SendCmd("AT+CIPMUX=1\r\n");

    /* 2. 启动tcp服务器 */
    Inf_ESP32_SendCmd("AT+CIPSERVER=1,8888,\"TCP\"\r\n");

    /* 3. 设置 ipd 消息的格式: 包含对方的ip和端口  */
    Inf_ESP32_SendCmd("AT+CIPDINFO=1\r\n");
}

void Inf_ESP32_Wifi_StartTcpClient(void)
{
    /* 0.  不启动多连接 */
    Inf_ESP32_SendCmd("AT+CIPMUX=0\r\n");

    /* 1.  连接服务器 */
    Inf_ESP32_SendCmd("AT+CIPSTART=\"TCP\",\"192.168.43.197\",9999\r\n");

    /* 2. 设置 ipd 消息的格式: 包含对方的ip和端口  */
    Inf_ESP32_SendCmd("AT+CIPDINFO=1\r\n");
}

void Inf_ESP32_Wifi_ReceiveTcpData(int8_t   *connId,
                                   uint8_t   ip[],
                                   uint32_t *port,
                                   uint8_t   data[],
                                   uint16_t *dataLen)
{
    /*
    esp32为tcp的服务器时:
        0,CONNECT

        +IPD,0,3,"192.168.43.196",57676:abc   // 真正的tcp数据

        0,CLOSED
    esp32为tcp客户端时:
        +IPD,7,"192.168.43.197",9999:abccdef

    */
    memset(data, 0, strlen(data));
    *dataLen = 0;

    if(rxSize > 0) /* 串口收到了数据 */
    {

        if(strstr(rxBuff, "+IPD"))
        {
            if(*connId >= 0)  /* 当连接id 小于0表示是单连接, 这个时候IPD格式不一样 */
            {

                sscanf((char *)rxBuff, "%*[\r\n]+IPD,%d,%d,\"%[^\"]\",%d", connId, dataLen, ip, port);
            }
            else
            {
                sscanf((char *)rxBuff, "%*[\r\n]+IPD,%d,\"%[^\"]\",%d", dataLen, ip, port);
            }
            strtok(rxBuff, ":");
            memcpy(data, strtok(NULL, ":"), *dataLen);
        }
        rxSize = 0;
    }
}

void Inf_ESP32_Wifi_SendTcpData(int8_t   connId,
                                uint8_t  data[],
                                uint16_t dataLen)
{
    uint8_t cmd[30] = {0};
    if(connId >= 0)
    {
        sprintf(cmd, "AT+CIPSENDL=%d,%d\r\n", connId, dataLen); /* 当作为服务器 发送数据的指令 */
    }
    else
    {
        sprintf(cmd, "AT+CIPSENDL=%d\r\n", dataLen);  /* 当作为客户端时发送 发送数据的指令 */
    }
    Inf_ESP32_SendCmd(cmd);

    HAL_UART_Transmit(&huart2, data, dataLen, 2000);

    Inf_ESP32_HandleResponse();
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
