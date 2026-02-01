#include "Driver_ADC.h"

void Driver_ADC1_Init(void)
{
    /* 1. 开启ADC1的时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    /* 1.1 设置ADC时钟的预分频  RCC_CFGR_ADCREF=10  6分频  72/6=12M */
    RCC->CFGR |= RCC_CFGR_ADCPRE_1;
    RCC->CFGR &= ~RCC_CFGR_ADCPRE_0;

    /* 2. 配置 PC0  PC2 为模拟输入模式 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);
    GPIOC->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2);

    /* 3. ADC的配置 */
    /* 3.1 2个通道, 扫描模式开启 */
    ADC1->CR1 |= ADC_CR1_SCAN;

    /* 3.2 开启连续转换  */
    ADC1->CR2 |= ADC_CR2_CONT;

    /* 3.3 配置数据对齐方式: 右对齐 */
    ADC1->CR2 &= ~ADC_CR2_ALIGN;

    /* 3.4 设置每个通道的采样时间: 010=13.5个周期 */
    ADC1->SMPR1 &= ~(ADC_SMPR1_SMP10_2 | ADC_SMPR1_SMP10_0);
    ADC1->SMPR1 |= ADC_SMPR1_SMP10_1;

    ADC1->SMPR1 &= ~(ADC_SMPR1_SMP12_2 | ADC_SMPR1_SMP12_0);
    ADC1->SMPR1 |= ADC_SMPR1_SMP12_1;

    /* 3.5 配置通道组: 规则组 */
    /* 3.5.1 配置几个通道需要转换  0表示1个通道 1表示2个通道 .... */
    ADC1->SQR1 &= ~ADC_SQR1_L;
    ADC1->SQR1 |= ADC_SQR1_L_0;

    /* 3.5.2 把要转换的通道放入到规则组中 */
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;
    ADC1->SQR3 |= 10 << 0;

    ADC1->SQR3 &= ~ADC_SQR3_SQ2;
    ADC1->SQR3 |= 12 << 5;

    /* 3.6 关闭外部转换 */
    ADC1->CR2 &= ~ADC_CR2_EXTTRIG;

    /* 3.7 选择软件启动方式 */
    ADC1->CR2 |= ADC_CR2_EXTSEL;
}

void Driver_ADC1_DMA_Init(void)
{
    /* 1. 给DMA开启时钟 */
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    /* 2. DMA 的方向  从外设读0 从存储器读是1 */
    DMA1_Channel1->CCR &= ~DMA_CCR1_DIR;

    /* 3. 数据宽度16位 00=8 01=16 10=32 */
    DMA1_Channel1->CCR &= ~DMA_CCR1_MSIZE_1;
    DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0;

    DMA1_Channel1->CCR &= ~DMA_CCR1_PSIZE_1;
    DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0;

    /* 4.外设和内存的地址是否自增 外设不增=0 内存要增=1*/
    DMA1_Channel1->CCR &= ~DMA_CCR1_PINC;
    DMA1_Channel1->CCR |= DMA_CCR1_MINC;

    /* 5.开启DMA的循环模式  */
    DMA1_Channel1->CCR |= DMA_CCR1_CIRC;

    /* 6. 配置ADC1的DMA使能 */
    ADC1->CR2 |= ADC_CR2_DMA;
}

/* 开始转换 */
void Driver_ADC1_DMA_StartConvert(uint32_t des, uint16_t len)
{
    /* 从ADC的数据寄存器搬运 */
    DMA1_Channel1->CPAR  = (uint32_t)(&(ADC1->DR)); /* 外设地址 */
    DMA1_Channel1->CMAR  = des;                     /* 内存地址 */
    DMA1_Channel1->CNDTR = len;                     /* 传输的数据长度 */
    DMA1_Channel1->CCR |= DMA_CCR1_EN;

    /* 1. 上电. 从休眠模式唤醒 */
    ADC1->CR2 |= ADC_CR2_ADON;
    /* 2. 进行adc校准 */
    ADC1->CR2 |= ADC_CR2_CAL;

    /* 等待校准完成 */
    while(ADC1->CR2 & ADC_CR2_CAL)
        ;

    /* 3. 再上电, 可以开始转换 */
    ADC1->CR2 |= ADC_CR2_ADON;

    /* 4. 使用软件开始转换 */
    ADC1->CR2 |= ADC_CR2_SWSTART;

    /* 5. 等待首次转换完成 */
    while((ADC1->SR & ADC_SR_EOC) == 0)
        ;
}
