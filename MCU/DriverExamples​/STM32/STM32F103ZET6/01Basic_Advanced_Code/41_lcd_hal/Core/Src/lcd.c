#include "lcd.h"
#include "lcd_font.h"

// 1. 底层基本控制操作
// 初始化
void LCD_Init(void)
{
    // 1. 初始化 FSMC
    // FSMC_Init();
    MX_FSMC_Init();

    // 2. 重置复位
    LCD_Reset();

    // 3. 开启背光
    LCD_BGOn();

    // 4. 配置LCD寄存器
    LCD_RegConfig();
}

// 重置复位，PG15
void LCD_Reset(void)
{
    // 低电平复位
    GPIOG->ODR &= ~GPIO_ODR_ODR15;

    HAL_Delay(100);

    GPIOG->ODR |= GPIO_ODR_ODR15;

    HAL_Delay(100);
}

// 开关背光，PB0
void LCD_BGOn(void)
{
    GPIOB->ODR |= GPIO_ODR_ODR0;
}
void LCD_BGOff(void)
{
    GPIOB->ODR &= ~GPIO_ODR_ODR0;
}

// 发送命令（写命令）
void LCD_WriteCmd(uint16_t cmd)
{
    *LCD_ADDR_CMD = cmd;
}

// 发送数据（写入数据）
void LCD_WriteData(uint16_t data)
{
    *LCD_ADDR_DATA = data;
}

// 读取数据
uint16_t LCD_ReadData(void)
{
    return *LCD_ADDR_DATA;
}

// 配置LCD控制寄存器
void LCD_RegConfig(void)
{
    /* 1. 设置灰阶电压以调整TFT面板的伽马特性， 正校准。一般出厂就设置好了 */
    LCD_WriteCmd(0xE0);
    LCD_WriteData(0x00);
    LCD_WriteData(0x07);
    LCD_WriteData(0x10);
    LCD_WriteData(0x09);
    LCD_WriteData(0x17);
    LCD_WriteData(0x0B);
    LCD_WriteData(0x41);
    LCD_WriteData(0x89);
    LCD_WriteData(0x4B);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x0C);
    LCD_WriteData(0x0E);
    LCD_WriteData(0x18);
    LCD_WriteData(0x1B);
    LCD_WriteData(0x0F);

    /* 2. 设置灰阶电压以调整TFT面板的伽马特性，负校准 */
    LCD_WriteCmd(0XE1);
    LCD_WriteData(0x00);
    LCD_WriteData(0x17);
    LCD_WriteData(0x1A);
    LCD_WriteData(0x04);
    LCD_WriteData(0x0E);
    LCD_WriteData(0x06);
    LCD_WriteData(0x2F);
    LCD_WriteData(0x45);
    LCD_WriteData(0x43);
    LCD_WriteData(0x02);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x09);
    LCD_WriteData(0x32);
    LCD_WriteData(0x36);
    LCD_WriteData(0x0F);

    /* 3.  Adjust Control 3 (F7h)  */
    /*LCD_WriteCmd(0XF7);
   LCD_WriteData(0xA9);
   LCD_WriteData(0x51);
   LCD_WriteData(0x2C);
   LCD_WriteData(0x82);*/
    /* DSI write DCS command, use loose packet RGB 666 */

    /* 4. 电源控制1*/
    LCD_WriteCmd(0xC0);
    LCD_WriteData(0x11); /* 正伽马电压 */
    LCD_WriteData(0x09); /* 负伽马电压 */

    /* 5. 电源控制2 */
    LCD_WriteCmd(0xC1);
    LCD_WriteData(0x02);
    LCD_WriteData(0x03);

    /* 6. VCOM控制 */
    LCD_WriteCmd(0XC5);
    LCD_WriteData(0x00);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x80);

    /* 7. Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
    LCD_WriteCmd(0xB1);
    LCD_WriteData(0xB0);
    LCD_WriteData(0x11);

    /* 8.  Display Inversion Control (B4h) （正负电压反转，减少电磁干扰）*/
    LCD_WriteCmd(0xB4);
    LCD_WriteData(0x02);

    /* 9.  Display Function Control (B6h)  */
    LCD_WriteCmd(0xB6);
    LCD_WriteData(0x0A);
    LCD_WriteData(0xA2);

    /* 10. Entry Mode Set (B7h)  */
    LCD_WriteCmd(0xB7);
    LCD_WriteData(0xc6);

    /* 11. HS Lanes Control (BEh) */
    LCD_WriteCmd(0xBE);
    LCD_WriteData(0x00);
    LCD_WriteData(0x04);

    /* 12.  Interface Pixel Format (3Ah) */
    LCD_WriteCmd(0x3A);
    LCD_WriteData(0x55); /* 0x55 : 16 bits/pixel  */

    /* 13. Sleep Out (11h) 关闭休眠模式 */
    LCD_WriteCmd(0x11);

    /* 14. 设置屏幕方向和RGB */
    LCD_WriteCmd(0x36);
    LCD_WriteData(0x08);

    HAL_Delay(120);

    /* 14. display on */
    LCD_WriteCmd(0x29);
}

