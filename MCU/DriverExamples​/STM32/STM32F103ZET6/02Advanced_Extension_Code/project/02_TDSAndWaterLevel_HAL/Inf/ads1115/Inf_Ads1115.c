#include "Inf_Ads1115.h"
#include "i2c.h"

#include "Common_Debug.h"

static void Inf_ADS1115_Config(void);

void Inf_ADS1115_Init(void)
{
    MX_I2C2_Init();

    /* 2. 配置ADS1115 */
    Inf_ADS1115_Config();
}

void Inf_ADS1115_ReadConfig(void)
{

    uint8_t cmd = ADS1115_REG_POINTER_CONFIG;
    HAL_I2C_Master_Transmit(&hi2c2, ADS1115_ADDR_W, &cmd, 1, 2000);

    uint8_t data[2] = {0};
    HAL_I2C_Master_Receive(&hi2c2, ADS1115_ADDR_R, data, 2, 2000);

    debug_printfln("config = %d", (data[0] << 8 + data[1]));
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

    uint8_t data[3] = {ADS1115_REG_POINTER_CONFIG, (conf >> 8) & 0xff, conf & 0xff};

    HAL_I2C_Master_Transmit(&hi2c2, ADS1115_ADDR_W, data, 3, 2000);

    // Delay_ms(1000);
    Inf_ADS1115_ReadConfig();
}

#include "Common_Debug.h"

double Inf_ADS1115_ReadValue(void)
{

    uint8_t cmd = ADS1115_REG_POINTER_CONVERT;
    HAL_I2C_Master_Transmit(&hi2c2, ADS1115_ADDR_W, &cmd, 1, 2000);

    
    uint8_t data[2] = {0};
    HAL_I2C_Master_Receive(&hi2c2, ADS1115_ADDR_R, data, 2, 2000);

   
    return 4.096 * ((data[0] << 8) + data[1]) / 32767;
}
