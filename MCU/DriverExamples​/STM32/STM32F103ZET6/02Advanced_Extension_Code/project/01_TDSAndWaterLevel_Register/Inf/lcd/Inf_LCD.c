#include "Inf_LCD.h"
#include "Inf_LCD_Font.h"

void Inf_LCD_Restart(void)
{
    GPIOG->ODR &= ~GPIO_ODR_ODR15;
    Delay_ms(800);
    GPIOG->ODR |= GPIO_ODR_ODR15;
    Delay_ms(100);
}

void Inf_LCD_OpenBg(void)
{
    GPIOB->ODR |= GPIO_ODR_ODR0;
}

void Inf_LCD_CloseBg(void)
{
    GPIOB->ODR &= ~GPIO_ODR_ODR0;
}

/* 写命令 */
void Inf_LCD_WriteCmd(uint16_t cmd)
{
    *LCD_CMD_ADDR = cmd;
}

/* 写数据 */
void Inf_LCD_WriteData(uint16_t data)
{
    *LCD_DATA_ADDR = data;
}

uint16_t Inf_LCD_ReadData(void)
{
    return *LCD_DATA_ADDR;
}

// TODO
/* 初始化寄存器的值 */
void Inf_LCD_RegConfig(void)
{
    /* 1. 设置灰阶电压以调整TFT面板的伽马特性， 正校准。一般出厂就设置好了 */
    Inf_LCD_WriteCmd(0xE0);
    Inf_LCD_WriteData(0x00);
    Inf_LCD_WriteData(0x07);
    Inf_LCD_WriteData(0x10);
    Inf_LCD_WriteData(0x09);
    Inf_LCD_WriteData(0x17);
    Inf_LCD_WriteData(0x0B);
    Inf_LCD_WriteData(0x41);
    Inf_LCD_WriteData(0x89);
    Inf_LCD_WriteData(0x4B);
    Inf_LCD_WriteData(0x0A);
    Inf_LCD_WriteData(0x0C);
    Inf_LCD_WriteData(0x0E);
    Inf_LCD_WriteData(0x18);
    Inf_LCD_WriteData(0x1B);
    Inf_LCD_WriteData(0x0F);

    /* 2. 设置灰阶电压以调整TFT面板的伽马特性，负校准 */
    Inf_LCD_WriteCmd(0XE1);
    Inf_LCD_WriteData(0x00);
    Inf_LCD_WriteData(0x17);
    Inf_LCD_WriteData(0x1A);
    Inf_LCD_WriteData(0x04);
    Inf_LCD_WriteData(0x0E);
    Inf_LCD_WriteData(0x06);
    Inf_LCD_WriteData(0x2F);
    Inf_LCD_WriteData(0x45);
    Inf_LCD_WriteData(0x43);
    Inf_LCD_WriteData(0x02);
    Inf_LCD_WriteData(0x0A);
    Inf_LCD_WriteData(0x09);
    Inf_LCD_WriteData(0x32);
    Inf_LCD_WriteData(0x36);
    Inf_LCD_WriteData(0x0F);

    /* 3.  Adjust Control 3 (F7h)  */
    /*LCD_WriteCmd(0XF7);
   Inf_LCD_WriteData(0xA9);
   Inf_LCD_WriteData(0x51);
   Inf_LCD_WriteData(0x2C);
   Inf_LCD_WriteData(0x82);*/
    /* DSI write DCS command, use loose packet RGB 666 */

    /* 4. 电源控制1*/
    Inf_LCD_WriteCmd(0xC0);
    Inf_LCD_WriteData(0x11); /* 正伽马电压 */
    Inf_LCD_WriteData(0x09); /* 负伽马电压 */

    /* 5. 电源控制2 */
    Inf_LCD_WriteCmd(0xC1);
    Inf_LCD_WriteData(0x02);
    Inf_LCD_WriteData(0x03);

    /* 6. VCOM控制 */
    Inf_LCD_WriteCmd(0XC5);
    Inf_LCD_WriteData(0x00);
    Inf_LCD_WriteData(0x0A);
    Inf_LCD_WriteData(0x80);

    /* 7. Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
    Inf_LCD_WriteCmd(0xB1);
    Inf_LCD_WriteData(0xB0);
    Inf_LCD_WriteData(0x11);

    /* 8.  Display Inversion Control (B4h) （正负电压反转，减少电磁干扰）*/
    Inf_LCD_WriteCmd(0xB4);
    Inf_LCD_WriteData(0x02);

    /* 9.  Display Function Control (B6h)  */
    Inf_LCD_WriteCmd(0xB6);
    Inf_LCD_WriteData(0x0A);
    Inf_LCD_WriteData(0xA2);

    /* 10. Entry Mode Set (B7h)  */
    Inf_LCD_WriteCmd(0xB7);
    Inf_LCD_WriteData(0xc6);

    /* 11. HS Lanes Control (BEh) */
    Inf_LCD_WriteCmd(0xBE);
    Inf_LCD_WriteData(0x00);
    Inf_LCD_WriteData(0x04);

    /* 12.  Interface Pixel Format (3Ah) */
    Inf_LCD_WriteCmd(0x3A);
    Inf_LCD_WriteData(0x55); /* 0x55 : 16 bits/pixel  */

    /* 13. Sleep Out (11h) 关闭休眠模式 */
    Inf_LCD_WriteCmd(0x11);

    /* 14. 设置屏幕方向和RGB */
    Inf_LCD_WriteCmd(0x36);
    Inf_LCD_WriteData(0x08);

    Delay_ms(120);

    /* 14. display on */
    Inf_LCD_WriteCmd(0x29);
}