// 2. 包装应用层接口
// 2.1 返回ID信息
uint32_t LCD_ReadID(void)
{
    uint32_t id = 0;

    // 发送命令
    LCD_WriteCmd(0x04);

    LCD_ReadData(); // 先接收一个无效字符

    id |= (LCD_ReadData() & 0xff) << 16; // 第一个字节
    id |= (LCD_ReadData() & 0xff) << 8;  // 第二个字节
    id |= (LCD_ReadData() & 0xff) << 0;  // 第三个字节

    return id;
}

// 2.2 设置操作区域的范围，给定 起始点横纵坐标、区域的宽高
void LCD_SetArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    // 1. 设置列的开始和结束
    LCD_WriteCmd(0x2A);

    // 1.1 开始列
    LCD_WriteData(x >> 8);   // 取高8位发送
    LCD_WriteData(x & 0xff); // 取低8位发送

    // 1.2 结束列
    LCD_WriteData((x + w - 1) >> 8);   // 取高8位发送
    LCD_WriteData((x + w - 1) & 0xff); // 取低8位发送

    // 2. 设置行的开始和结束
    LCD_WriteCmd(0x2B);

    // 2.1 开始列
    LCD_WriteData(y >> 8);   // 取高8位发送
    LCD_WriteData(y & 0xff); // 取低8位发送

    // 2.2 结束列
    LCD_WriteData((y + h - 1) >> 8);   // 取高8位发送
    LCD_WriteData((y + h - 1) & 0xff); // 取低8位发送
}

// 2.3 清屏（整个屏幕用指定颜色来覆盖）
void LCD_ClearAll(uint16_t color)
{
    // 1. 设置区域为全屏
    LCD_SetArea(0, 0, 320, 480);

    // 2. 发送写显存命令
    LCD_WriteCmd(0x2C);

    // 3. 对所有像素点设置同样的颜色
    for (uint32_t i = 0; i < 320 * 480; i++)
    {
        LCD_WriteData(color);
    }
}

// 2.4 显示一个英文字符，给定显示区域左上角坐标、字体高、字体颜色、背景颜色
void LCD_WriteAsciiChar(uint16_t x, uint16_t y, uint16_t h, uint8_t c, uint16_t fColor, uint16_t bColor)
{
    // 1. 确定显示区域
    LCD_SetArea(x, y, h / 2, h);

    // 2. 发送写显存命令
    LCD_WriteCmd(0x2C);

    // 3. 计算字符在字库对应数组中的下标索引
    uint8_t index = c - ' ';

    // 4. 根据字体大小，判断当前选择哪个数组点阵
    if (h == 16 || h == 12)
    {
        // 双重for循环，遍历每一个像素点
        for (uint8_t i = 0; i < h; i++)
        {
            // 选择当前行对应点阵中的字节
            uint8_t rowByte = h == 16 ? ascii_1608[index][i] : ascii_1206[index][i];

            for (uint8_t j = 0; j < h / 2; j++)
            {
                // 从低位开始，遍历每一位
                if (rowByte & 0x01)
                {
                    LCD_WriteData(fColor); // 如果是1，就显示字体颜色
                }
                else
                {
                    LCD_WriteData(bColor); // 如果是0，就显示背景颜色
                }
                rowByte >>= 1; // 右移1位
            }
        }
    }
    else if (h == 24)
    {
        // 双重for循环，遍历每一个像素点
        for (uint8_t i = 0; i < h * 2; i++)
        {
            // 获取当前"行"对应点阵中的字节
            uint8_t rowByte = ascii_2412[index][i];

            // 定义j的取值范围，i为偶数取8位，为奇数取4位
            uint8_t jCount = i % 2 ? 4 : 8;

            for (uint8_t j = 0; j < jCount; j++)
            {
                // 从低位开始，遍历每一位
                if (rowByte & 0x01)
                {
                    LCD_WriteData(fColor); // 如果是1，就显示字体颜色
                }
                else
                {
                    LCD_WriteData(bColor); // 如果是0，就显示背景颜色
                }
                rowByte >>= 1; // 右移1位
            }
        }
    }
    else if (h == 32)
    {
        // 双重for循环，遍历每一个像素点
        for (uint8_t i = 0; i < h * 2; i++)
        {
            // 选择当前行对应点阵中的字节
            uint8_t rowByte = ascii_3216[index][i];

            for (uint8_t j = 0; j < 8; j++)
            {
                // 从低位开始，遍历每一位
                if (rowByte & 0x01)
                {
                    LCD_WriteData(fColor); // 如果是1，就显示字体颜色
                }
                else
                {
                    LCD_WriteData(bColor); // 如果是0，就显示背景颜色
                }
                rowByte >>= 1; // 右移1位
            }
        }
    }
}

