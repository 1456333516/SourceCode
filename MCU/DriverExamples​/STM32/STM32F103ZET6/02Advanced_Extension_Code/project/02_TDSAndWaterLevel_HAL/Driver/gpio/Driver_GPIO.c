#include "Driver_GPIO.h"

void Driver_GPIO_Init(void)
{
   //
}

/* 检测按按键按下:
如果按键1被按下返回1
如果按键2被按下返回2
*/
uint8_t detect_key(void)
{
    /* 表示key1或者key2按下 */
    if(READ_KEY1 == 0 || READ_KEY2 == 0)
    {
        HAL_Delay(50);
        if(READ_KEY1 == 0)
        {
            /* 等待松开 */
            while(READ_KEY1 == 0);
            return KEY1;
        }
        else if(READ_KEY2 == 0)
        {
            while(READ_KEY2 == 0);
            return KEY2;
        }
    }
    return NO_KEY;
}
