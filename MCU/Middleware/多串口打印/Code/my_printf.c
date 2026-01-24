/*
 * @Descripttion: 
 * @version: 
 * @Author: tlm
 * @Date: 2025-01-23 15:41:54
 * @LastEditors: tlm
 * @LastEditTime: 2025-01-23 15:50:02
 */
#include "my_printf.h"

/* 多串口重定向 */
int my_printf(UART_HandleTypeDef *huart, const char *format, ...)
{
    char buffer[512];
    va_list arg;
    int len;
    // 初始化可变参数列表
    va_start(arg, format);
    len = vsnprintf(buffer, sizeof(buffer), format, arg);
    va_end(arg);
    HAL_UART_Transmit(huart, (uint8_t *)buffer, (uint16_t)len, 0xFFFF);
    return len;
}








