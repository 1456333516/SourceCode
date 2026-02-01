#ifndef __DRVIER_USART_H
#define __DRVIER_USART_H

#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"

void Driver_USART1_Init(void);
void Driver_USART1_Start(void);
void Driver_USART1_Stop(void);

void Driver_USART1_SendChar(uint8_t byte);
void Driver_USART1_SendString(uint8_t *str, uint16_t len);

#endif
