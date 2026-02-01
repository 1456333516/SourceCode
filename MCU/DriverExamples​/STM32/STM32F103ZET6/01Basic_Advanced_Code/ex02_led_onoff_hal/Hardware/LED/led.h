/*
 * @Author: wushengran
 * @Date: 2024-04-24 16:25:31
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
/*
 * @Author: wushengran
 * @Date: 2024-04-24 09:24:35
 * @Description: 
 * 
 * Copyright (c) 2024 by atguigu, All Rights Reserved. 
 */
#ifndef __LED_H
#define __LED_H

#include "gpio.h"

#define LED uint16_t

// 开关某个LED灯
void LED_On(LED led);
void LED_Off(LED led);

// 翻转某个LED灯状态
void LED_Toggle(LED led);

// 全开/全关
void LED_OnAll(LED leds[], uint8_t size);
void LED_OffAll(LED leds[], uint8_t size);

#endif
