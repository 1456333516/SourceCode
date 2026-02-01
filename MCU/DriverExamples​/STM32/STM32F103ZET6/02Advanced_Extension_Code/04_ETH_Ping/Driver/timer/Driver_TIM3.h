#ifndef __DRIVER_TIM3_H
#define __DRIVER_TIM3_H

#include "stm32f10x.h"
#include "stdio.h"


void Driver_TIM3_Init(void);
void Driver_TIM3_Start(void);
void Driver_TIM3_Stop(void);
double Driver_TIM3_GetPWMCycle(void);
double Driver_TIM3_GetPWMFreq(void);
#endif
