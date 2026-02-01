
#include "App_Tcp.h"

#define SN 0 /* socket 的编号 */
#define CLIENT 0
#define SERVER 1
uint8_t connected = 0;
uint8_t self = SERVER;
/* 启动一个TCP的服务器 */
void App_Tcp_ServerStart(void)
{
    self = SERVER;
    uint8_t status = getSn_SR(SN);

    // printf("%#x\r\n", status);

    if(status == SOCK_CLOSED) /* 表示socket已经关闭了 */
    {
        uint8_t r = socket(SN, Sn_MR_TCP, 8888, SF_TCP_NODELAY);
        if(r == SN)
        {
            printf("socket 0打开成功\r\n");
        }
        else
        {
            printf("socket 0打开失败 %d\r\n", r);
        }
        connected = 0;
    }
    else if(status == SOCK_INIT) /* 表示socket的已经打开, 并处于tcp模式 */
    {
        /* 监听客户端的连接 */
        uint8_t r = listen(SN);
        if(r == SOCK_OK)
        {
            printf("socket 0监听成功\r\n");
        }
        else
        {
            printf("socket 0监听失败 %d\r\n", r);
        }
        connected = 0;
    }
    else if(status == SOCK_CLOSE_WAIT)
    {
        printf("失去与客户端的连接\r\n");
        close(SN);
    }
}

/* 启动一个客户端 */
uint8_t  serverIp[4] = {192, 168, 32, 228}; /* 服务端的ip地址: (电脑的ip地址) */
uint16_t serverPort  = 9999;
void     App_Tcp_ClientStart(void)
{
    self = CLIENT;
    uint8_t status = getSn_SR(SN);

    if(status == SOCK_CLOSED) /* 表示socket已经关闭了 */
    {
        uint8_t r = socket(SN, Sn_MR_TCP, 8888, SF_TCP_NODELAY);
        if(r == SN)
        {
            printf("socket 0打开成功\r\n");
        }
        else
        {
            printf("socket 0打开失败 %d\r\n", r);
        }
        connected = 0;
    }
    else if(status == SOCK_INIT) /* 表示socket的已经打开, 并处于tcp模式 */
    {
        /* 客户端需要主动区连接tcp服务器 */
        int8_t r = connect(SN, serverIp, serverPort);
        if(r == SOCK_OK)
        {
            printf("客户端连接服务器成功\r\n");
            App_Tcp_SendData("hello, this stm32 tcp client!", 29);
        }
        else
        {
            printf("客户端连接服务器失败 %d\r\n", r);
        }
        connected = 0;
    }
    else if(status == SOCK_CLOSE_WAIT)
    {
        printf("失去与服务端的连接\r\n");
        close(SN);
    }
}
uint8_t  clientIp[4];
uint16_t clientPort;
/* 接收tcp协议传输的数据 */
void App_Tcp_ReceiveData(uint8_t data[], uint16_t *dataLen)
{
    uint8_t status = getSn_SR(SN);
    // printf("%#x\r\n", status);

    if(status == SOCK_ESTABLISHED) /* 表示客户端已经连接成功服务器 */
    {
        if(connected == 0 && self == SERVER)
        {
            /* 获取对方的ip地址和端口号 */
            getSn_DIPR(SN, clientIp);
            clientPort = getSn_DPORT(SN);
            printf("客户端连接建立成功:ip =  %d.%d.%d.%d, port = %d\r\n",
                   clientIp[0],
                   clientIp[1],
                   clientIp[2],
                   clientIp[3],
                   clientPort);
            connected = 1;
        }

        if(getSn_IR(SN) & Sn_IR_RECV) /* 收到tcp数据 */
        {
            setSn_IR(SN, Sn_IR_RECV); /* 写1清除 */
            /* 收到的数据长度 */
            *dataLen = getSn_RX_RSR(SN);

            recv(SN, data, *dataLen); /* 接收数据 */
        }
    }
}

void App_Tcp_SendData(uint8_t data[], uint16_t dataLen)
{
    if(dataLen == 0) return;

    uint8_t status = getSn_SR(SN);
    if(status == SOCK_ESTABLISHED)
    {
        send(SN, data, dataLen);
    }
}