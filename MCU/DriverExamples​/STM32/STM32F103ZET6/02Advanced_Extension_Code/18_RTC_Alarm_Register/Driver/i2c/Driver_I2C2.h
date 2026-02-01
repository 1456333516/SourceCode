#ifndef __DRIVER_I2C2_H
#define __DRIVER_I2C2_H

#include "Delay.h"
#include "stm32f10x.h"
#include "Driver_USART.h"

#define ACK 0
#define NACK 1

#define SCL_HIGH  (GPIOB->ODR |= GPIO_ODR_ODR10)
#define SCL_LOW  (GPIOB->ODR &= ~GPIO_ODR_ODR10)

#define SDA_HIGH  (GPIOB->ODR |= GPIO_ODR_ODR11)
#define SDA_LOW  (GPIOB->ODR &= ~GPIO_ODR_ODR11)

#define READ_SDA (GPIOB->IDR & GPIO_IDR_IDR11)


void Driver_I2C2_Init(void);

void Driver_I2C2_Start(void);

void Driver_I2C2_Stop(void);

void Driver_I2C2_Ack(void);

void Driver_I2C2_NAck(void);

uint8_t Driver_I2C2_WaitAck(void);

void Driver_I2C_SendByte(uint8_t byte);

uint8_t Driver_I2C_ReadByte(void);

#endif

