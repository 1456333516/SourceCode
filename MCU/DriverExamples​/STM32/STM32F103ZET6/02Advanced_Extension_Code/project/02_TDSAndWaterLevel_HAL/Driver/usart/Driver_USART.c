#include "Driver_USART.h"

void Driver_USART1_Init(void)
{
    
    MX_USART1_UART_Init();

}

void Driver_USART1_Start(void)
{
    /* 使能串口 */
    //USART1->CR1 |= USART_CR1_UE;
}

void Driver_USART1_Stop(void)
{
    /* 失能串口 */
    //USART1->CR1 &= ~USART_CR1_UE;
}

void Driver_USART1_SendChar(uint8_t byte)
{
    
    HAL_UART_Transmit(&huart1, &byte, 1, 1000);

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
