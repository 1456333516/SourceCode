#ifndef __APP_DISPLAY_H
#define __APP_DISPLAY_H

#include "Inf_LCD.h"

/* 初始化显示模块 */
void App_Display_Init(void);

/* 显示公司Logo */
void App_Display_ShowLogo(void);

/* 显示Title: 尚硅谷水质水位检测 */
void App_Display_ShowTitle(void);

/* 显示水质:  tds = 11.11 */
void App_Display_ShowTDS(double tds);

/* 显示水位:  wl = 11.11 */
void App_Display_ShowWL(double wl);

#endif
