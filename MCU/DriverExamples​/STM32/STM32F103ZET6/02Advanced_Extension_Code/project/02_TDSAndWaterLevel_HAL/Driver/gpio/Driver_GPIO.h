#ifndef __DRVIER_GPIO_H
#define __DRVIER_GPIO_H

#include "gpio.h"

//#define SCK_H (GPIOB->ODR |= GPIO_ODR_ODR12)
#define SCK_H (HAL_GPIO_WritePin(TM7711_SCK_GPIO_Port, TM7711_SCK_Pin, GPIO_PIN_SET))
//#define SCK_L (GPIOB->ODR &= ~GPIO_ODR_ODR12)
#define SCK_L (HAL_GPIO_WritePin(TM7711_SCK_GPIO_Port, TM7711_SCK_Pin, GPIO_PIN_RESET))

//#define OUT_R (GPIOB->IDR & GPIO_IDR_IDR13)
#define OUT_R (HAL_GPIO_ReadPin(TM7711_OUT_GPIO_Port, TM7711_OUT_Pin))


#define NO_KEY 0
#define KEY1 1
#define KEY2 2
#define KEY3 3
#define KEY4 4

#define READ_KEY1  (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin))
#define READ_KEY2  (GPIOF->IDR & GPIO_IDR_IDR9)
#define READ_KEY3  (GPIOF->IDR & GPIO_IDR_IDR10)
#define READ_KEY4  (GPIOF->IDR & GPIO_IDR_IDR11)

void Driver_GPIO_Init(void);

uint8_t detect_key(void);

#endif 
