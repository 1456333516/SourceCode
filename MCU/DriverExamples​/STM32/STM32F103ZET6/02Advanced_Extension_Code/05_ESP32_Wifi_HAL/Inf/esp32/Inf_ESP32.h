#ifndef __INF_ESP32_H
#define __INF_ESP32_H

#include "usart.h"


void Inf_ESP32_Init(void);
void Inf_ESP32_Wifi_Init(void);
void Inf_ESP32_Wifi_StartTcpServer(void);
void Inf_ESP32_Wifi_StartTcpClient(void);
void Inf_ESP32_Wifi_ReceiveTcpData(int8_t *connId, uint8_t ip[], uint32_t *port, uint8_t data[], uint16_t *dataLen);
void Inf_ESP32_Wifi_SendTcpData(int8_t connId, uint8_t data[], uint16_t dataLen);
#endif
