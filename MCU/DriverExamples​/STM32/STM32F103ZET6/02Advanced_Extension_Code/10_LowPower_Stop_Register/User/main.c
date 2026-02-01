#include "Driver_USART.h"
#include "Driver_LED.h"
#include "Delay.h"
#include "Driver_Key.h"

void enter_stop(void)
{
    /* 当进入睡眠时, 进入深度睡眠 */
    SCB->SCR |= SCB_SCR_SLEEPDEEP;

    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    /* 清除电源控制器的pdds位 */
    PWR->CR &= ~PWR_CR_PDDS;
    /* 配置电压调节器的模式: 正常还是低功耗 */
    PWR->CR |= PWR_CR_LPDS;

    __WFI();
}

void config_clock(void)
{
    /* 1. 使能hse */
    RCC->CR |= RCC_CR_HSEON;
    /* 等待HSE就绪 */
    while((RCC->CR & RCC_CR_HSERDY) == 0);

    /* 2. 使能PLL */
    RCC->CR |= RCC_CR_PLLON;
    /* 等待PLL就绪 */
    while((RCC->CR & RCC_CR_PLLRDY) == 0);

    /* 3. 设置PLL作为系统时钟源 */
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    /* 等待PLL切换成功 */
    while((RCC->CFGR & RCC_CFGR_SWS_PLL) == 0);

}
int main()
{
    Driver_USART1_Init();
    printf("低功耗 stop 模式\r\n");
    Driver_Key_Init();

    Driver_LED_Init();
    Drviver_LED_On(LED_2);

    while(1)
    {
        printf("3s后进入stop 模式\r\n");
        Delay_s(3);
        printf("立即进入stop模式\r\n");
        Delay_ms(10);
        enter_stop();

        /* 重新配置(HSE)  PLL * 9 作为系统系统时钟 */
        config_clock();

        printf("从stop被唤醒\r\n");
    }
}
