#ifndef __INF_ESP32_H
#define __INF_ESP32_H

#include "usart.h"


void Inf_ESP32_Init(void);
void Inf_ESP32_BLE_Init(void);

void Inf_ESP32_BLE_ReadData(uint8_t data[], uint16_t *dataLen);

void Inf_ESP32_BLE_SendData(uint8_t data[], uint16_t dataLen);

#endif