void Inf_LCD_Init(void)
{
    /* 1. 初始化 FSMC */
    Driver_FSMC_Init();

    /* 2. 重启LCD */
    Inf_LCD_Restart();

    /* 3. 设置LCD的(给LCD设置一些通用参数) */
    Inf_LCD_RegConfig();

    /* 4. 开启背光 */
    Inf_LCD_OpenBg();
}

/* 读取id,测试lcd是否正常 */
uint32_t Inf_LCD_ReadId(void)
{
    Inf_LCD_WriteCmd(0x04);
    Inf_LCD_ReadData();   // 垃圾数据

    uint32_t id = 0;

    id |= (Inf_LCD_ReadData() & 0xff) << 16;
    id |= (Inf_LCD_ReadData() & 0xff) << 8;
    id |= (Inf_LCD_ReadData() & 0xff) << 0;
    return id;
}

void Inf_LCD_SetArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    /* 1. 设置起始列和结束列 */
    Inf_LCD_WriteCmd(0x2A);
    /* 1.1 起始列 */
    Inf_LCD_WriteData(x >> 8);
    Inf_LCD_WriteData(x & 0xff);

    /* 1.2 结束列 */
    Inf_LCD_WriteData((x + w - 1) >> 8);
    Inf_LCD_WriteData((x + w - 1) & 0xff);

    /* 2. 设置起始行和结束行 */
    Inf_LCD_WriteCmd(0x2B);
    /* 1.1 起始行 */
    Inf_LCD_WriteData(y >> 8);
    Inf_LCD_WriteData(y & 0xff);

    /* 1.2 结束行 */
    Inf_LCD_WriteData((y + h - 1) >> 8);
    Inf_LCD_WriteData((y + h - 1) & 0xff);
}

#include "stdio.h"
/**
 * @description: 在lcd上显示一个asci码字符
 * @param {uint16_t} x
 * @param {uint16_t} y
 * @param {uint16_t} height 高度  16 | 24 | 32
 * @param {uint8_t} c 要显示的字符
 * @param {uint16_t} fColor 字符的颜色
 * @param {uint16_t} bColor 字符的背景色
 */
void Inf_LCD_ShowAsciChar(uint16_t x, uint16_t y, uint16_t height, uint8_t c, uint16_t fColor,
                          uint16_t bColor)
{
    /* 1. 先确定显示的区域 */
    Inf_LCD_SetArea(x, y, height / 2, height);

    /* 2. 计算出字符在字模数组中的索引  */
    uint8_t index = c - ' ';

    /* 3. 向lcd的控制器发送数据(颜色数据) */
    /* 3.1 发送颜色命令 */
    Inf_LCD_WriteCmd(0x2C);
    if(height == 16)
    {
        /* 3.2 找到数组中的每个字节 */
        for(uint8_t i = 0; i < height; i++)
        {
            uint8_t tmp = ascii_1608[index][i];

            /* 3.3 遍历tmp的每一位,如果是0发送背景色, 如果是1就发送前景色 */
            for(uint8_t j = 0; j < height / 2; j++)
            {
                if(tmp & 0x01)
                {
                    Inf_LCD_WriteData(fColor);
                }
                else
                {
                    Inf_LCD_WriteData(bColor);
                }
                tmp >>= 1;
            }
        }
    }
    else if(height == 24)
    {
        for(uint8_t i = 0; i < 48; i++)
        {
            uint8_t tmp    = ascii_2412[index][i];
            uint8_t jCount = i % 2 == 0 ? 8 : 4;
            for(uint8_t j = 0; j < jCount; j++)
            {
                if(tmp & 0x01)
                {
                    Inf_LCD_WriteData(fColor);
                }
                else
                {
                    Inf_LCD_WriteData(bColor);
                }
                tmp >>= 1;
            }
        }
    }
    else if(height == 32)
    {
        for(uint8_t i = 0; i < 64; i++)
        {
            uint8_t tmp = ascii_3216[index][i];

            for(uint8_t i = 0; i < 8; i++)
            {
                if(tmp & 0x01)
                {
                    Inf_LCD_WriteData(fColor);
                }
                else
                {
                    Inf_LCD_WriteData(bColor);
                }
                tmp >>= 1;
            }
        }
    }
}

