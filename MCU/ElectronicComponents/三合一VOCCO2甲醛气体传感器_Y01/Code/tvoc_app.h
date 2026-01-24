#ifndef __TVOC_APP_C__
#define __TVOC_APP_C__






#include "bsp_system.h"

// 定义存储解析结果的结构体
typedef struct {
    float tvoc_mg_m3;    // TVOC 浓度 (mg/m3)
    float hcho_mg_m3;    // 甲醛 (HCHO/CH2O) 浓度 (mg/m3)
    uint16_t co2_ppm;    // 二氧化碳 (CO2) 浓度 (ppm)
} AirQuality_Data_t;
void tvoc_task(void);


#endif
