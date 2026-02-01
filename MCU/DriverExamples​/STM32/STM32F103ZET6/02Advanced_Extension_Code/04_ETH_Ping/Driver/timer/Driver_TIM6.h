#ifndef __DRIVER_TIM6_H
#define __DRIVER_TIM6_H

#include "stm32f10x.h"
#include "stdio.h"
#include "Driver_LED.h"

void Driver_TIM6_Init(void);
void Driver_TIM6_Start(void);
void Driver_TIM6_Stop(void);
#endif
