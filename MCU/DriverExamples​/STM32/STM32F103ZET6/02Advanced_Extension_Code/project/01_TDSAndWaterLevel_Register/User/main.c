#include "Common_Debug.h"

#include "string.h"
#include "App_Display.h"

#include "App_TDS.h"
#include "App_WL.h"

#include "Inf_W25Q32.h"

int main()
{

    debug_init();
    debug_printfln("尚硅谷水质水位项目");

    /* 1. 初始化显示模块 */
    App_Display_Init();
    /* 2. 显示logo */
    App_Display_ShowLogo();
    App_Display_ShowTitle();

    /* 3. 启动tds采集模块 */
    App_TDS_Start();

    /* 4. 启动水位测量模块 */
    App_WL_Start();

    while(1)
    {
        /* 4. 计算tds的值 */
        double tds = App_TDS_CalcTDS();
        /* 5.显示tds的值 */
        App_Display_ShowTDS(tds);

        /*6 显示水位的值 */
        App_Display_ShowWL(App_WL_CalcWL());
        debug_printfln("wl = %.2f", App_WL_CalcWL());
        
        if(detect_key() == KEY2)
        {
            Inf_W25Q32_Erase(0, 0);
        }
    }
}
