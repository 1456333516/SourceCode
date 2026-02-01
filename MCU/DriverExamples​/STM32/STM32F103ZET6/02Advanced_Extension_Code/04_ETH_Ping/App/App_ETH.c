
#include "App_ETH.h"
#include "Driver_SPI.h"
#include "w5500.h"
extern void user_register_function(void);

static void App_Eth_RST(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPGEN;
    /* pg7:  推挽输出  mode=11 cnf=00 */
    GPIOG->CRL |= GPIO_CRL_MODE7;
    GPIOG->CRL &= ~GPIO_CRL_CNF7;

    /* RST引脚拉低不低于500us */
    GPIOG->ODR &= ~GPIO_ODR_ODR7;
    Delay_ms(1);
    GPIOG->ODR |= GPIO_ODR_ODR7;
    Delay_ms(100);
}

uint8_t ip[]  = {192, 168, 32, 223};
uint8_t ga[]  = {192, 168, 32, 1};
uint8_t sub[] = {255, 255, 255, 0};
uint8_t mac[] = {110, 120, 13, 140, 150, 160};
void    App_Eth_Init(void)
{
    /* 初始化spi驱动 */
    Driver_SPI_Init();

    /* 软重启芯片 */
    App_Eth_RST();

    /* 注册函数 */
    user_register_function();

    /* 设置ip地址相关 */

    /* ip地址 */
    setSIPR(ip);
    /* mac地址 */
    setSHAR(mac);
    /* 子网掩码 */
    setSUBR(sub);
    /*  设置网关 */
    setGAR(ga);

    printf("ip地址设置完成\r\n");
}