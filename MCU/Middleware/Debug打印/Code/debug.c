#include "debug.h"//核心宏定义在这个头文件


/* 串口重定向-调试用，串口1 */
int fputc(int ch, FILE * str)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

















