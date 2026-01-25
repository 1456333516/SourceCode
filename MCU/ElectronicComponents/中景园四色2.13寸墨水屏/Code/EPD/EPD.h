#ifndef _EPD_H_
#define _EPD_H_

#include "bsp_system.h"

#define EPD_W  122 
#define EPD_H  250

#define Source_Bits 128
#define Gate_Bits   250

#define BLACK  0x00
#define WHITE  0x01
#define YELLOW 0x02
#define RED    0x03

void EPD_READBUSY(void);
void EPD_HW_RESET(void);
void EPD_Update(void);
void EPD_DeepSleep(void);
void EPD_Init(void);
uint8_t Color_Conversion(uint8_t color);
void EPD_Display_Fill(uint8_t dat);
void EPD_Display(const uint8_t *image);
#endif




