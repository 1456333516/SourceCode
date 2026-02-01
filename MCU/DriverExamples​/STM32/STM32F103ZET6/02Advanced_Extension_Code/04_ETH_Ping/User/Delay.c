#include "Delay.h" // Device header
#include "stdio.h"
void Delay_us(uint16_t us)
{
   /* 1. 重装载寄存器的值 */
   SysTick->LOAD = 72 * us;

   /* 2. 设置控制与状态寄存器 */
   SysTick->CTRL = 0x5;

   /* 3. 等待定时时间到 */
   while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG) == 0);
   
   /* 4. 定时器关闭 */
   SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
}

void Delay_ms(uint16_t ms)
{
    while (ms--)
    {
        Delay_us(1000);
    }
    
}

void Delay_s(uint16_t s)
{
    while (s--)
    {
        Delay_ms(1000);
    }
}
