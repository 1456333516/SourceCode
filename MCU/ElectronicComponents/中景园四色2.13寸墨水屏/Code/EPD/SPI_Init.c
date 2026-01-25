#include "SPI_Init.h"

//void EPD_GPIOInit(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(EPD_SCL_GPIO_CLK|EPD_SDA_GPIO_CLK|EPD_RES_GPIO_CLK
//												 |EPD_DC_GPIO_CLK|EPD_CS_GPIO_CLK|EPD_BUSY_GPIO_CLK,ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin=EPD_SCL_GPIO_PIN;
//  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(EPD_SCL_GPIO_PORT,&GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin=EPD_SDA_GPIO_PIN;
//  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(EPD_SDA_GPIO_PORT,&GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin=EPD_RES_GPIO_PIN;
//  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(EPD_RES_GPIO_PORT,&GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin=EPD_DC_GPIO_PIN;
//  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(EPD_DC_GPIO_PORT,&GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin=EPD_CS_GPIO_PIN;
//  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(EPD_CS_GPIO_PORT,&GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin=EPD_BUSY_GPIO_PIN;
//  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;
//	GPIO_Init(EPD_BUSY_GPIO_PORT,&GPIO_InitStructure);
//	
//}


extern SPI_HandleTypeDef hspi1;  // 假设 SPI1 配置已经初始化

void EPD_WR_Bus(uint8_t dat)
{
    EPD_CS_Clr();  // 拉低片选

    // 使用 SPI 发送 8 位数据
    HAL_SPI_Transmit(&hspi1, &dat, 1, HAL_MAX_DELAY);  // 发送一个字节的数据

    EPD_CS_Set();  // 拉高片选
}

void EPD_WR_REG(uint8_t reg)
{
    EPD_DC_Clr();  // 选择寄存器操作
    EPD_WR_Bus(reg);  // 写寄存器
    EPD_DC_Set();  // 切换到数据操作
}

void EPD_WR_DATA8(uint8_t dat)
{
    EPD_DC_Set();  // 选择数据操作
    EPD_WR_Bus(dat);  // 写数据
    EPD_DC_Set();  // 保持数据操作
}




