/*
 * scheduler.c
 * 调度器
 *  Created on: 2025年7月15日
 *      Author: 14563
 */

#include "scheduler.h"

uint32_t uwtick;//调度器时间
void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));//定时器3快速中断

void Tim3_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseInitStruct.TIM_ClockDivision=0;//时钟分割
    TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//设置为向上计数
    TIM_TimeBaseInitStruct.TIM_Period=arr;//自动重装载值，5000为500ms
    TIM_TimeBaseInitStruct.TIM_Prescaler=psc;//设置分频值

    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
    TIM_ITConfig(TIM3, TIM_IT_Update|TIM_IT_Trigger, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM3, ENABLE);
}

/*任务调度器相关*/
typedef struct
{
    void (*task_func)(void);//任务函数
    unsigned long int rate_ms;  //任务执行周期
    unsigned long int last_run; //任务上次运行的时间
}task_t;
void test(void)
{
    printf("test\r\n");


}
task_t scheduler_task[]={
        {test,1000,0},//屏幕处理函数，100毫秒执行一次，0秒开始执行

};
unsigned char task_num;  //任务数量变量
void scheduler_init()
{
    task_num=sizeof(scheduler_task)/sizeof(task_t);  //计算任务数量
}
void scheduler_run()
{
    unsigned char i;
    for(i=0;i<task_num;i++)
    {
        unsigned long int now_time=uwtick;  //更新当前系统时间
        if(now_time> (scheduler_task[i].last_run+scheduler_task[i].rate_ms) )
        {
            scheduler_task[i].last_run=now_time;//记录本次运行时间
            scheduler_task[i].task_func();//运行任务
        }
    }
}





/*系统计时定时器3中断服务函数*/
void TIM3_IRQHandler(void)//每1毫秒
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update)!=RESET)
    {
        uwtick++;//系统计时变量自增
//      uart7_rec_tick++;
//      uart6_rec_tick++;
    }
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}







