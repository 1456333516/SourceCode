#ifndef __APP_UDP_H
#define __APP_UDP_H
#include "w5500.h"
#include "socket.h"
#include "stdio.h"



void App_UDP_ReceiveData(uint8_t data[], uint16_t *dataLen, uint8_t *rIp, uint16_t *rPort);
void App_UDP_SendData(uint8_t data[], uint16_t dataLen, uint8_t *sIp, uint16_t sPort);
#endif
