#ifndef __DRIVER_CAN_H
#define __DRIVER_CAN_H

#include "stm32f10x.h"
#include "Common_Debug.h"

typedef struct 
{   
    uint16_t id;
    uint8_t data[8];
    uint8_t len;
    
} RxMsgStruct;


void Driver_CAN_Init(void);
void Driver_CAN_SendMsg(uint16_t id, uint8_t *data, uint8_t len);
void Driver_CAN_RecevieMsgs(RxMsgStruct msgs[], uint8_t *count);
#endif
