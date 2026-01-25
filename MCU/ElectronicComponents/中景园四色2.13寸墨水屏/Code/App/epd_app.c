/* 
 * @Description: 电子纸显示屏应用
 * 注意事项：1.头文件需包含#include "SPI_Init.h"
                         #include "EPD.h"
                         #include "EPD_GUI.h"
            2.编码注意都需要为GB2312。
 * @Author: tlm
 * @LastEditors: tlm
 * @LastEditTime: 2026-01-25 16:05:05
 * @FilePath: /EPD_APP/epd_app.c
 * Copyright (c) 2026 by tlm, All Rights Reserved.
 * 

 */
#include "epd_app.h"

uint8_t ImageBW[8000];


/**
 * @brief 电子纸显示屏应用初始化函数
 *        初始化电子纸显示屏并绘制商品价签界面
 * 
 * @param void 无参数
 * @return void 无返回值
 */
void EPD_APP_Init(void)
{
    // 1. 初始化画布
    Paint_NewImage(ImageBW, EPD_W, EPD_H, 0, WHITE); 
    Paint_Clear(WHITE);
    EPD_Init();

    // 2. 绘制边框
    EPD_DrawRectangle(0, 0, EPD_H - 1, EPD_W - 1, RED, 1);
    EPD_DrawRectangle(3, 3, EPD_H - 3, EPD_W - 3, WHITE, 1);
    
    // 3. 第一行：标题 (建议汉字后面加个空格或重新输入)
    EPD_ShowChinese(65, 6, "商品价标签", 24, BLACK);

    // 4. 第二行：品名与产地
    EPD_DrawRectangle(3,31,EPD_H-3,EPD_W-89,RED,1);
		EPD_DrawRectangle(135,31,EPD_H-114,EPD_W-3,RED,1);
    EPD_ShowChinese(3, 34, "品名：", 16, BLACK);
    EPD_ShowChinese(43, 34, "西红柿", 16, BLACK);
    EPD_ShowChinese(137, 34, "产地：", 16, BLACK);
    EPD_ShowChinese(177, 34, "北京市", 16, BLACK);

    // 5. 第三行：价格与规格
		EPD_DrawRectangle(3,51,EPD_H-3,EPD_W-69,RED,1);
    EPD_ShowChinese(3, 54, "零售价：", 16, BLACK);
    EPD_ShowChinese(137, 54, "规格：", 16, BLACK);
    EPD_ShowString(177, 54, "500g", 16, BLACK, WHITE);

    // 6. 第四、五行：条码与会员价
		EPD_DrawRectangle(135,71,EPD_H-3,EPD_W-49,RED,1);
		EPD_DrawRectangle(135,91,EPD_H-3,EPD_W-29,RED,1);
    EPD_ShowChinese(137, 74, "条码：", 16, BLACK);
    EPD_ShowString(175, 74, "12345678", 16, BLACK, WHITE);
    EPD_ShowChinese(119, 98, "元", 16, BLACK);
    EPD_ShowString(35, 84, "4.50", 24, BLACK, WHITE); // 建议背景色与画布一致
    EPD_ShowChinese(137, 98, "会员价：", 16, BLACK);
    EPD_ShowChinese(231, 98, "元", 16, BLACK);
    EPD_ShowString(191, 98, "4.5", 16, BLACK, WHITE);

    // 7. 刷新屏幕
    EPD_Display(ImageBW);
    EPD_Update();
}


