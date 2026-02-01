#ifndef __DRVIER_GPIO_H
#define __DRVIER_GPIO_H

#include "stm32f10x.h"

#define SCK_H (GPIOB->ODR |= GPIO_ODR_ODR12)
#define SCK_L (GPIOB->ODR &= ~GPIO_ODR_ODR12)

#define OUT_R (GPIOB->IDR & GPIO_IDR_IDR13)


#define NO_KEY 0
#define KEY1 1
#define KEY2 2
#define KEY3 3
#define KEY4 4

#define READ_KEY1  (GPIOF->IDR & GPIO_IDR_IDR8)
#define READ_KEY2  (GPIOF->IDR & GPIO_IDR_IDR9)
#define READ_KEY3  (GPIOF->IDR & GPIO_IDR_IDR10)
#define READ_KEY4  (GPIOF->IDR & GPIO_IDR_IDR11)

void Driver_GPIO_Init(void);

uint8_t detect_key(void);

#endif 