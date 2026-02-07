#ifndef __CONTROL_DEVICE_H__
#define __CONTROL_DEVICE_H__

#include "bsp_system.h"

/* 定义器件的触发极性 */
typedef enum {
    ACTIVE_LOW  = 0, // 低电平有效 (如蜂鸣器、风扇)
    ACTIVE_HIGH = 1  // 高电平有效 (如加湿器)
} ActiveLevel_t;

/* 定义器件状态 */
typedef enum {
    DEV_OFF = 0,
    DEV_ON  = 1
} DevState_t;

/* 器件对象结构体 */
typedef struct {
    GPIO_TypeDef* port;       // GPIO端口 (如 GPIOA)
    uint16_t      pin;        // GPIO引脚 (如 GPIO_PIN_5)
    ActiveLevel_t active_lvl; // 激活电平 (高有效还是低有效)
    DevState_t    state;      // 当前状态 (用于记录)
} Device_t;

/* 1. 给你的器件起名字 (ID) */
/* 以后添加新器件，就在这里加一行，不用改逻辑代码 */
typedef enum {
    ID_BUZZER = 0,    // 0: 蜂鸣器
    ID_FAN,           // 1: 风扇
    ID_HUMIDIFIER,    // 2: 加湿器
		ID_HOT,						// 3: 加热片
    // ID_LED_RED,    // 例子: 未来添加的 LED
    
    ID_MAX_COUNT      // 这是一个技巧，自动计算器件总数
} DeviceID_t;




void Device_InitAll(void);
void Device_Toggle(DeviceID_t id);
void Device_App(void);

#endif
