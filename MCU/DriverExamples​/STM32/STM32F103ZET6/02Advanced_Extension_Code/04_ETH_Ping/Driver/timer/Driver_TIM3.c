#include "Driver_TIM3.h"

void Driver_TIM3_Init(void)
{
    /* 1. 开启时钟 */
    /* 1.1 TIM3的时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    /* 1.2 GPIOA的时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* PA6的模式的配置 浮空输入 MODE=00 CNF=01*/
    GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6_1);
    GPIOA->CRL |= GPIO_CRL_CNF6_0;

    /* 2. 定时器的配置: 时基单元的配置 */
    /* 2.1 预分配器 计数器的时钟频率时1M, 计数依次时1us*/
    TIM3->PSC = 72 - 1;
    /* 2.2 重装载寄存器 进来避免溢出*/
    TIM3->ARR = 65535;
    /* 2.3 配置计数器的计数方向 0=向上 1=向下 */
    TIM3->CR1 &= ~TIM_CR1_DIR;

    /* 3. 定时器的配置: 输入捕获部分的配置 */
    /* 3.1 通道1的选择 让ch1连接到TI1 */
    TIM3->CR2 &= ~TIM_CR2_TI1S;
    /* 3.2 信号的滤波  不滤波 IC1F=0000*/
    TIM3->CCMR1 &= ~TIM_CCMR1_IC1F;
    /* 3.3 信号边缘检测配置 CC1P=0*/
    TIM3->CCER &= ~TIM_CCER_CC1P;
    /* 3.4 IC1映射到TI1上 CC1S=01 */
    TIM3->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    TIM3->CCMR1 |= TIM_CCMR1_CC1S_0;
    /* 3.5 对IC1进行预分频 不分频 IC1PCS=00 */
    TIM3->CCMR1 &= ~TIM_CCMR1_IC1PSC;
    /* 3.6  开启输入捕获中断 DIER_CC1IE=1*/
    TIM3->CCER |= TIM_CCER_CC1E;
    /* 3.7 使能通道1 */
    TIM3->DIER |= TIM_DIER_CC1IE;

    /* 4. NVIC的配置 */
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM3_IRQn, 3);
    NVIC_EnableIRQ(TIM3_IRQn);
}

void Driver_TIM3_Start(void)
{
    TIM3->CR1 |= TIM_CR1_CEN;
}

void Driver_TIM3_Stop(void)
{
    TIM3->CR1 &= ~TIM_CR1_CEN;
}

/* 记录上升沿的个数 */
uint8_t raiseEdgeCount = 0;

/* 存储信号的周期 us*/
uint16_t t = 0;
/* TIM3 中断服务函数 */
void TIM3_IRQHandler(void)
{
    /* 判断是否时通道1发生了捕获中断 */
    if(TIM3->SR & TIM_SR_CC1IF)
    {
        /* 清除标记位 */
        TIM3->SR &= ~TIM_SR_CC1IF;
        /* 上升沿的个数+1 */
        raiseEdgeCount++;
        if(raiseEdgeCount == 1) /* 第1个上升沿 */
        {
            TIM3->CNT = 0; /* 计数器清0 */
        }
        else if(raiseEdgeCount == 2) /* 第2个上升沿 */
        {
            t = TIM3->CCR1;
            //printf("%d\r\n", t);
            raiseEdgeCount = 0; /* 上升沿的个数重新从0计数 */
        }
    }
}

#include "stdio.h"
/* 11233us 返回信号周期 ms */
double Driver_TIM3_GetPWMCycle(void)
{

    return t / 1000.0;
}

/* 获取信号的频率 */
double Driver_TIM3_GetPWMFreq(void)
{
    return 1000000.0 / t;
}
