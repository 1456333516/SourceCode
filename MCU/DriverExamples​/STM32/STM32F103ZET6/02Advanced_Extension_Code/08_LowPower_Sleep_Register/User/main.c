#include "Driver_USART.h"
#include "Driver_LED.h"
#include "Delay.h"

void enter_sleep(void)
{
    /* 1. 浅睡眠 */
    SCB->SCR &= ~(SCB_SCR_SLEEPDEEP | SCB_SCR_SLEEPONEXIT);

    __WFI();
}

int main()
{
    Driver_USART1_Init();
    printf("低功耗 sleep模式\r\n");

    Driver_LED_Init();
    Drviver_LED_On(LED_2);

    
    while (1)
    {
        printf("3s后进入sleep模式\r\n");
        Delay_s(3);
        printf("立即进入sleep模式\r\n");
        Delay_ms(10);
        enter_sleep();
        printf("从睡眠被唤醒\r\n");
    }
}

