#include "Driver_Key.h"

//粗延时函数，微秒
void delay_us(uint16_t time)
{    
   u16 i=0;  
   while(time--)
   {
      i=10;  //自己定义
      while(i--) ;    
   }
}
//毫秒级的延时
void delay_ms(uint16_t time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12000;  
      while(i--) ;    
   }
}

/**
 * @description: 初始化按键.
 *  1. 给按键对应的io口设置工作模式: 下拉输入
 *  2. 配置复用为外部中断
 *  3. 配置外部中断控制器 EXTI
 *  4. 配置NVIC
 */
void Driver_Key_Init(void)
{
    /* 1. 开启时钟 */
    /* 1.1  GPIOF*/
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
    /* 1.2  AFIO*/
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    /* 2. 配置 PF10为下拉输入: MODE=65 CNF=10  ODR=0 */
    GPIOF->CRH &= ~GPIO_CRH_MODE10;
    GPIOF->CRH |= GPIO_CRH_CNF10_1;
    GPIOF->CRH &= ~GPIO_CRH_CNF10_0;
    GPIOF->ODR &= ~GPIO_ODR_ODR10;

    /* 3. 配置AFIO 配置PF10引脚为外部中断  EXTICR3  0101*/
    AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI10;
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI10_PF;

    /* 4. 配置EXTI */
    /* 4.1. 配置上升沿触发 RTSR TR10=1*/
    EXTI->RTSR |= EXTI_RTSR_TR10;
    /* 4.2 开启 LINE10, 配置的中断屏蔽寄存器 */
    EXTI->IMR |= EXTI_IMR_MR10;

    /* 5. 配置 NVIC */
    /* 5.1 配置优先级组 (3-7) 配置3表示4个二进制位全部用于表示抢占优先级*/
    NVIC_SetPriorityGrouping(3);
    /* 5.2 配置优先级 参数1:中断号*/
    NVIC_SetPriority(EXTI15_10_IRQn, 3);
    /* 5.3 使能Line10 */
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**
 * @description: line 15-10的中断服务函数.
 *  一旦按键下按键1,则会执行一次这个函数
 * @return {*}
 */
uint8_t key3Pressed = 0;
void    EXTI15_10_IRQHandler(void)
{
    /* 务必一定必须要清除中断标志位 */
    EXTI->PR |= EXTI_PR_PR10;

    delay_ms(5);
    if((GPIOF->IDR & GPIO_IDR_IDR10) != 0)
    {
        key3Pressed = 1;
    }
}
