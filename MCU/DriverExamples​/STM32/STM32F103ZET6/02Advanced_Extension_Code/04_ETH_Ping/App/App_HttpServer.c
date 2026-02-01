
#include "App_HttpServer.h"
#include "string.h"
#include "Driver_LED.h"

uint8_t txBuff[2048];
uint8_t rxBuff[2048];
uint8_t sockeList[] = {0, 1, 2, 3, 4, 5, 6, 7};

const uint8_t content[] = "<!doctype html>\n"
                             "<html lang=\"en\">\n"
                             "<head>\n"
                             "    <meta charset=\"GBK\">\n"
                             "    <meta name=\"viewport\"\n"
                             "          content=\"width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0\">\n"
                             "    <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">\n"
                             "    <title>尚硅谷嵌入式课程</title>\n"
                             "\n"
                             "    <style type=\"text/css\">\n"
                             "        #open_red{\n"
                             "            color: red;\n"
                             "            width: 100px;\n"
                             "            height: 40px;\n"
                             "\n"
                             "\n"
                             "        }\n"
                             "        #close_red{\n"
                             "            color: black;\n"
                             "            width: 100px;\n"
                             "            height: 40px;\n"
                             "        }\n"
                             "    </style>\n"
                             "</head>\n"
                             "<body>\n"
                             "<a href=\"/index.html?action=1\"><button id=\"open_red\" >开灯</button></a>\n"
                             "<a href=\"/index.html?action=2\"><button id=\"close_red\" >关灯</button></a>\n"
                             "<a href=\"/index.html?action=3\"><button id=\"close_red\" >翻转</button></a>\n"
                             "</body>\n"
                             "</html>";

void App_HttpSever_Init(void)
{
    Driver_LED_Init();
    /* 初始化一个httpserver */
    httpServer_init(txBuff, rxBuff, 8, sockeList);
    /* 注册一个网页: 首页 */
    reg_httpServer_webContent("index.html", content);
}
void App_HttpServer_Start(void)
{

    /* 启动服务器 */
    for(size_t i = 0; i < sizeof(sockeList); i++)
    {
        httpServer_run(i);
    }
}

void App_HttpServer_DoAction(uint8_t action);

// 这个函数由http驱动层调用
void App_HttpServer_ParseUrl(uint8_t url[])
{
    // /index.html?action=2
    char *index = strstr((char *)url, "action");
    if(index != NULL)
    {
        uint8_t action = (uint8_t)(*(index + 7));
        App_HttpServer_DoAction(action);
    }
}

void App_HttpServer_DoAction(uint8_t action)
{
    if(action == '1')
    {
        Drviver_LED_On(LED_2);
    }
    else if(action == '2')
    {
        Drviver_LED_Off(LED_2);
    }
    else if(action == '3')
    {
        Drviver_LED_Toggle(LED_2);
    }
}