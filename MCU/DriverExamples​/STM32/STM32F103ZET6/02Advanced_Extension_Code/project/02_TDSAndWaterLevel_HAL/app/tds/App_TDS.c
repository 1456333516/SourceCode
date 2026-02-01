#include "App_TDS.h"

void App_TDS_Start(void)
{
    Inf_ADS1115_Init();
}

double App_TDS_CalcTDS(void)
{
    
    double v = Inf_ADS1115_ReadValue();
    double vv = v * v;
    return 66.71 * vv * v - 127.93 * vv + 428.7 * v;
}