// 2.5 显示英文字符串，给定显示区域左上角坐标、字体高、字体颜色、背景颜色
void LCD_WriteAsciiString(uint16_t x, uint16_t y, uint16_t h, uint8_t *str, uint16_t fColor, uint16_t bColor)
{
    uint8_t i = 0;
    // 判断如果是'\0'就退出，否则就循环处理下一个字符
    while (str[i] != '\0')
    {
        // 判断是否是'\n'，如果不是，x增加；如果是，y增加
        if (str[i] != '\n')
        {
            // 判断x增加后，是否超出了屏幕范围
            if (x + h / 2 > DISPLAY_W)
            {
                // 换行显示
                x = 0;
                y += h;
            }
            LCD_WriteAsciiChar(x, y, h, str[i], fColor, bColor);
            x += h / 2;
        }
        else
        {
            // 直接换行
            x = 0;
            y += h;
        }
        i++;
    }
}

// 2.6 显示一个汉字，给定显示区域左上角坐标、字体高、字体颜色、背景颜色、在字库中的索引
void LCD_WriteChineseChar(uint16_t x, uint16_t y, uint16_t h, uint8_t index, uint16_t fColor, uint16_t bColor)
{
    LCD_SetArea(x, y, h, h);

    LCD_WriteCmd(0x2C);

    // 双重for循环，遍历每一个像素点
    for (uint8_t i = 0; i < h * h / 8; i++)
    {
        // 选择当前行对应点阵中的字节
        uint8_t rowByte = chinese[index][i];

        for (uint8_t j = 0; j < 8; j++)
        {
            // 从低位开始，遍历每一位
            if (rowByte & 0x01)
            {
                LCD_WriteData(fColor); // 如果是1，就显示字体颜色
            }
            else
            {
                LCD_WriteData(bColor); // 如果是0，就显示背景颜色
            }
            rowByte >>= 1; // 右移1位
        }
    }
}

// 2.7 显示一张图片，尚硅谷logo
void LCD_DisplayAtguiguLogo(uint16_t x, uint16_t y)
{
    LCD_SetArea(x, y, 227, 68);

    LCD_WriteCmd(0x2C);

    // 获取数组长度
    uint16_t len = sizeof(gImage_logo);

    // 遍历数组，每两个字节拼成一个16位的RGB像素点数据
    for (uint16_t i = 0; i < len; i += 2)
    {
        // 拼接像素点RGB数据
        uint16_t px = gImage_logo[i] + (gImage_logo[i + 1] << 8);
        LCD_WriteData(px);
    }
}

// 2.8 画一个点/画一个正方形（实心），给定左上角坐标
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t w, uint16_t color)
{
    LCD_SetArea(x, y, w, w);

    LCD_WriteCmd(0x2C);

    for (uint16_t i = 0; i < w * w; i++)
    {
        LCD_WriteData(color);
    }
}

// 2.9 画一条线
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t w, uint16_t color)
{
    // 判断如果横坐标相同，直接画一条竖线
    if (x1 == x2)
    {
        for (uint16_t y = y1; y <= y2; y++)
        {
            LCD_DrawPoint(x1, y, w, color);
        }
        return;
    }

    // 计算斜率和截距
    double k = 1.0 * (y2 - y1) / (x2 - x1);
    double b = y1 - k * x1;

    // 根据直线方程，画出所有点
    for (uint16_t x = x1; x <= x2; x++)
    {
        uint16_t y = (uint16_t)(k * x + b);
        LCD_DrawPoint(x, y, w, color);
    }
}

// 2.10 画一个长方形（空心）
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t w, uint16_t color)
{
    LCD_DrawLine(x1, y1, x2, y1, w, color);
    LCD_DrawLine(x2, y1, x2, y2, w, color);
    LCD_DrawLine(x1, y2, x2, y2, w, color);
    LCD_DrawLine(x1, y1, x1, y2, w, color);
}

// 2.11 画一个圆（空心），给定圆心坐标和半径
void LCD_DrawCircle(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color)
{
    // 用极坐标进行计算
    for (uint16_t theta = 0; theta < 360; theta++)
    {
        uint16_t x = xCenter + r * cos(theta * 3.14 / 180);
        uint16_t y = yCenter + r * sin(theta * 3.14 / 180);

        LCD_DrawPoint(x, y, w, color);
    }
}

// 2.12 画一个实心圆
void LCD_DrawFilledCircle(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t fColor, uint16_t bColor)
{
    // 双重for循环，让r递增
    for (uint16_t ri = 0; ri <= r; ri++)
    {
        // 用极坐标进行计算
        for (uint16_t theta = 0; theta < 360; theta++)
        {
            uint16_t x = xCenter + ri * cos(theta * 3.14 / 180);
            uint16_t y = yCenter + ri * sin(theta * 3.14 / 180);

            if (ri == r)
            {
                LCD_DrawPoint(x, y, 1, bColor);
            }
            else
            {
                LCD_DrawPoint(x, y, 1, fColor);
            }       
        }
    }
}
