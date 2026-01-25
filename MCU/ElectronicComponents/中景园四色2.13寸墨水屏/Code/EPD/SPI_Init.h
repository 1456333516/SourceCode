#ifndef _SPI_INIT_H_
#define _SPI_INIT_H_

#include "bsp_system.h"




#define EPD_RES_Clr()    HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, GPIO_PIN_RESET)
#define EPD_RES_Set()    HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, GPIO_PIN_SET)

#define EPD_DC_Clr()     HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET)
#define EPD_DC_Set()     HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET)

#define EPD_CS_Clr()     HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET)
#define EPD_CS_Set()     HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET)

#define EPD_ReadBusy     HAL_GPIO_ReadPin(BUSY_GPIO_Port, BUSY_Pin)


void EPD_WR_Bus(uint8_t dat);	//写入一个字节
void EPD_WR_REG(uint8_t reg);	//写入指令
void EPD_WR_DATA8(uint8_t dat);//写入数据

#endif



