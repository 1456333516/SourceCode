#include "Delay.h"

#include "Driver_USART.h"

#include "string.h"
#include "App_ETH.h"
#include "App_Tcp.h"
#include "App_UDP.h"
#include "App_HttpServer.h"
int main()
{
    Driver_USART1_Init();
    printf("ÒÔÌ«Íø pingÊµÑé\r\n");

    App_Eth_Init();
    App_HttpSever_Init();

    while(1)
    {
        App_HttpServer_Start();
    }
}
