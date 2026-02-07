#include "control_device.h"


/* 2. 配置器件参数表 */
/* 顺序必须和上面的枚举一一对应 */
Device_t Device_List[ID_MAX_COUNT] = {
    /* 端口             引脚            有效电平(极性)    初始状态 */
    {FMD_GPIO_Port,    FMD_Pin,        ACTIVE_LOW,      DEV_OFF}, // 蜂鸣器
    {FAN_GPIO_Port,    FAN_Pin,        ACTIVE_LOW,      DEV_OFF}, // 风扇
    {GPIOB,            HUMIDIFIER_Pin, ACTIVE_HIGH,     DEV_OFF}, // 加湿器
		{HOT_GPIO_Port,		 HOT_Pin,				 ACTIVE_HIGH,			DEV_OFF},	// 加热片
		
};



/**
 * @brief 统一控制函数
 * @param id:    器件ID (如 ID_FAN)
 * @param state: 目标状态 (DEV_ON 或 DEV_OFF)
 */
void Device_SetState(DeviceID_t id, DevState_t state)
{
    // 1. 安全检查，防止数组越界
    if (id >= ID_MAX_COUNT) return;

    // 2. 更新软件记录的状态
    Device_List[id].state = state;

    // 3. 计算物理电平
    // 如果想要开启 (ON): 输出应该等于“有效电平”
    // 如果想要关闭 (OFF): 输出应该等于“有效电平的反”
    GPIO_PinState pin_state;
    
    if (state == DEV_ON) {
        // 要开启：如果是高有效就输出SET，低有效就输出RESET
        pin_state = (Device_List[id].active_lvl == ACTIVE_HIGH) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    } else {
        // 要关闭：与开启相反
        pin_state = (Device_List[id].active_lvl == ACTIVE_HIGH) ? GPIO_PIN_RESET : GPIO_PIN_SET;
    }

    // 4. 执行硬件操作
    HAL_GPIO_WritePin(Device_List[id].port, Device_List[id].pin, pin_state);
}

/**
 * @brief 初始化所有器件 (在 main 中调用一次)
 * 确保上电时所有设备都是关闭的
 */
void Device_InitAll(void)
{
    for (int i = 0; i < ID_MAX_COUNT; i++)
    {
        /* 修改这一行，加上强制转换 (DeviceID_t) */
        Device_SetState((DeviceID_t)i, DEV_OFF);
    }
}


void Device_Toggle(DeviceID_t id)
{
    // 1. 获取当前状态
    DevState_t state = Device_List[id].state;

    // 2. 反转状态
    if (state == DEV_ON) {
        state = DEV_OFF;
    } else {
        state = DEV_ON;
    }

    // 3. 统一控制函数
    Device_SetState(id, state);
    
}


void Device_App(void)
{
    // 开启风扇
    // Device_SetState(ID_FAN, DEV_ON);        
    // 开启加湿器
    // Device_SetState(ID_HUMIDIFIER, DEV_ON);
		// Device_Toggle(ID_FAN);
		// Device_Toggle(ID_HUMIDIFIER);
		// Device_Toggle(ID_BUZZER);
		// Device_Toggle(ID_HOT);
}
