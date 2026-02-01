#include "Inf_TM7711.h"

void Inf_TM7711_Init(void)
{
    Driver_GPIO_Init();
}

uint32_t Inf_TM7711_ReadValue(void)
{
    uint32_t data = 0x0;

    SCK_L;        /* 拉低时钟 */
    while(OUT_R); /* 等待out变低 */
    TM7711_DELAY; /* 延时一段时间 >0.1us */

    for(uint8_t i = 0; i < 24; i++)
    {
        SCK_H;
        TM7711_DELAY;
        SCK_L;
        /* 开始读取数据 */
        data <<= 1;
        if(OUT_R)
        {
            data |= 1;
        }
        TM7711_DELAY;
    }

    SCK_H;
    TM7711_DELAY;
    SCK_L;
    TM7711_DELAY;
    return data ^ 0x800000;
}
/*
异或:
    0 ^ 0 = 0
    1 ^ 0 = 1
    0 ^ 1 = 1
    1 ^ 1 = 0

-----
0x800000->0x7fffff

0x800000 ^ 0x800000 = 0x0
0x800001 ^ 0x800000 = 0x1
....
0xffffff ^ 0x800000 = 0x7fffff

0x000000 ^ 0x800000 = 0x100000
0x000001 ^ 0x800000 = 0x100001
...
0x7fffff ^ 0x800000 = 0xffffff
...

*/
