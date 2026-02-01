#include "Driver_TIM1.h"

void Driver_TIM1_Init(void)
{
    /* 1. 开启时钟 */
    /* 1.1 TIM1的时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    /* 1.2 GPIOA的时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* PA8的模式的配置 复用推挽  MODE=11  CNF=10*/
    GPIOA->CRH |= (GPIO_CRH_MODE8 | GPIO_CRH_CNF8_1);
    GPIOA->CRH &= ~GPIO_CRH_CNF8_0;

    /* 2. 定时器的配置: 时基单元的配置 */
    /* 2.1 预分配器 */
    TIM1->PSC = 7200 - 1;
    /* 2.2 重装载寄存器 频率时2Hz*/
    TIM1->ARR = 5000 - 1;
    /* 2.3 配置计数器的计数方向 0=向上 1=向下 */
    TIM1->CR1 &= ~TIM_CR1_DIR;
    /* 2.4  配置高级定时器的重复计数器 CNT溢出5, 则重复计数器寄存器的值时4*/
    TIM1->RCR = 4;

    /* 3. 配置输出通道 */
    /* 3.1 通道1配置为输出 CCMR1_CC1S=00 */
    TIM1->CCMR1 &= ~TIM_CCMR1_CC1S;
    /* 3.2 配置捕获比较寄存器的值 */
    TIM1->CCR1 = 2500;  /* 占空比 50% */
    /* 3.3 输出电平极性 */
    TIM1->CCER &= ~TIM_CCER_CC1P;
    /* 3.4 通道1使能 */
    TIM1->CCER |= TIM_CCER_CC1E;
    /* 3.5 配置输出模式为PWM1  CCMR1_OC1M=110 */
    TIM1->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
    TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_0;
    /* 3.6  主输出使能*/
    TIM1->BDTR |= TIM_BDTR_MOE;

    /* 4 中断配置 */
    /* 防止一启动就进入中断 */
    TIM1->EGR |= TIM_EGR_UG;
    TIM1->SR &= ~TIM_SR_UIF;

    TIM1->DIER |= TIM_DIER_UIE;

    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM1_UP_IRQn, 3);
    NVIC_EnableIRQ(TIM1_UP_IRQn);
}

void Driver_TIM1_Start(void)
{
    TIM1->CR1 |= TIM_CR1_CEN;
}

void Driver_TIM1_Stop(void)
{
    TIM1->CR1 &= ~TIM_CR1_CEN;
}

#include "stdio.h"
void TIM1_UP_IRQHandler(void)
{
    printf("中断\r\n");
    /*清除更新中断标记位 */
    TIM1->SR &= ~TIM_SR_UIF;

    /* 停止定时器 */
    Driver_TIM1_Stop();
}
