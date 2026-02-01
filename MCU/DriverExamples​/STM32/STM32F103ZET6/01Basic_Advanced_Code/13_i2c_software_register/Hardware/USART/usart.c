#include "usart.h"

void USART_Init(void)
{
    // 1. 配置时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // 2. GPIO引脚配置
    // 2.1 PA9 设为复用推挽输出 MODE: 11, CNF: 10
    GPIOA->CRH |= GPIO_CRH_MODE9;
    GPIOA->CRH |= GPIO_CRH_CNF9_1;
    GPIOA->CRH &= ~GPIO_CRH_CNF9_0;

    // 2.2 PA10 设为浮空输入 MODE：00， CNF：01
    GPIOA->CRH &= ~GPIO_CRH_MODE10;
    GPIOA->CRH &= ~GPIO_CRH_CNF10_1;
    GPIOA->CRH |= GPIO_CRH_CNF10_0;

    // 3. 串口配置
    // 3.1 设置波特率
    USART1->BRR = 0x271;

    // 3.2 串口使能和收发使能
    USART1->CR1 |= (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);
}

// 发送一个字符
void SendChar(uint8_t ch)
{
    while ((USART1->SR & USART_SR_TXE) == 0)
    {
    }

    // 准备就绪，继续发送下一个字符
    USART1->DR = ch;
}

// 接收一个字符
uint8_t ReceiveChar(void)
{
    while ((USART1->SR & USART_SR_RXNE) == 0)
    {
        // 一旦检测到空闲帧，就立即退出
        if ((USART1->SR & USART_SR_IDLE))
        {
            return '\0';
        }
    }
    // 准备就绪，读取DR，然后继续接收下一个字符
    return USART1->DR;
}

// 发送一个字符串
void SendString(uint8_t *str, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        SendChar(str[i]);
    }
}

// 接收字符串
void ReceiveString(uint8_t buffer[], uint8_t *pSize)
{
    // 定义一个局部变量，保存当前接收到数据的长度
    uint8_t i = 0;

    // 1. 错误写法：
    // 用循环判断当前SR中的IDLE标志位是否为1
    // while ((USART1->SR & USART_SR_IDLE) == 0)
    // {
    //     // 如果还没有空闲，就继续接收下一个字符
    //     buffer[i] = ReceiveChar();
    //     i++;
    // }
    // * pSize = i;

    // 2. 正确写法
    // 外层循环，不停地读取下一个字符，如果检测到空闲帧就退出
    // while (1)
    // {
    //     // 内层循环，判断当前数据帧是否已经接收完毕
    //     while ((USART1->SR & USART_SR_RXNE) == 0)
    //     {
    //         // 一旦检测到空闲帧，就立即退出
    //         if ((USART1->SR & USART_SR_IDLE))
    //         {
    //             *pSize = i;
    //             return;
    //         }
    //     }

    //     // 准备就绪，读取DR，然后继续接收下一个字符
    //     buffer[i] = USART1->DR;
    //     i++;
    // }

    // 3. 错误写法改进：
    // 用循环判断当前SR中的IDLE标志位是否为1
    while ((USART1->SR & USART_SR_IDLE) == 0)
    {
        // 如果还没有空闲，就继续接收下一个字符
        buffer[i] = ReceiveChar();
        if (buffer[i] != '\0')
        {
            i++;
        }
    }
    // USART1->SR;
    USART1->DR;
    * pSize = i;
}

// 重写fputc函数
int fputc(int ch, FILE * file)
{
    SendChar(ch);
    return ch;
}
