/*
 * @Descripttion: 此文件为stm32hal库使用tb6612模块的代码，以下采用四路，如果只需要两路，可以把其它两路注释掉
 *                接口说明：需要将stm32控制tb6612的IO口设置为AIN1,AIN2,BIN1,BIN2,CIN1,CIN2,DIN1,DIN2。设置模式为推挽输出
 *                         tb6612模块上的STBY要与高电平相连，此时才能驱动tb6612模块
 *                         还需要设置四个定时器通道，在此模版上设置的是TIM1的四个通道
 * 
 * @version: 1.0
 * @Author: tlm
 * @Date: 2024-09-06 00:26:22
 * @LastEditors: tlm
 * @LastEditTime: 2024-09-06 12:26:12
 */
#include "tb6612.h"


/**
 * @description: 控制电机A的方向
 * @param {uint8_t} direct 期望的转动方向
 * @return {*}
 */
void Int_TB6612_MotorA(uint8_t direct)
{
    if(direct == GO)
    {
        /* 正转：AIN1=0,AIN2=1 */
       AIN1_L;
       AIN2_H;
    }
    else if(direct == BACK)
    {
        /* 反转：AIN1=1,AIN2=0 */
        AIN1_H;
        AIN2_L;
    }
    else 
    {
        /* 其他刹车：AIN1=1,AIN2=1 */
        AIN1_H;
        AIN2_H;
    }
}

/**
 * @description: 控制电机B的方向
 * @param {uint8_t} direct 期望的转动方向
 * @return {*}
 */
void Int_TB6612_MotorB(uint8_t direct)
{
    if(direct == GO)
    {
        /* 正转：BIN1=0,BIN2=1 */
       BIN1_L;
       BIN2_H;
    }
    else if(direct == BACK)
    {
        /* 反转：BIN1=1,BIN2=0 */
        BIN1_H;
        BIN2_L;
    }
    else 
    {
        /* 其他刹车：BIN1=1,BIN2=1 */
        BIN1_H;
        BIN2_H;
    }
}

/**
 * @description: 控制电机C的方向
 * @param {uint8_t} direct 期望的转动方向
 * @return {*}
 */
void Int_TB6612_MotorC(uint8_t direct)
{
    if(direct == GO)
    {
        /* 正转：CIN1=0,CIN2=1 */
       CIN1_L;
       CIN2_H;
    }
    else if(direct == BACK)
    {
        /* 反转：CIN1=1,CIN2=0 */
        CIN1_H;
        CIN2_L;
    }
    else 
    {
        /* 其他刹车：CIN1=1,CIN2=1 */
        CIN1_H;
        CIN2_H;
    }
}

/**
 * @description: 控制电机D的方向
 * @param {uint8_t} direct 期望的转动方向
 * @return {*}
 */
void Int_TB6612_MotorD(uint8_t direct)
{
    if(direct == GO)
    {
        /* 正转：DIN1=0,DIN2=1 */
       DIN1_L;
       DIN2_H;
    }
    else if(direct == BACK)
    {
        /* 反转：DIN1=1,DIN2=0 */
        DIN1_H;
        DIN2_L;
    }
    else 
    {
        /* 其他刹车：DIN1=1,DIN2=1 */
        DIN1_H;
        DIN2_H;
    }
}


/**
 * @description: 根据带符号的pwm值，设置转速和方向
 * @param {int} pwma   A电机，带符号
 * @param {int} pwmb   B电机，带符号
 * @param {int} pwmc   C电机，带符号
 * @param {int} pwmd   D电机，带符号
 * @return {*}
 */
void Int_TB6612_SetPWM(int pwma,int pwmb,int pwmc,int pwmd)
{
    /* 根据带符号的pwm值：1、处理方向；2、设置pwm占空比 */


    /* 1.处理pwma */
    /* 1.1 根据符号处理方向 */
    if(pwma > 0)
    {
        /* 正转 */
        Int_TB6612_MotorA(GO);
    }
    else if(pwma < 0)
    {
        /* 反转 */
        Int_TB6612_MotorA(BACK);
        /* 为后续设置ccr做准备，取绝对值 */
        pwma = -pwma;
    }
    else 
    {
        /* 刹车 */
        Int_TB6612_MotorA(STOP);
    }
    /* 1.2 设置对应的pwm值 =》 对应定时器通道CH1的ccr值 */
    __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,pwma);


    /* 2.处理pwmb */
    /* 1.1 根据符号处理方向 */
    if(pwmb > 0)
    {
        /* 正转 */
        Int_TB6612_MotorB(GO);
    }
    else if(pwmb < 0)
    {
        /* 反转 */
        Int_TB6612_MotorB(BACK);
        /* 为后续设置ccr做准备，取绝对值 */
        pwmb = -pwmb;
    }
    else 
    {
        /* 刹车 */
        Int_TB6612_MotorB(STOP);
    }
    /* 1.2 设置对应的pwm值 =》 对应定时器通道CH2的ccr值 */
    __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,pwmb);

    /* 1.处理pwmc */
    /* 1.1 根据符号处理方向 */
    if(pwmc > 0)
    {
        /* 正转 */
        Int_TB6612_MotorC(GO);
    }
    else if(pwmc < 0)
    {
        /* 反转 */
        Int_TB6612_MotorC(BACK);
        /* 为后续设置ccr做准备，取绝对值 */
        pwmc = -pwmc;
    }
    else 
    {
        /* 刹车 */
        Int_TB6612_MotorC(STOP);
    }
    /* 1.2 设置对应的pwm值 =》 对应定时器通道CH3的ccr值 */
    __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_3,pwmc);


    /* 2.处理pwmd */
    /* 1.1 根据符号处理方向 */
    if(pwmd > 0)
    {
        /* 正转 */
        Int_TB6612_MotorD(GO);
    }
    else if(pwmd < 0)
    {
        /* 反转 */
        Int_TB6612_MotorD(BACK);
        /* 为后续设置ccr做准备，取绝对值 */
        pwmd = -pwmd;
    }
    else 
    {
        /* 刹车 */
        Int_TB6612_MotorD(STOP);
    }
    /* 1.2 设置对应的pwm值 =》 对应定时器通道CH4的ccr值 */
    __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_4,pwmd);
}
