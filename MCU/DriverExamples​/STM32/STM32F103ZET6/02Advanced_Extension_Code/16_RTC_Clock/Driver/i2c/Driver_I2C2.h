#ifndef __DRIVER_I2C2_H
#define __DRIVER_I2C2_H

#include "stm32f10x.h"

#define OK 0
#define FAIL 1 

void Driver_I2C2_Init(void);

uint8_t Driver_I2C2_Start(void);

void Driver_I2C2_Stop(void);

void Driver_I2C2_Ack(void);

void Driver_I2C2_NoAck(void);

uint8_t Driver_I2C2_SendAddr(uint8_t addr);

uint8_t Driver_I2C2_SendByte(uint8_t byte);

uint8_t Driver_I2C2_ReadByte(void);

#endif 
