#include "Driver_USART.h"
#include "Driver_LED.h"
#include "Delay.h"
#include "Driver_Key.h"

void enter_standby(void)
{

    SCB->SCR |= SCB_SCR_SLEEPDEEP;

    PWR->CR |= PWR_CR_PDDS;

    /* 清除唤醒标志位:  PWR_CSR_WUF */
    PWR->CR |= PWR_CR_CWUF;

    /* 使能唤醒引脚 */
    PWR->CSR |= PWR_CSR_EWUP;

    __WFI();
}

void standby_detect(void)
{
    if(PWR->CSR & PWR_CSR_SBF)
    {
        printf("从待机被唤醒....\r\n");
        /* 清除待机标志位 */
        PWR->CR |= PWR_CR_CSBF;
    }
    else
    {
        printf("正常启动....\r\n");
    }
}

void standby_wakeup_detect(void)
{
    if(PWR->CSR & PWR_CSR_WUF)
    {
        printf("用唤醒引脚 唤醒....\r\n");
        
        PWR->CR |= PWR_CR_CWUF;
    }
    else
    {
        printf("使用用复位按键唤醒....\r\n");
    }
}

int main()
{
    Driver_USART1_Init();
    printf("低功耗 stop 模式\r\n");
    Driver_Key_Init();

    Driver_LED_Init();
    Drviver_LED_On(LED_2);

    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    standby_detect();
    standby_wakeup_detect();

    while(1)
    {
        printf("3s后进入standby 模式\r\n");
        Delay_s(3);
        printf("立即进入 standby 模式\r\n");
        Delay_ms(10);
        enter_standby();

        printf("从待机被被唤醒\r\n");
    }
}
