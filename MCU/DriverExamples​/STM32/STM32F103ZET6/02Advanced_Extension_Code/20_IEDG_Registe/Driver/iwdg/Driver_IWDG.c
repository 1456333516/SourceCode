#include "Driver_IWDG.h"

void Driver_IWDG_Init(void)
{
    /* 1. 先启动看门狗  一旦启动, 就无法停止*/
    IWDG->KR = 0xcccc;

    /* 2. 解除写保护:  预分频 重装载寄存器*/
    IWDG->KR = 0x5555;
    /* 3. 设置预分频系数 */
    IWDG->PR = 0x4; /* 递减一次时: 1.6m  RLR=3000 3000 * 1.6ms = 4.8m */
    /* 4. 重装载寄存器的值 */
    IWDG->RLR = 3000;

    /* 5. 立即喂狗 */
    IWDG->KR = 0xAAAA;
}

/* 周期性喂狗 */
void Driver_IWDG_Refresh(void)
{
    IWDG->KR = 0xAAAA;
}