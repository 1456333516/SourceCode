#include "Inf_Ads1115.h"
#include "Driver_I2C2.h"
#include "Delay.h"
#include "Common_Debug.h"

static void     Inf_ADS1115_Config(void);
static uint16_t Inf_ADS1115_ReadConfig(void);



void Inf_ADS1115_Init(void)
{
    /* 1. 初始化I2C */
    Driver_I2C2_Init();

    /* 2. 配置ADS1115 */
    Inf_ADS1115_Config();
}

static void Inf_ADS1115_Config(void)
{
    uint16_t conf = 0x8583;

    /* 1. 配置单端输入模式:  A0 */
    conf &= ~ADS1115_REG_CONFIG_MUX_MASK;
    conf |= ADS1115_REG_CONFIG_MUX_SINGLE_0;

    /* 2. 配置电压的增益 */
    conf &= ~ADS1115_REG_CONFIG_PGA_MASK;
    conf |= ADS1115_REG_CONFIG_PGA_4_096V;

    /* 3. 配置操作模式 0:连续转换  1:单次转换 */
    conf &= ~ADS1115_REG_CONFIG_MODE_MASK;
    conf |= ADS1115_REG_CONFIG_MODE_CONTIN;

    // debug_printfln("写入的配置: 0x%x", conf);

    /* 4. 把配置通过I2C写入到ads115中 */
    /* 4.1 起始信号 */
   // Driver_I2C2_Start();
    /* 4.2 写设备地址 */
   // Driver_I2C2_SendAddr(ADS1115_ADDR_W);
    /* 4.3 向指针寄存器写入01 表示要操作配置寄存器 */
   // Driver_I2C2_SendByte(ADS1115_REG_POINTER_CONFIG);
    /* 4.4 向配置寄存器写入配置的值 */
   // Driver_I2C2_SendByte((conf >> 8) & 0xff);
   // Driver_I2C2_SendByte(conf & 0xff);
    /* 4.5 结束信号*/
   // Driver_I2C2_Stop();

    Delay_ms(1000);
    debug_printfln("修改后的配置: 0x%x", Inf_ADS1115_ReadConfig());
}

static uint16_t Inf_ADS1115_ReadConfig(void)
{
    Driver_I2C2_Start();
    Driver_I2C2_SendAddr(ADS1115_ADDR_W);
    Driver_I2C2_SendByte(ADS1115_REG_POINTER_CONFIG);
    Driver_I2C2_Stop();
    Driver_I2C2_Start();
    Driver_I2C2_SendAddr(ADS1115_ADDR_R);

    uint16_t conf = 0x0;
    Driver_I2C2_Ack(); /* 读之前先准备 好应答或非应答*/
    conf |= Driver_I2C2_ReadByte() << 8;
    conf |= Driver_I2C2_ReadByte();
    Driver_I2C2_Stop();
    return conf;
}

double Inf_ADS1115_ReadValue(void)
{
    Driver_I2C2_Start();
    Driver_I2C2_SendAddr(ADS1115_ADDR_W);
    Driver_I2C2_SendByte(ADS1115_REG_POINTER_CONVERT);
    Driver_I2C2_Stop();
    Driver_I2C2_Start();
    Driver_I2C2_SendAddr(ADS1115_ADDR_R);

    uint16_t v = 0x0000;
    Driver_I2C2_Ack();
    v |= Driver_I2C2_ReadByte() << 8;
    v |= Driver_I2C2_ReadByte();
    Driver_I2C2_Stop();
    // x 伏特
    //x / 4.096 = v / (1 << 15 - 1)

    return 4.096 * v / 32767;
}
