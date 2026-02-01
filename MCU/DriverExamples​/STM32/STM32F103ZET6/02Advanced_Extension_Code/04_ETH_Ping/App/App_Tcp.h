#ifndef __APP_TCP_H
#define __APP_TCP_H
#include "w5500.h"
#include "socket.h"
#include "stdio.h"


void App_Tcp_ServerStart(void);
void App_Tcp_ClientStart(void);
void App_Tcp_ReceiveData(uint8_t data[], uint16_t *dataLen);
void App_Tcp_SendData(uint8_t data[], uint16_t dataLen);
#endif
