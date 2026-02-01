#include "adc.h"

// 初始化
void ADC1_Init(void)
{
    // 1. 时钟配置
    // 1.1 开启 APB2 里的 ADC1 时钟
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    // 1.2 最高14MHz，要做 6 分频，ADCPRE = 10
    RCC->CFGR |= RCC_CFGR_ADCPRE_1;
    RCC->CFGR &= ~RCC_CFGR_ADCPRE_0;

    // 2. GPIO 工作模式：PC0 模拟输入，MODE = 00， CNF = 00
    GPIOC->CRL &= ~GPIO_CRL_MODE0;
    GPIOC->CRL &= ~GPIO_CRL_CNF0;

    // PC2 模拟输入，MODE = 00， CNF = 00
    GPIOC->CRL &= ~GPIO_CRL_MODE2;
    GPIOC->CRL &= ~GPIO_CRL_CNF2;

    // 3. ADC 相关配置
    // 3.1 启用扫描模式
    ADC1->CR1 |= ADC_CR1_SCAN;

    // 3.2 启用连续转换模式
    ADC1->CR2 |= ADC_CR2_CONT;

    // 3.3 数据对齐方式，0 - 右对齐
    ADC1->CR2 &= ~ADC_CR2_ALIGN;

    // 3.4 采样时间，001 - 7.5 个周期
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP10;
    ADC1->SMPR1 |= ADC_SMPR1_SMP10_0;
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP12;
    ADC1->SMPR1 |= ADC_SMPR1_SMP12_0;

    // 3.5 通道序列的配置
    // 3.5.1 序列中要转换的通道数量，L = 0001，表示2个通道
    ADC1->SQR1 &= ~ADC_SQR1_L;
    ADC1->SQR1 |= ADC_SQR1_L_0;

    // 3.5.2 将要转换的通道号添加到序列中，SQ1 = 10（十进制）
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;
    ADC1->SQR3 |= 10 << (0 * 5);
    // SQ2 = 12（十进制）
    ADC1->SQR3 &= ~ADC_SQR3_SQ2;
    ADC1->SQR3 |= 12 << (1 * 5);    // 左移位，配置SQ2
}

// DMA 相关的初始化
void ADC1_DMA_Init(void)
{
    // 1. 配置时钟
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 2. 配置通道1，数据传输方向
    DMA1_Channel1->CCR &= ~DMA_CCR1_DIR;

    // 3. 数据宽度，01 - 16位
    DMA1_Channel1->CCR &= ~DMA_CCR1_PSIZE;
    DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0;
    DMA1_Channel1->CCR &= ~DMA_CCR1_MSIZE;
    DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0;

    // 4. 是否自增：外设不自增，存储器地址自增
    DMA1_Channel1->CCR &= ~DMA_CCR1_PINC;
    DMA1_Channel1->CCR |= DMA_CCR1_MINC;

    // 5. 开启循环模式
    DMA1_Channel1->CCR |= DMA_CCR1_CIRC;

    // 6. ADC1启用 DMA 模式
    ADC1->CR2 |= ADC_CR2_DMA;
}

// 开启转换的函数，带 DMA 通道启动
void ADC1_DMA_StartConvert(uint32_t destAddr, uint8_t dataLen)
{
    // 0. 配置 DMA 的源、目的地址，并开启 DMA 通道
    DMA1_Channel1->CPAR = (uint32_t)&(ADC1->DR);
    DMA1_Channel1->CMAR = destAddr;
    DMA1_Channel1->CNDTR = dataLen;

    DMA1_Channel1->CCR |= DMA_CCR1_EN;

    // 1. 上电：将 ADC 从断电模式中唤醒
    ADC1->CR2 |= ADC_CR2_ADON;

    // 2. 执行校准
    ADC1->CR2 |= ADC_CR2_CAL;
    // 等待硬件清零
    while (ADC1->CR2 & ADC_CR2_CAL)
    {
    }

    // 3. 启动AD转换，在 ADON = 1时再次置1
    ADC1->CR2 |= ADC_CR2_ADON;
    
    // 4. 等待转换完成
    while ((ADC1->SR & ADC_SR_EOC) == 0)
    {
    }
}

// // 返回转换后的数字信号计算成的模拟电压值
// double ADC1_ReadV(void)
// {
//     return ADC1->DR * 3.3 / 4095;
// }