void Inf_LCD_ShowAsciString(uint16_t x, uint16_t y, uint8_t height, uint8_t *str, uint16_t fColor,
                            uint16_t bColor)
{
    uint16_t w = height / 2;
    // ABC
    for(uint16_t i = 0; str[i] != '\0'; i++)
    {
        if(str[i] != '\n')
        {
            if(x + w > DISPLAY_W)
            {
                y += height;
                x = 0;
            }
            Inf_LCD_ShowAsciChar(x, y, height, str[i], fColor, bColor);
            x += w;
        }
        else
        {
            y += height;
            x = 0;
        }
    }
}

/* 显示一个汉字 */
void Inf_LCD_ShowChineseChar(uint16_t x, uint16_t y, uint8_t charIndex, uint16_t fColor,
                             uint16_t bColor)
{
    Inf_LCD_SetArea(x, y, 32, 32);

    Inf_LCD_WriteCmd(0x2C);

    for(uint16_t i = 0; i < 128; i++)
    {
        uint8_t tmp = chinese[charIndex][i];
        for(uint8_t i = 0; i < 8; i++)
        {
            if(tmp & 0x01)
            {
                Inf_LCD_WriteData(fColor);
            }
            else
            {
                Inf_LCD_WriteData(bColor);
            }
            tmp >>= 1;
        }
    }
}

void Inf_LCD_SetColor(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    Inf_LCD_SetArea(x, y, w, h);

    Inf_LCD_WriteCmd(0x2C);

    for(uint32_t i = 0, count = w * h; i < count; i++)
    {
        Inf_LCD_WriteData(color);
    }
}

void Inf_LCD_ShowImg(uint16_t x, uint16_t y, uint8_t *img, uint32_t len, uint16_t imgW,
                     uint16_t imgH)
{
    Inf_LCD_SetArea(x, y, imgW, imgH);

    Inf_LCD_WriteCmd(0x2C);

    for(uint32_t i = 0; i < len; i += 2)
    {
        uint16_t color = (img[i] << 8) + img[i + 1];
        Inf_LCD_WriteData(color);
    }
}

/* 绘制点 */
void Inf_LCD_ShowPoint(uint16_t x, uint16_t y, uint8_t w, uint16_t color)
{
    Inf_LCD_SetArea(x, y, w, w);

    Inf_LCD_WriteCmd(0x2C);

    for(uint8_t i = 0; i < w * w; i++)
    {
        Inf_LCD_WriteData(color);
    }
}

/* lcd绘制直行
 起点和终点
 y = kx + b
    y1 = kx1 + b
    y2 = kx2 + b
        k = (y2 - y1) / (x2 - x1)
        b = y1 - kx1

*/
void Inf_LCD_ShowLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t w, uint16_t color)
{
    if(x1 == x2)
    {
        for (uint16_t y = y1; y <= y2; y++)
        {
            Inf_LCD_ShowPoint(x1, y, w, color);
        }

        return;
        
    }

    double k = 1.0 * (y2 - y1) / (x2 - x1);
    double b = y1 - k * x1;

    uint16_t y = 0;
    for(uint16_t x = x1; x <= x2; x++)
    {
        y = (uint16_t)(k * x + b);
        Inf_LCD_ShowPoint(x, y, w, color);
    }
}

#include "math.h"
void Inf_LCD_ShowCircle(uint16_t centerX, uint16_t centerY, uint16_t r, uint8_t w, uint16_t color)
{
    for (double ang = 0; ang < 2 * 3.14; ang += 0.01)
    {
        uint16_t x = r * sin(ang) + centerX;
        uint16_t y = r * cos(ang) + centerY;

        Inf_LCD_ShowPoint(x, y, w, color);
    }
}
