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

    // 3. ADC 相关配置
    // 3.1 不启用扫描模式
    ADC1->CR1 &= ~ADC_CR1_SCAN;

    // 3.2 启用连续转换模式
    ADC1->CR2 |= ADC_CR2_CONT;

    // 3.3 数据对齐方式，0 - 右对齐
    ADC1->CR2 &= ~ADC_CR2_ALIGN;

    // 3.4 采样时间，001 - 7.5 个周期
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP10;
    ADC1->SMPR1 |= ADC_SMPR1_SMP10_0;

    // 3.5 通道序列的配置
    // 3.5.1 序列中要转换的通道数量，L = 000，表示1个通道
    ADC1->SQR1 &= ~ADC_SQR1_L;

    // 3.5.2 将要转换的通道号添加到序列中，SQ1 = 10（十进制）
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;
    ADC1->SQR3 |= 10 << (0 * 5);
}

// 开启转换的函数
void ADC1_StartConvert(void)
{
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

// 返回转换后的数字信号计算成的模拟电压值
double ADC1_ReadV(void)
{
    return ADC1->DR * 3.3 / 4095;
}
