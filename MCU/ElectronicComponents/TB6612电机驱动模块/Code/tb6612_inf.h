/*
 * @Descripttion: 
 * @version: 
 * @Author: tlm
 * @Date: 2024-09-01 16:40:48
 * @LastEditors: tlm
 * @LastEditTime: 2024-09-01 17:45:07
 */
#ifndef __TB6612_H__
#define __TB6612_H__


#include "tim.h"



#define GO 0
#define BACK 1
#define STOP 2

#define AIN1_L HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_RESET)
#define AIN1_H HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_SET)

#define AIN2_L HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_RESET)
#define AIN2_H HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_SET)

#define BIN1_L HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_RESET)
#define BIN1_H HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_SET)

#define BIN2_L HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,GPIO_PIN_RESET)
#define BIN2_H HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,GPIO_PIN_SET)

#define CIN1_L HAL_GPIO_WritePin(CIN1_GPIO_Port,CIN1_Pin,GPIO_PIN_RESET)
#define CIN1_H HAL_GPIO_WritePin(CIN1_GPIO_Port,CIN1_Pin,GPIO_PIN_SET)

#define CIN2_L HAL_GPIO_WritePin(CIN2_GPIO_Port,CIN2_Pin,GPIO_PIN_RESET)
#define CIN2_H HAL_GPIO_WritePin(CIN2_GPIO_Port,CIN2_Pin,GPIO_PIN_SET)

#define DIN1_L HAL_GPIO_WritePin(DIN1_GPIO_Port,DIN1_Pin,GPIO_PIN_RESET)
#define DIN1_H HAL_GPIO_WritePin(DIN1_GPIO_Port,DIN1_Pin,GPIO_PIN_SET)

#define DIN2_L HAL_GPIO_WritePin(DIN2_GPIO_Port,DIN2_Pin,GPIO_PIN_RESET)
#define DIN2_H HAL_GPIO_WritePin(DIN2_GPIO_Port,DIN2_Pin,GPIO_PIN_SET)


void Int_TB6612_SetPWM(int pwma, int pwmb,int pwmc,int pwmd);

#endif
