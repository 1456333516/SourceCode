#include "App_WL.h"
#include "Common_Debug.h"
#include "Inf_W25Q32.h"
#include "string.h"
#include "stdlib.h"
#include "Inf_LCD.h"

static void Inf_TIM7711_Calibrate(void);
/*
水位和气压 线性关系
气压和电压 线性关系
=> 水位和电压是线性关系

水位 x  电压 y (电压实时测量)
y = ax + b;
=>
    x = (y - b) / a

    y实时测量. 计算出 a和b

需要提前计算a和b
    x = 0cm  y = y1
    x = 10cm y = y2

    b = y1
    a = (y2 - y1) / 10


*/
void App_WL_Start(void)
{
    Inf_W25Q32_Init();
    Inf_TM7711_Init();
    Inf_TIM7711_Calibrate();
}

static double b, a;

// 0 0 0   存储字符的个数
// 0 0 1   存储具体的数据
// a = 123321.6666  b=13121.7777
// 123321.6666#13121.7777
uint8_t data[50] = {0};
static void Inf_TIM7711_Calibrate(void)
{
    /* 先判断在flash中是否已经存储了a和b, 如果已经存储,直接返回 */
    uint8_t size = 0;
    Inf_W25Q32_ReadBytes(0,0,0, &size, 1);
    debug_printfln("size = %d", size);
    if(size > 0 && size < 255) 
    {
        Inf_W25Q32_ReadBytes(0,0,1, data, size);
        debug_printfln("不是第一次: %s", data);

        /* 123321.6666#13121.7777 */
       a =  strtod(strtok(data, "#"), NULL);   // strtok(data, "#") => "123321.6666"   string=> double
       b =  strtod(strtok(NULL, "#"), NULL); 

       debug_printfln("不是第一次启动:a =  %.4f, b = %.4f", a, b);

       return;
    }
    
    debug_printfln("第一次启动, 需要做校准!");
    /* 需要计算出来a和b的值 */
    debug_printfln("请不要放水里面, 然后按下按键1");
    Inf_LCD_ShowAsciString(10, 130, 24, "Please don't put into water,then press key1         ", BLUE, WHITE);
    /* 等待按键被按下 */
    while(detect_key() != KEY1);
    /* 读取电压值 得到 b */
    uint32_t y1 = Inf_TM7711_ReadValue();
    debug_printfln("请放入水中20cm, 然后按下按键1");
    Inf_LCD_ShowAsciString(10, 130, 24, "Please put into water 20cm,then press key1         ", BLUE, WHITE);
    /* 等待按键被按下 */
    while(detect_key() != KEY1);
    /* 读取电压值 计算 a */
    uint32_t y2 = Inf_TM7711_ReadValue();

    b = y1;
    a = (y2 - y1) / 20.0;
    /* 如果没有存储,则把a和b的值存储到flash */
    Inf_W25Q32_Erase(0,0);
    sprintf(data, "%.4f#%.4f", a, b);
    size = strlen(data); // 97
    Inf_W25Q32_WriteBytes(0,0,0, &size, 1);  
    Inf_W25Q32_WriteBytes(0,0,1, data, size);  
    debug_printfln("data = %s",data);
    debug_printfln("a = %.6f, b = %.6f", a, b);
    Inf_LCD_ShowAsciString(10, 130, 24, "Calibrate done!                                    ", BLUE, WHITE);
}

double App_WL_CalcWL(void)
{
    return (Inf_TM7711_ReadValue() - b) / a;
}
