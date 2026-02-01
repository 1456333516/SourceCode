#ifndef __DRIVER_RTC_H
#define __DRIVER_RTC_H

#include "stm32f10x.h"

typedef struct 
{
    uint16_t year;
    uint16_t mon;
    uint16_t day;
    uint16_t hour;
    uint16_t min;
    uint16_t sec;
} DateTimeStruct;



void Driver_RTC_Init(uint32_t unixSecond);
uint32_t Driver_RTC_GetSecond(void);

void Driver_RTC_GetDateTime(DateTimeStruct *dtInf);

void Driver_RTC_SetAlarm(uint16_t alarmSecond);

#endif
