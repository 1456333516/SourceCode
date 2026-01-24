/*
 * @Descripttion: 调度器
 * @version: V1.0
 * @Author: tlm
 * @Date: 2024-12-29 14:46:03
 * @LastEditors: tlm
 * @LastEditTime: 2024-12-29 15:44:21
 */
#include "scheduler.h"

typedef struct{
    void (*task_func)(void);    //任务函数
    uint32_t rate_ms;           //任务执行周期
    uint32_t last_run;           //上次执行时间
}task_t;

uint8_t task_num;//任务数量


void printf_task(void)
{
	debug_printf("hello\r\n");
}

static task_t scheduler_task[] =
{
    //{printf_task,1000,0},

};

/**
 * @deseription: 调度器初始化，计算任务数量
 * @return {*}
 */
void scheduler_init(void)
{
    task_num = sizeof(scheduler_task)/sizeof(task_t);
}

/**
 * @deseription: 执行调度器
 * @return {*}
 */
void scheduler_run(void)
{

    for(uint8_t i=0;i<task_num;i++)
    {
        uint32_t now_time = HAL_GetTick();
        if(now_time >= scheduler_task[i].last_run + scheduler_task[i].rate_ms)
        {

            scheduler_task[i].last_run = now_time;
            scheduler_task[i].task_func();
        }

    }
}










