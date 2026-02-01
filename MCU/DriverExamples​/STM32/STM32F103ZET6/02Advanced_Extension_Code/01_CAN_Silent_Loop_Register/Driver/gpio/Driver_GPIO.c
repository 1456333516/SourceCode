#include "Driver_GPIO.h"
#include "Delay.h"

void Driver_GPIO_Init(void)
{
    /*
        pb12 sck : 通用推挽  mode=11 cnf=00
        pb13 out:  浮空输入  mode=00 cnf=01
     */

    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    GPIOB->CRH |= (GPIO_CRH_MODE12 | GPIO_CRH_CNF13_0);
    GPIOB->CRH &= ~(GPIO_CRH_CNF12 | GPIO_CRH_MODE13 | GPIO_CRH_CNF13_1);

    /* 按键gpio的初始化 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
    /* pf8: 上拉输入 mode=00 cnf=10  odr=1 */
    GPIOF->CRH &= ~(GPIO_CRH_MODE8 | GPIO_CRH_CNF8_0);
    GPIOF->CRH |= GPIO_CRH_CNF8_1;
    GPIOF->ODR |= GPIO_ODR_ODR8;

    GPIOF->CRH &= ~(GPIO_CRH_MODE9 | GPIO_CRH_CNF9_0);
    GPIOF->CRH |= GPIO_CRH_CNF9_1;
    GPIOF->ODR |= GPIO_ODR_ODR9;
}

/* 检测按按键按下:
如果按键1被按下返回1
如果按键2被按下返回2
*/
uint8_t detect_key(void)
{
    /* 表示key1或者key2按下 */
    if(READ_KEY1 == 0 || READ_KEY2 == 0)
    {
        Delay_ms(50);
        if(READ_KEY1 == 0)
        {
            /* 等待松开 */
            while(READ_KEY1 == 0);
            return KEY1;
        }
        else if(READ_KEY2 == 0)
        {
            while(READ_KEY2 == 0);
            return KEY2;
        }
    }
    return NO_KEY;
}
