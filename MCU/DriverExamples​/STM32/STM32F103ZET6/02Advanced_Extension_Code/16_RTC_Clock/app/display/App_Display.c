#include "App_Display.h"
#include "stdio.h"
#include "string.h"

extern const unsigned char gImage_atguigu[22248];
extern const unsigned char chinese[][128];
void App_Display_Init()
{
    /* 初始化lcd */
    Inf_LCD_Init();

    /* 整屏白色 */
    Inf_LCD_SetColor(0, 0, DISPLAY_W, DISPLAY_H, WHITE);
}

void App_Display_ShowLogo(void)
{
    Inf_LCD_ShowImg(57, 10, (uint8_t *)gImage_atguigu, 22248, 206, 54);
}

void App_Display_ShowTitle(void)
{
    //尚硅谷水质水位检测仪
    for (uint8_t i = 0; i < 10; i++)
    {
        /* 每个汉字大小是32 * 32 */
        Inf_LCD_ShowChineseChar(i * 32 + 0, 80, i, RED, WHITE);
    }
}


uint8_t tmpBuff[30] = {0};
void App_Display_ShowTDS(double tds)
{
    // double => string
    memset(tmpBuff, 0,  strlen(tmpBuff));
    sprintf((char *)tmpBuff, "tds = %.3f         ", tds);
    //Inf_LCD_SetColor(10,130, 310, 24, WHITE);
    Inf_LCD_ShowAsciString(10, 130, 24, tmpBuff, BLUE, WHITE);
}

// TODO
void App_Display_ShowWL(double wl)
{
    memset(tmpBuff, 0,  strlen(tmpBuff));
    sprintf((char *)tmpBuff, "wl = %.3f         ", wl);
    Inf_LCD_ShowAsciString(10, 160, 24, tmpBuff, BLUE, WHITE);
}
