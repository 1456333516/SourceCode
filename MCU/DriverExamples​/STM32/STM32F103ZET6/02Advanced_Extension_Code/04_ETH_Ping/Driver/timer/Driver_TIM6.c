#include "Driver_TIM6.h"

void Driver_TIM6_Init(void)
{
    /* 1. 给定时器6开启时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    /* 2. 配置预分频器
        设置 7200-1 表示是7200分频, 计数器的频率变成10000
            周期是100us
        1s可以计数10000个
    */
    TIM6->PSC = 7200 - 1;
    /* 3. 配置自动重装载寄存器的值
        计数10000个溢出依次

        频率是10000,表示1s计数10000个,计数10000个表示1s
    */
    TIM6->ARR = 10000 - 1;

    TIM6->CR1 |= TIM_CR1_URS; /* 只有定时器溢出才会产生中断 */
    /* 为了解决一启动就立即进中断的问题
        产生一个更新事件, 预分频系数和自动重装载寄存器都会立即更新到影子寄存器
     */
    TIM6->EGR |= TIM_EGR_UG;
    //TIM6->SR &= ~TIM_SR_UIF;

    /* 4. 使能更新中断 (溢出的时候产生中断)*/
    TIM6->DIER |= TIM_DIER_UIE;

    /* 5. 配置NVIC  3 4 5 6 7*/
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM6_IRQn, 3);
    NVIC_EnableIRQ(TIM6_IRQn);
}

void Driver_TIM6_Start(void)
{
    /* 计数器使能 */
    TIM6->CR1 |= TIM_CR1_CEN;
}

void Driver_TIM6_Stop(void)
{
    /* 计数器失能 */
    TIM6->CR1 &= ~TIM_CR1_CEN;
}
void TIM6_IRQHandler(void) 
{
    /* 1. 先清除中断标志位 */
    TIM6->SR &= ~TIM_SR_UIF;

    Drviver_LED_Toggle(LED_1);
    printf("1s\r\n");
}
