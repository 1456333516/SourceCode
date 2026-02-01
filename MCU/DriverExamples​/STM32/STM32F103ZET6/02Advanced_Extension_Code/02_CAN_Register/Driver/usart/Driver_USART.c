#include "Driver_USART.h"

void Driver_USART1_Init(void)
{
    /* 1. 开启时钟 */
    RCC->APB2ENR |= (RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN);
    /*
        TX: PA9 复用推挽  MODE=11 CNF=10
        RX: PA10 浮空输入 MODE=00  CNF=01
    */
   GPIOA->CRH |= (GPIO_CRH_MODE9 | GPIO_CRH_CNF9_1 | GPIO_CRH_CNF10_0);
   GPIOA->CRH &= ~(GPIO_CRH_CNF9_0 | GPIO_CRH_MODE10 | GPIO_CRH_CNF10_1);

    /* 2. 串口的寄存器的配置 */

    /* 2.1 波特率 115200 */
    USART1->BRR = 0x271;

    /* 2.2 字长 */
    USART1->CR1 &= ~USART_CR1_M;
    /* 2.3 不要检验位 */
    USART1->CR1 &= ~USART_CR1_PCE;
    /* 2.4 停止位 */
    USART1->CR2 &= ~USART_CR2_STOP;

    /* 2.5 发送使能 */
    USART1->CR1 |= USART_CR1_TE;


}

void Driver_USART1_Start(void)
{
    /* 使能串口 */
    USART1->CR1 |= USART_CR1_UE;
}

void Driver_USART1_Stop(void)
{
    /* 失能串口 */
    USART1->CR1 &= ~USART_CR1_UE;
}

void Driver_USART1_SendChar(uint8_t byte)
{
    while((USART1->SR & USART_SR_TXE) == 0);

    USART1->DR = byte;

}

void Driver_USART1_SendString(uint8_t *str, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
    {
        Driver_USART1_SendChar(str[i]);
    }
}


int fputc(int c, FILE *file)
{
    Driver_USART1_SendChar((uint8_t)c);
    return c;
}
