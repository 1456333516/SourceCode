/*
 * @Author: wushengran
 * @Date: 2024-04-23 16:49:27
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "lcd.h"

int main(void)
{
	// 1. 初始化
	USART_Init();
	LCD_Init();

	printf("Hello world\n");

	// 2. 读取ID
	uint32_t id = LCD_ReadID();
	printf("id = %#X\n", id);

	// 3. 清屏
	LCD_ClearAll(WHITE);

	// 4. 显示一个英文字符
	LCD_WriteAsciiChar(20, 20, 16, 'A', RED, BLUE);
	LCD_WriteAsciiChar(20, 36, 32, 'A', RED, YELLOW);
	LCD_WriteAsciiChar(20, 70, 12, 'A', RED, YELLOW);

	// 5. 显示一个字符串
	LCD_WriteAsciiString(150, 50, 32, (uint8_t *)"Hello, Atguigu!\nHow are you!", BLACK, WHITE);

	// 6. 显示一个汉字
	LCD_WriteChineseChar(30, 320, 32, 0, RED, YELLOW);
	LCD_WriteChineseChar(30, 352, 32, 1, RED, YELLOW);
	LCD_WriteChineseChar(30, 384, 32, 2, RED, YELLOW);

	// 7. 显示图片：尚硅谷logo
	LCD_DisplayAtguiguLogo(50, 200);

	// 8. 画一个点
	LCD_DrawPoint(100, 320, 3, RED);

	// 9. 画一条线
	LCD_DrawLine(20, 300, 190, 400, 3, RED);

	// 10. 画一个矩形
	LCD_DrawRectangle(20, 300, 190, 400, 3, BLUE);

	// 11. 画圆
	LCD_DrawCircle(120, 360, 50, 3, BLUE);
	LCD_DrawFilledCircle(250, 360, 50, BLUE, RED);

	while (1)
	{
	}
}
