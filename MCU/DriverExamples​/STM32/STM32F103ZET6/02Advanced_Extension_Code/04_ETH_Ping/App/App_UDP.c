
#include "App_UDP.h"
#include "string.h"

#define SN 0 /* socket 的编号 */


void App_UDP_ReceiveData(uint8_t data[], uint16_t *dataLen, uint8_t *rIp, uint16_t *rPort)
{
    memset(data, 0, strlen(data));
    *dataLen = 0;
    

    uint8_t status = getSn_SR(SN);
    if(status == SOCK_CLOSED) /* 表示socket已经关闭了 */
    {
        uint8_t r = socket(SN, Sn_MR_UDP, 8888, 0);
        if(r == SN)
        {
            printf("socket 0打开成功\r\n");
        }
        else
        {
            printf("socket 0打开失败 %d\r\n", r);
        }
    }
    else if(status == SOCK_UDP)
    {
        /* 先判断是否收到数据 */
        if(getSn_IR(SN) & Sn_IR_RECV)
        {
            setSn_IR(SN, Sn_IR_RECV);
            /* 从寄存器读取到数据的长度, 比实际大8个字节 */
            uint16_t tmp = getSn_RX_RSR(SN);
            if(tmp > 0)
            {
                *dataLen = tmp - 8;

                recvfrom(SN, data, *dataLen, rIp, rPort);
            }
        }
    }
}

void App_UDP_SendData(uint8_t data[], uint16_t dataLen, uint8_t *sIp, uint16_t sPort)
{

    uint8_t status = getSn_SR(SN);


    if(status == SOCK_CLOSED) /* 表示socket已经关闭了 */
    {
        uint8_t r = socket(SN, Sn_MR_UDP, 8888, 0);
        if(r == SN)
        {
            printf("socket 0打开成功\r\n");
        }
        else
        {
            printf("socket 0打开失败 %d\r\n", r);
        }
    }
    else if(status == SOCK_UDP)
    {
        /* 发送数据 */
        int32_t r = sendto(SN, data, dataLen, sIp, sPort);
        printf("发送完毕.... %d\r\n", r);
    }
}
