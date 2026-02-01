#ifndef __INF_TM7711_H
#define __INF_TM7711_H

#include "Driver_GPIO.h"
#include "Delay.h"

#define TM7711_DELAY  (Delay_us(5))


void Inf_TM7711_Init(void);

uint32_t Inf_TM7711_ReadValue(void);
#endif 