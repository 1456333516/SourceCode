#include "Driver_RTC.h"
#include "time.h"

void Driver_RTC_Init(uint32_t unixSecond)
{

    /* 开启相关时钟 */
    RCC->APB1ENR |= (RCC_APB1ENR_BKPEN | RCC_APB1ENR_PWREN);

    /* 先判断是否第一次 */
    debug_printfln("%d", BKP->DR2);
    if(BKP->DR2 == 111)
    {
        debug_printfln("不是第一次");
        return;
    }
    else
    {
        debug_printfln("第一次");
    }
    /* 解除后备域的写保护 */
    PWR->CR |= PWR_CR_DBP;

    /* 先清除整个备份域:   要更改RTC的时钟源 */
    RCC->BDCR |= RCC_BDCR_BDRST;
    RCC->BDCR &= ~RCC_BDCR_BDRST; /* 解除复位 */

    BKP->DR2 = 111;

    /* 1. 时钟源相关配置 */
    /* 先配置RTC的时钟源: LSE */
    RCC->BDCR &= ~RCC_BDCR_RTCSEL;
    RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;

    /* 使能LSE */
    RCC->BDCR |= RCC_BDCR_LSEON;

    /* 使能RTC时钟使能 */
    RCC->BDCR |= RCC_BDCR_RTCEN;

    /* 2. 配置RTC相关 */
    /* 2.1 等待允许操作预分频和计数器 */
    while((RTC->CRL & RTC_CRL_RTOFF) == 0);

    /* 2.2 进入配置模式 */
    RTC->CRL |= RTC_CRL_CNF;

    /* 2.3 配置预分频
        0xffffff >> 16 & 0xf
    */
    RTC->PRLH = (0x7fff >> 16) & 0xf;
    RTC->PRLL = 0x7fff & 0xffff;

    /* 2.4 配置计数器 */
    RTC->CNTH = unixSecond >> 16;
    RTC->CNTL = unixSecond & 0xffff;

    /* 2. 退出配置模式 */
    RTC->CRL &= ~RTC_CRL_CNF;
}

uint32_t Driver_RTC_GetSecond(void)
{
    /* 等待计数器的值同步完成 */
    while((RTC->CRL & RTC_CRL_RSF) == 0);

    return (RTC->CNTH << 16) + RTC->CNTL;
}

void Driver_RTC_GetDateTime(DateTimeStruct *dtInf)
{
    uint32_t s = Driver_RTC_GetSecond();

    struct tm *dt = localtime(&s);

    dtInf->year = dt->tm_year + 1900;
    dtInf->mon  = dt->tm_mon + 1;
    dtInf->day  = dt->tm_mday;
    dtInf->hour = dt->tm_hour;
    dtInf->min  = dt->tm_min;
    dtInf->sec  = dt->tm_sec;
}