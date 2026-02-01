#include "Common_Debug.h"

#include "Driver_RTC.h"
#include "string.h"
#include "Delay.h"
#include "Inf_LCD.h"

uint8_t dtBuff[50];
int main()
{

    debug_init();
    debug_printfln("尚硅谷水质水位项目");
    Inf_LCD_Init();
    Inf_LCD_SetColor(0, 0, 320, 480, WHITE);

    /* 在window的powshell获取unix时间戳命令:
    [int][double]::Parse((Get-Date -UFormat %s))
    */
    Driver_RTC_Init(1717255835);

    DateTimeStruct dt;
    while(1)
    {
        Driver_RTC_GetDateTime(&dt);
        sprintf((char *)dtBuff,"%04d-%02d-%02d %02d:%02d:%02d",
                       dt.year,
                       dt.mon,
                       dt.day,
                       dt.hour,
                       dt.min,
                       dt.sec);
        Inf_LCD_ShowAsciString(0, 30, 32, dtBuff, BLUE, WHITE);
    }
}
