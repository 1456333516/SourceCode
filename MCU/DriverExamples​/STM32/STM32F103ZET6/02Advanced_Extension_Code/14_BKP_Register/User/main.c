#include "Driver_USART.h"
#include "Driver_LED.h"
#include "Delay.h"
#include "Driver_GPIO.h"

int main()
{
    Driver_USART1_Init();
    printf("后备寄存器\r\n");

    Driver_GPIO_Init();

    /* 后备寄存器开启时钟 和 电源控制的时钟*/
    RCC->APB1ENR |= (RCC_APB1ENR_BKPEN | RCC_APB1ENR_PWREN);

    while(1)
    {
        uint8_t key = detect_key();
        if(key == KEY1)
        {
            /* 向后备寄存器写入数据 */
            /* 1. 解除对 rtc和后备寄存器的写保护 */
            PWR->CR |= PWR_CR_DBP;
            BKP->DR1 = 200;
            PWR->CR &= ~PWR_CR_DBP;

            printf("写入成功....\r\n");
        }
        else if(key == KEY2)
        {
            printf("读取成功: %d\r\n", BKP->DR1);
        }
    }
}
