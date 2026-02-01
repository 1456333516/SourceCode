#include "Driver_USART.h"
#include "Driver_LED.h"
#include "Delay.h"
#include "Driver_GPIO.h"
#include "Driver_IWDG.h"

int main()
{
    Driver_USART1_Init();
    printf("程序开始\r\n");

    Driver_GPIO_Init();

    Driver_IWDG_Init();

   
    while(1)
    {
        Driver_IWDG_Refresh();
        printf("正常正常要监控的代码 开始...\r\n");
        Delay_ms(4000);
        printf("正常正常要监控的代码  结束...\r\n");
        Driver_IWDG_Refresh();
    }
}
