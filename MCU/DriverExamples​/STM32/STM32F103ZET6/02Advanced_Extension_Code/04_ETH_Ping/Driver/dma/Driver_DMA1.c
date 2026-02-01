#include "Driver_DMA1.h"

void Driver_DMA1_Init(void)
{
    /* 1. 开始DMA时钟 */
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    /* 2. DMA配置 */
    
    /* 2.2 数据传输方向  从外设读  CCR_DIR = 0  从存储器读 1*/
    DMA1_Channel4->CCR |= DMA_CCR4_DIR;

    /* 2.3 设置存储器和外设的数据宽度 00=8  01=16 10=32*/
    DMA1_Channel4->CCR &= ~DMA_CCR4_MSIZE;
    DMA1_Channel4->CCR &= ~DMA_CCR4_PSIZE;

    /* 2.4 存储器和外设的地址是否自增 */
    DMA1_Channel4->CCR |= DMA_CCR4_MINC; /* 存储器增 */
    DMA1_Channel4->CCR &= ~DMA_CCR4_PINC; /* 外设不则 因为串口的Tx只有一个寄存器 */

    /* 2.5开启循环模式 */
    DMA1_Channel4->CCR |= DMA_CCR4_CIRC;

    /* 2.5 配置串口的DMA使能位: 使能发送 */
    USART1->CR3 |= USART_CR3_DMAT;

}

/* 开始数据传输  内存的数据通过tx使用dma发送出去*/
void Driver_DMA1_StartTransmit(uint32_t srcAddr, uint32_t desAddr, uint16_t dataLen)
{
    /* 外设地址 */
    DMA1_Channel4->CPAR = desAddr; /* ROM */
    /* 内存地址 */
    DMA1_Channel4->CMAR = srcAddr; /* RAM */
    /* 传输的数据长度  (不是字节数) */
    DMA1_Channel4->CNDTR = dataLen;

    /* 使能通道 */
    DMA1_Channel4->CCR |= DMA_CCR4_EN;
}

