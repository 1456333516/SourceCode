#include "Driver_TIM5.h"

void Driver_TIM5_Init(void)
{
    /* 1. 开启时钟 */
    /* 1.1 TIM5的时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    /* 1.2 GPIOA的时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* PA1的模式的配置 复用推挽  MODE=11  CNF=10*/
    GPIOA->CRL |= (GPIO_CRL_MODE1 | GPIO_CRL_CNF1_1);
    GPIOA->CRL &= ~GPIO_CRL_CNF1_0;

    /* 2. 定时器的配置: 时基单元的配置 */
    /* 2.1 预分配器 */
    TIM5->PSC = 7200 - 1;
    /* 2.2 重装载寄存器 */
    TIM5->ARR = 100 - 1;
    /* 2.3 配置计数器的计数方向 0=向上 1=向下 */
    TIM5->CR1 &= ~TIM_CR1_DIR;

    /* 3. 定时器的配置: 输出比较部分的配置 */
    /* 3.1 通道2配置输出模式  CCMR1_CC2S=0 输出模式  其他都是输入模式*/
    TIM5->CCMR1 &= ~TIM_CCMR1_CC2S;

    /* 3.2 配置输出比较模式位PWM1  CCMR1_OC2M=110  */
    /*TIM5->CCMR1 &= ~TIM_CCMR1_OC2M;
    TIM5->CCMR1 |= 6 << 12;*/
    TIM5->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
    TIM5->CCMR1 &= ~TIM_CCMR1_OC2M_0;
    /* 3.3 配置捕获/比较寄存器的值 */
    TIM5->CCR2 = 1;

    /* 3.4 配置输出的有效电平  CCER_CC2P=0 高电平有效  =1低电平有效*/
    TIM5->CCER &= ~TIM_CCER_CC2P;
    /* 3.5 使能通道2 CCER_CC2E = 1 */
    TIM5->CCER |= TIM_CCER_CC2E;
}

void Driver_TIM5_Start(void)
{
    TIM5->CR1 |= TIM_CR1_CEN;
}

void Driver_TIM5_Stop(void)
{
    TIM5->CR1 &= ~TIM_CR1_CEN;
}

void Driver_TIM5_SetDutyCycle(uint16_t dutyCycle)
{
    if(dutyCycle <= 0) dutyCycle = 0;
    if(dutyCycle >= 99) dutyCycle = 99;

    TIM5->CCR2 = dutyCycle;
}
